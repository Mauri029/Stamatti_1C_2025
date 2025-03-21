/*! @mainpage Blinking switch
 *
 * \section genDesc General Description
 *
 * This example makes LED_1 and LED_2 blink if SWITCH_1 or SWITCH_2 are pressed.
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Mauricio Stamatti
 *
 */

 #include <stdio.h>
 #include <stdint.h>
 #include <stdbool.h>
 #include <string.h>
 #include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "led.h"
 #include "switch.h"
 
 
 #define CONFIG_BLINK_PERIOD 1000
 #define ON 0
 #define OFF 1
 #define TOGGLE 2
 
 struct leds
 {
	uint8_t mode;
	 uint8_t n_led;        
	 uint8_t n_ciclos;   
	 uint16_t periodo;    
 };
 
 void Funcion_prueba(struct leds *led)
 {
	switch (led->mode)
	{
	case ON: // Encender LED
	switch (led->n_led)
	{
	case 1:
		LedOn(LED_1);
		break;
	case 2:
		LedOn(LED_2);
		break;
	
	default:
		LedOn(LED_3);
		break;
	}
		break;

	
	case OFF: // Apagar LED
	switch (led->n_led)
	{
	case 1:
		LedOff(LED_1);
		break;
	case 2:
		LedOff(LED_2);
		break;
	
	default:
		LedOff(LED_3);
		break;
	}
		break;


	case TOGGLE: // Parpadear LED
	int i=0;	
		while ( i < led->n_ciclos*2)
		{
		
		switch (led->n_led)
		{
		case 1:
		LedToggle(LED_1);
			break;
		case 2:
		LedToggle(LED_2);
			break;
		
		default:
		LedToggle(LED_3);
			break;
		}
			
		for (size_t j = 0; j < 10; j++)
		{
			vTaskDelay(led->periodo / portTICK_PERIOD_MS);
		}	
				i++;
			}
				break;
					
			default:
				break;
			
}
	 
 }


 
 void app_main(void){
	 LedsInit();
	 SwitchesInit();
 
	 struct leds my_leds;
	 my_leds.mode = TOGGLE;
	 my_leds.n_led = 3;
	 my_leds.n_ciclos = 15;
	 my_leds.periodo = 250;
 
	 Funcion_prueba(&my_leds);
 }
 