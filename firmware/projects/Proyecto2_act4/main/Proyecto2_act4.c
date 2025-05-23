/*! @mainpage Proyecto 2 Actividad 4
 *
 * @section genDesc General Description
 *
 * En esta actividad se usara la entrada adc para leer un valor continuo y pasarlo a digital, para luego leerlo en la computadora como un
 * osciloscopio, luego se usara una senial ecg digital y pasarla a analogica, luego unir ambas cosas.
 *
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	Ch1		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Mauricio  Stamatti
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
unsigned char ECG[] = {
	17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 17, 17, 16, 16, 16, 16, 17, 17, 18, 18, 18, 17, 17, 17, 17,
	18, 18, 19, 21, 22, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34, 34, 35, 37, 38, 37, 34, 29, 24, 19, 15, 14, 15, 16, 17, 17, 17, 16, 15, 14, 13, 13, 13, 13, 13, 13, 13, 12, 12,
	10, 6, 2, 3, 15, 43, 88, 145, 199, 237, 252, 242, 211, 167, 117, 70, 35, 16, 14, 22, 32, 38, 37, 32, 27, 24, 24, 26, 27, 28, 28, 27, 28, 28, 30, 31, 31, 31, 32, 33, 34, 36,
	38, 39, 40, 41, 42, 43, 45, 47, 49, 51, 53, 55, 57, 60, 62, 65, 68, 71, 75, 79, 83, 87, 92, 97, 101, 106, 111, 116, 121, 125, 129, 133, 136, 138, 139, 140, 140, 139, 137,
	133, 129, 123, 117, 109, 101, 92, 84, 77, 70, 64, 58, 52, 47, 42, 39, 36, 34, 31, 30, 28, 27, 26, 25, 25, 25, 25, 25, 25, 25, 25, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25,
	24, 24, 24, 24, 24, 24, 24, 24, 23, 23, 22, 22, 21, 21, 21, 20, 20, 20, 20, 20, 19, 19, 18, 18, 18, 19, 19, 19, 19, 18, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 17, 17, 17, 17,
	17, 17, 17

};
/*==================[internal data definition]===============================*/
TaskHandle_t leer_senial_analogica_handle = NULL;
TaskHandle_t leer_ecg_handle = NULL;
/*==================[internal functions declaration]=========================*/
void FuncTimerA(void *param)
{
	vTaskNotifyGiveFromISR(leer_senial_analogica_handle, pdFALSE);
}

void FuncTimerB(void *param)
{
	vTaskNotifyGiveFromISR(leer_ecg_handle, pdFALSE);
}

uint16_t valor_leido;
void leer_senial_analogica()
{
	while (true)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		AnalogInputReadSingle(CH1, &valor_leido);
		UartSendString(UART_PC, ">valor:");
		UartSendString(UART_PC, (char *)UartItoa(valor_leido, 10));
		UartSendString(UART_PC, "\r\n");
	}
}


void leer_ecg()
{
	uint16_t i = 0;
	uint8_t ecg = 0;
	while (true)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		ecg = ECG[i];
		AnalogOutputWrite(ecg);
		i++;
		if (i == 256)//sizeof(ECG))
			i = 0;
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
	AnalogOutputInit();

	timer_config_t timer_1 = {
		.timer = TIMER_A,
		.period = 20000,
		.func_p = FuncTimerA,
		.param_p = NULL};

	TimerInit(&timer_1);

	timer_config_t timer_2 = {
		.timer = TIMER_B,
		.period = 25000,
		.func_p = FuncTimerB,
		.param_p = NULL};

	TimerInit(&timer_2);

	serial_config_t my_uart = {
		.baud_rate = 115200,
		.port = UART_PC,
		.func_p = NULL,
		.param_p = NULL,
	};
	UartInit(&my_uart);
	xTaskCreate(&leer_senial_analogica, "Leer", 2048, NULL, 5, &leer_senial_analogica_handle);
	xTaskCreate(&leer_ecg, "Mandar", 2048, NULL, 5, &leer_ecg_handle);
	TimerStart(timer_1.timer);
	TimerStart(timer_2.timer);
}
/*==================[end of file]============================================*/