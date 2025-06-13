/*! @mainpage Proyecto Final Programable
 *
 * @section genDesc General Description
 *
 * Utilización de un microcontrolador que trabaja conjuntamente con un sensor láser  y un sensor de movimiento
 * tipo PIR. Si se activa únicamente el sensor láser, se interpreta como una salida del paciente, mientras que
 * si ambos sensores se activan en un intervalo breve de tiempo, se asume que se trata de una entrada,
 * y por tanto, no se genera una alerta.
 * Ante la detección de una salida no permitida, el sistema activa una señal visual mediante un  LED ,
 * y envía un  mensaje de aviso. Se contempla la inclusión de una fuente
 * de alimentación.
 *
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
 * | 23/05/2025 | Document creation		                         |
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
#include "sensor_pir.h"
#include "receptor_laser.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
uint16_t distancia;
uint8_t teclas;
bool laser_detected;
/*==================[internal functions declaration]=========================*/

static void Medir_task()
{
    while (true)
    {
        distancia = HcSr04ReadDistanceInCentimeters();
        LcdItsE0803Write(distancia);
        laser_detected = Laser_receiverRead();



        if (distancia > 20 && laser_detected)
        {
            LedOn(LED_1);
            LedOn(LED_2);
            LedOn(LED_3);
            UartSendString(UART_PC, ">Alerta: Posible salida no autorizada");
            UartSendString(UART_PC, "\r\n");
        }
        else
        {
            LedOff(LED_1);
            LedOff(LED_2);
            LedOff(LED_3);
            UartSendString(UART_PC, ">Todo normal");
            UartSendString(UART_PC, "\r\n");
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    HcSr04Init(GPIO_3, GPIO_2);
    Laser_receiverPirInit(GPIO_1);
    LcdItsE0803Init();
    LedsInit();


    serial_config_t my_uart = {
        .baud_rate = 19200,
        .port = UART_PC,
        .func_p = NULL,
        .param_p = NULL,
    };
    UartInit(&my_uart);

    xTaskCreate(&Medir_task, "Medir", 4096, NULL, 5, NULL);
}
/*==================[end of file]============================================*/