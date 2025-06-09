/*! @mainpage Examen Final Programable
 *
 * @section genDesc General Description
 *
 * Programa que debe cumplir con un sistema de monitoreo de temperatura, humedad y radiación.
 * Donde por arriba de cierto valor de temperatura, presion o radiacion da alarmas. Ademas encia los valores por UART.
 *
 *
 * @section hardConn Hardware Connection
 *
 * |   	DHT11		|   ESP-EDU 	|
 * |:--------------:|:--------------|
 * | 	VCC     	|	3V3     	|
 * | 	DATA	 	| 	GPIO_6		|
 * | 	GND		 	| 	GND 		|
 *
 * |  Analog_input	|   ESP-EDU 	|
 * |:--------------:|:--------------|
 * | 	DATA	 	| 	CH1	        |
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 19/06/2025 | Document creation		                         |
 *
 * @author Mauricio Stamatti
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "lcditse0803.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "hc_sr04.h"
#include "lcditse0803.h"
#include "switch.h"
#include "timer_mcu.h"
#include "uart_mcu.h"
#include "analog_io_mcu.h"
#include "dht11.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD_sensor_presion_temperatura 1000
#define CONFIG_BLINK_PERIOD_sensor_radiacion 5000

bool estado_sensores_y_notificacion = false; // Variable para controlar el estado de los sensores y la notificación(empieza apagado)
float humedad = 0, temperatura = 0;
uint16_t radicacion_leida = 0;

/*==================[internal data definition]===============================*/
TaskHandle_t medir_presion_y_temperatura_task_handle = NULL;
TaskHandle_t medir_radiacion_task_handle = NULL;
TaskHandle_t todo_bien_task_handle = NULL;

/*==================[internal functions declaration]=========================*/

/**
 * @fn void medir_presion_y_temperatura_task
 * @brief Tarea para medir la presion y temperatura ademas de notificar por puerto UART
 * @note Esta tarea lee los valores de temperatura y humedad del sensor DHT11, y si la humedad es mayor a 85% y 
 * la temperatura entre 0 y 2 grados Celsius, enciende el LED rojo y envía una notificación por UART. Sino envia 
 * constantemente el valor de la temperatura y humedad.
 * @param None
 * @return None
 */
static void medir_presion_y_temperatura_task()
{
	while (true)
	{
		if (estado_sensores_y_notificacion)
		{

			dht11Read(&humedad, &temperatura);
			if (humedad > 85 && (temperatura > 0 && temperatura < 2))
			{
				LedOn(LED_3);  // led rojo
				LedOff(LED_1); // led verde
				UartSendString(UART_PC, ">Temperatura: ");
				UartSendString(UART_PC, (char *)UartItoa(temperatura, 10));
				UartSendString(UART_PC, "°C- ");
				UartSendString(UART_PC, ">Humedad: ");
				UartSendString(UART_PC, (char *)UartItoa(humedad, 10));
				UartSendString(UART_PC, "%");
				UartSendString(UART_PC, " - RIESGO DE NEVADA");
				UartSendString(UART_PC, "\r\n");
			}
			else
			{
				UartSendString(UART_PC, ">Temperatura: ");
				UartSendString(UART_PC, (char *)UartItoa(temperatura, 10));
				UartSendString(UART_PC, "°C- ");
				UartSendString(UART_PC, ">Humedad: ");
				UartSendString(UART_PC, (char *)UartItoa(humedad, 10));
				UartSendString(UART_PC, "%");
				UartSendString(UART_PC, "\r\n");
			}
		}
		vTaskDelay(CONFIG_BLINK_PERIOD_sensor_presion_temperatura / portTICK_PERIOD_MS);
	}
}
/**
 * @fn leer_entrada_analogica
 * @brief Lee la entrada analogica del sensor de radiacion
 * @note Esta funcion lee el valor de la entrada analogica del sensor de radiacion y lo convierte a mR/h.
 * @param None
 * @return valor de la entrada analogica
 */
uint16_t leer_entrada_analogica()
{
	AnalogInputReadSingle(CH1, &radicacion_leida);
	radicacion_leida = (radicacion_leida / 4095) * 100; // lo convierte a mR/h, siendo 100 el maximo
	return radicacion_leida;
}

/**
 * @fn medir_radiacion_task
 * @brief mide la radiacion,  y notifica por puerto UART
 * @note mide la radiacion a traves de la entrada analogica y si el valor es mayor a 40 mR/h,
 *  enciende el LED amarillo y envía una notificación por UART. Sino notifica constantemente el valor de la radiacion.
 * @param None
 * @return None
 */
static void medir_radiacion_task()
{
	while (true)
	{
		if (estado_sensores_y_notificacion)
		{
			if (leer_entrada_analogica() > 40)
			{
				LedOn(LED_2);  // led amarillo
				LedOff(LED_1); // led verde
				UartSendString(UART_PC, ">Radiacion: ");
				UartSendString(UART_PC, (char *)UartItoa(radicacion_leida, 10));
				UartSendString(UART_PC, " mR/h ");
				UartSendString(UART_PC, " - RADIACION ELEVADA");
				UartSendString(UART_PC, "\r\n");
			}
			else
			{
				UartSendString(UART_PC, ">Radiacion: ");
				UartSendString(UART_PC, (char *)UartItoa(radicacion_leida, 10));
				UartSendString(UART_PC, " mR/h ");
				UartSendString(UART_PC, "\r\n");
			}
		}
		vTaskDelay(CONFIG_BLINK_PERIOD_sensor_presion_temperatura / portTICK_PERIOD_MS);
	}
}

/**
 * @fn apagar_sensores_y_notificacion
 * @brief Apaga los sensores y las notificaciónes
 * @note Esta función apaga los sensores y las notificaciónes.
 * @param None
 * @return None
 */
void apagar_sensores_y_notificacion()
{
	estado_sensores_y_notificacion = false;
}

/**
 * @fn prender_sensores_y_notificacion
 * @brief Enciende los sensores y las notificaciónes
 * @note Esta función enciende los sensores y las notificaciónes.
 * @param None
 * @return None
 */
void prender_sensores_y_notificacion()
{
	estado_sensores_y_notificacion = true;
}

/**
 * @fn todo_bien_task
 * @brief Verifica si los sensores están en un estado seguro
 * @note Esta función verifica si los sensores están en un estado seguro.
 * @param None
 * @return None
 */
static void todo_bien_task()
{
	while (true)
	{
		if (humedad < 85 && leer_entrada_analogica() < 40 && temperatura > 0 && temperatura < 2)
		{
			LedOn(LED_1);  // led verde
			LedOff(LED_2); // led amarillo
			LedOff(LED_3); // led rojo
		}
		vTaskDelay(CONFIG_BLINK_PERIOD_sensor_presion_temperatura / portTICK_PERIOD_MS);
	}
}

/*==================[external functions definition]==========================*/
void app_main(void)
{

	
	analog_input_config_t adc_config = {
		CH1,		/*!< Inputs: CH0, CH1, CH2, CH3 */
		ADC_SINGLE, /*!< Mode: single read or continuous read */
		NULL,		/*!< Pointer to callback function for convertion end (only for continuous mode) */
		NULL,
		0, /*!< Pointer to callback function parameters (only for continuous mode) */
	};

	AnalogInputInit(&adc_config);


	serial_config_t my_uart = {
		.baud_rate = 9600,
		.port = UART_PC,
		.func_p = NULL,
		.param_p = NULL,
	};
	UartInit(&my_uart);

	dht11Init(GPIO_6);
	LedsInit();
	SwitchesInit();

	SwitchActivInt(SWITCH_1, prender_sensores_y_notificacion, NULL);
	SwitchActivInt(SWITCH_2, apagar_sensores_y_notificacion, NULL);
	xTaskCreate(&medir_presion_y_temperatura_task, "Medir_temperatura_y_presion", 2048, NULL, 5, &medir_presion_y_temperatura_task_handle);
	xTaskCreate(&medir_radiacion_task, "Medir_radiacion", 2048, NULL, 5, &medir_radiacion_task_handle);
	xTaskCreate(&todo_bien_task, "Dice_si_esta_todo_bien", 2048, NULL, 5, &todo_bien_task_handle);
}
/*==================[end of file]============================================*/