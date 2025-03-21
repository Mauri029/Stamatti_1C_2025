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
 #include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "led.h"
 #include "switch.h"
 
 #define CONFIG_BLINK_PERIOD 1000
 #define TOGGLE 2  // Definir TOGGLE si no existe
 
 struct leds
 {
	 uint8_t mode;       
	 uint8_t n_led;        
	 uint8_t n_ciclos;   
	 uint16_t periodo;    
 };
 
 void Funcion_prueba(struct leds *led)
 {
	 if (led->mode == 1)  // Encender LED
	 {
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
		 /*if (led->n_led == 1)
		 {
			 LedOn(LED_1);
		 }
		 else if (led->n_led == 2)
		 {
			 LedOn(LED_2);
		 }
		 else
		 {
			 LedOn(LED_3);
		 }*/
	 }
	 else if (led->mode == 0)  // Apagar LED
	 { 
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
		 /*if (led->n_led == 1)
		 {
			 LedOff(LED_1);
		 }
		 else if (led->n_led == 2)
		 {
			 LedOff(LED_2);
		 }
		 else
		 {
			 LedOff(LED_3);
		 }*/
	 }
	 else if (led->mode == TOGGLE)  // Parpadear LED
	 {
		 for (int i = 0; i < led->n_ciclos; i++)
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
			/* if (led->n_led == 1)
			 {
				 LedToggle(LED_1);
			 }
			 else if (led->n_led == 2)
			 {
				 LedToggle(LED_2);
			 }
			 else
			 {
				 LedToggle(LED_3);
			 }*/
			 vTaskDelay(led->periodo / portTICK_PERIOD_MS);
		 }
	 }
 }
 
 void app_main(void){
	 uint8_t teclas;
	 LedsInit();
	 SwitchesInit();
 
	 struct leds my_leds;
	 my_leds.mode = 2;
	 my_leds.n_led = 1;
	 my_leds.n_ciclos = 10;
	 my_leds.periodo = 100;
 
	 Funcion_prueba(&my_leds);
 }
 