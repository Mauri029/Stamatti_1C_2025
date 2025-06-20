/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "hc_sr04.h"           // Driver del sensor ultrasónico HC-SR04
#include "gpio_mcu.h"          // Control de GPIOs
#include "analog_io_mcu.h"     // Lectura de entradas analógicas (acelerómetro)
#include "led.h"               // Control de LEDs
#include "buzzer.h"            // Control del buzzer
#include "timer_mcu.h"         // Temporizadores (no usado en este código pero útil si se agrega)
#include "freertos/FreeRTOS.h" // Kernel FreeRTOS
#include "freertos/task.h"     // Tareas de FreeRTOS
#include "ble_mcu.h"           // Driver BLE emulando HM-10


/*==================[macros and definitions]=================================*/
// Períodos de muestreo en milisegundos
#define DIST_TASK_PERIOD_MS     500     // Cada cuánto se mide la distancia
#define ACC_TASK_PERIOD_MS      10      // Cada cuánto se mide aceleración

// Parámetros del acelerómetro
#define ACC_THRESHOLD_G         4.0     // Umbral de aceleración para detectar caída
#define ACC_0G_VOLTAGE          1.65    // Voltaje de referencia cuando no hay aceleración (0 g)
#define ACC_SENSITIVITY         0.3     // Sensibilidad del acelerómetro (V/g)
#define ADC_REF_VOLTAGE         3.3     // Voltaje de referencia del ADC
#define ADC_RESOLUTION          4096    // Resolución del ADC (12 bits)

// Umbrales de distancia en centímetros
#define DANGER_DISTANCE_CM      300     // Distancia considerada peligrosa
#define WARNING_DISTANCE_CM     500     // Distancia de advertencia

// Pines y canales
#define BUZZER_GPIO             GPIO_22 // Pin del buzzer
#define ACC_X_CHANNEL           CH1     // Canal ADC del eje X
#define ACC_Y_CHANNEL           CH2     // Canal ADC del eje Y
#define ACC_Z_CHANNEL           CH3     // Canal ADC del eje Z

/*==================[internal data definition]===============================*/
// Configuración del módulo BLE
static ble_config_t ble_config = {
    .device_name = "ESP-EDU BIKE",      // Nombre visible por BLE
    .func_p = BLE_NO_INT                // No se usa interrupción por recepción
};

static uint8_t danger_counter = 0;      // Contador para controlar frecuencia del buzzer

/*==================[internal functions declaration]=========================*/
static float ReadAcceleration(void);              // Lee y calcula aceleración total
static void DistanceTask(void *pvParameters);     // Tarea de monitoreo de distancia
static void AccelerationTask(void *pvParameters); // Tarea de monitoreo de caídas

/*==================[external functions definition]==========================*/
void app_main(void){
    // Inicialización de periféricos y tareas
    BleInit(&ble_config);                        // Inicializa BLE
    HcSr04Init(GPIO_3, GPIO_2);                  // Inicializa HC-SR04 con pines ECHO y TRIG

    // Inicialización de entradas analógicas (acelerómetro)
    AnalogInputInit(ACC_X_CHANNEL);
    AnalogInputInit(ACC_Y_CHANNEL);
    AnalogInputInit(ACC_Z_CHANNEL);

    // Inicialización del buzzer y LEDs
    GpioInit(BUZZER_GPIO, GPIO_OUTPUT);
    LedInit(LED_1); // Verde
    LedInit(LED_2); // Amarillo
    LedInit(LED_3); // Rojo

    // Creación de tareas periódicas
    xTaskCreate(&DistanceTask, "distance_task", 512, NULL, 1, NULL);
    xTaskCreate(&AccelerationTask, "accel_task", 512, NULL, 1, NULL);
}

/*==================[internal functions definition]==========================*/

// Tarea que mide distancia y actualiza LEDs, buzzer y BLE según cercanía de objetos
static void DistanceTask(void *pvParameters) {
    while (1) {
        uint16_t distance = HcSr04ReadDistanceInCentimeters(); // Lee distancia

        if (distance > WARNING_DISTANCE_CM) {
            // No hay peligro: LED verde
            LedOn(LED_1);
            LedOff(LED_2);
            LedOff(LED_3);
            GpioWrite(BUZZER_GPIO, false); // Silencio
        } else if (distance > DANGER_DISTANCE_CM) {
            // Advertencia: LED verde + amarillo y pitido alternado
            LedOn(LED_1);
            LedOn(LED_2);
            LedOff(LED_3);
            if (++danger_counter % 2 == 0)
                GpioWrite(BUZZER_GPIO, true);
            else
                GpioWrite(BUZZER_GPIO, false);
            BleSendString("Precaucion, vehiculo cerca\n");
        } else {
            // Peligro: LED rojo + buzzer parpadea más rápido
            LedOn(LED_1);
            LedOn(LED_2);
            LedOn(LED_3);
            GpioWrite(BUZZER_GPIO, !GpioRead(BUZZER_GPIO)); // Inversión de estado
            BleSendString("Peligro, vehiculo cerca\n");
        }

        // Espera hasta el próximo ciclo de lectura
        vTaskDelay(pdMS_TO_TICKS(DIST_TASK_PERIOD_MS));
    }
}

// Lee aceleraciones en los 3 ejes y devuelve la suma de sus valores absolutos (g)
static float ReadAcceleration(void) {
    // Lectura cruda de ADC (12 bits)
    uint16_t x = AnalogInputReadSingle(ACC_X_CHANNEL);
    uint16_t y = AnalogInputReadSingle(ACC_Y_CHANNEL);
    uint16_t z = AnalogInputReadSingle(ACC_Z_CHANNEL);

    // Conversión a voltaje
    float v_x = ((float)x * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    float v_y = ((float)y * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    float v_z = ((float)z * ADC_REF_VOLTAGE) / ADC_RESOLUTION;

    // Conversión a aceleración en g
    float a_x = (v_x - ACC_0G_VOLTAGE) / ACC_SENSITIVITY;
    float a_y = (v_y - ACC_0G_VOLTAGE) / ACC_SENSITIVITY;
    float a_z = (v_z - ACC_0G_VOLTAGE) / ACC_SENSITIVITY;

    // Aceleración total (simplificada como suma de valores absolutos)
    float total_a = fabs(a_x) + fabs(a_y) + fabs(a_z);
    return total_a;
}

// Tarea que detecta caídas en base a aceleración total
static void AccelerationTask(void *pvParameters) {
    while (1) {
        float acc_total = ReadAcceleration();
        if (acc_total > ACC_THRESHOLD_G) {
            BleSendString("Caida detectada\n");
        }
        vTaskDelay(pdMS_TO_TICKS(ACC_TASK_PERIOD_MS));
    }
}

/*==================[end of file]============================================*/
