/*! @mainpage Blinking switch
 *
 * @section genDesc General Description
 *
 * Este ejemplo hace que el LED_1 o LED_2 parpadeen al presionar SWITCH_1 o SWITCH_2, respectivamente.
 * Si ambos switches son presionados simultáneamente, se alterna el estado del LED_3.
 *
 * @section changelog Changelog
 *
 * |   Date     | Description          |
 * |:----------:|:---------------------|
 * | 12/09/2023 | Document creation    |
 *
 * @author Mauricio Stamatti
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"

/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 1000 /*!< Período de parpadeo en milisegundos */

/*==================[external functions definition]==========================*/


void app_main(void){
	uint8_t teclas;
	LedsInit();
	SwitchesInit();

	while(1){
		teclas = SwitchesRead();
		switch(teclas){
			case SWITCH_1:
				LedToggle(LED_1);
				break;
			case SWITCH_2:
				LedToggle(LED_2);
				break;
			case SWITCH_1 | SWITCH_2:
				LedToggle(LED_3);
				break;
		}
		// Descomentar para agregar retardo entre lecturas
		// vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
}

/*==================[end of file]============================================*/
