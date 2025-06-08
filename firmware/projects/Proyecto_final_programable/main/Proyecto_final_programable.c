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
bool on_off_pantalla = true;
bool hold = false;
uint16_t distancia;
uint8_t teclas;
/*==================[internal functions declaration]=========================*/



static void Medir_task()
{
    while (true)
    {
            distancia = HcSr04ReadDistanceInCentimeters();

            if (distancia < 20)
            {
                LedOn(LED_1);
                LedOn(LED_2);
                LedOn(LED_3);
			}
                LcdItsE0803Write(distancia);
                
            
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void Teclas_task()
{
    while (true)
    {
        teclas = SwitchesRead();
        switch (teclas)
        {
        case SWITCH_1:
            on_off_pantalla = !on_off_pantalla;
            break;
        case SWITCH_2:
            hold = !hold;

            break;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void app_main(void)
{
    HcSr04Init(GPIO_3, GPIO_2);
    LcdItsE0803Init();
    LedsInit();
    SwitchesInit();
    xTaskCreate(&Medir_task, "Medir", 2048, NULL, 1, NULL);
    xTaskCreate(Teclas_task, "Teclas", 2048, NULL, 1, NULL);
}
/*==================[end of file]============================================*/