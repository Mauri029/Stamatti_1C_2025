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
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void)
{
}
/*==================[end of file]============================================*/