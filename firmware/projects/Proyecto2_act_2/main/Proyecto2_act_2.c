/*! @mainpage Actividad 2 del proyecto 2
 *
 * @section genDesc General Description
 *
 *El programa mide la distancia , prende los led segun la distancia que mida, muestra la distancia en una
 * pantalla lcd y prende o apaga la medicion y mantiene el valor en pantalla con interrupciones
 *
 *
 * @section hardConn Hardware Connection
 * \brief Driver for using the 3 digits numeric display in ESP-EDU.
 *
 * |   Display      |   EDU-CIAA	|
 * |:--------------:|:-------------:|
 * | 	Vcc 	    |	5V      	|
 * | 	BCD1		| 	GPIO_20		|
 * | 	BCD2	 	| 	GPIO_21		|
 * | 	BCD3	 	| 	GPIO_22		|
 * | 	BCD4	 	| 	GPIO_23		|
 * | 	SEL1	 	| 	GPIO_19		|
 * | 	SEL2	 	| 	GPIO_18		|
 * | 	SEL3	 	| 	GPIO_9		|
 * | 	Gnd 	    | 	GND     	|
 *
 * \brief Driver for reading distance with HC-SR04 module.
 * |   HC_SR04      |   EDU-CIAA	|
 * |:--------------:|:-------------:|
 * | 	Vcc 	    |	5V      	|
 * | 	Echo		| 	GPIO_3		|
 * | 	Trig	 	| 	GPIO_2		|
 * | 	Gnd 	    | 	GND     	|
 *
 *
 *
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

/*==================[macros and definitions]=================================*/
bool on_off_pantalla = true;
bool hold = false;
uint16_t distancia;
uint8_t teclas;

/*==================[internal data definition]===============================*/
TaskHandle_t Medir_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
/**
 * @fn static void Medir_task()
 * @brief Tarea encargada de medir la distancia mediante el sensor HC-SR04.
 * @note Según el valor medido, enciende un número determinado de LEDs, actualiza el display LCD (si no está en hold)
 * y apaga el sistema si está deshabilitado.
 * @param None
 * @return None
 */
static void Medir_task()
{
	while (true)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (on_off_pantalla)
		{
			distancia = HcSr04ReadDistanceInCentimeters();

			if (distancia <= 10)
			{
				LedOff(LED_1);
				LedOff(LED_2);
				LedOff(LED_3);
			}
			else if (distancia > 10 && distancia <= 20)
			{
				LedOn(LED_1);
				LedOff(LED_2);
				LedOff(LED_3);
			}
			else if (distancia > 20 && distancia <= 30)
			{
				LedOn(LED_1);
				LedOn(LED_2);
				LedOff(LED_3);
			}
			else
			{
				LedOn(LED_1);
				LedOn(LED_2);
				LedOn(LED_3);
			}
			if (!hold)
			{
				LcdItsE0803Write(distancia);
			}
		}
		else
		{
			LcdItsE0803Off();
			LedsOffAll();
		}
	}
}
/**
 * @fn void On_off_pantalla()
 * @brief Alterna el estado de encendido/apagado de la medición y el display.
 * @note Asociada a la interrupción por botón físico (SWITCH_1).
 * @param None
 * @return None
 */
void On_off_pantalla()
{
	on_off_pantalla = !on_off_pantalla;
}
/**
 * @fn void Hold()
 * @brief Alterna el estado de "hold" del display.
 * @note Si está activado, mantiene el valor actual en pantalla aunque cambie la distancia.
 * @param None
 * @return None
 */
void Hold()
{
	hold = !hold;
}
/**
 * @fn void FuncTimerA(void *param)
 * @brief Función de callback del temporizador.
 * @note Notifica a la tarea de medición para que se ejecute.
 * @param param No utilizado.
 * @return None
 */
void FuncTimerA(void *param)
{
	vTaskNotifyGiveFromISR(Medir_task_handle, pdFALSE);
}
/*==================[external functions definition]==========================*/
void app_main(void)
{
	HcSr04Init(GPIO_3, GPIO_2);
	LcdItsE0803Init();
	LedsInit();
	SwitchesInit();

	timer_config_t timer_led_1 = {
		.timer = TIMER_A,
		.period = 1000000,
		.func_p = FuncTimerA,
		.param_p = NULL};

	TimerInit(&timer_led_1);

	SwitchActivInt(SWITCH_1, On_off_pantalla, NULL);
	SwitchActivInt(SWITCH_2, Hold, NULL);
	xTaskCreate(&Medir_task, "Medir", 2048, NULL, 5, &Medir_task_handle);
	TimerStart(timer_led_1.timer);
}
/*==================[end of file]============================================*/