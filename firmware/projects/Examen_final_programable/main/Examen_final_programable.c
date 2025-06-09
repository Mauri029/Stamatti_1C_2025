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
/*==================[internal data definition]===============================*/
TaskHandle_t medir_presion_y_temeratura_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
static void medir_presion_y_temeratura_task()
{
	while (true)
	{

		float humedad = 0, temperatura = 0;
		dht11Read(&humedad, &temperatura);
		if (humedad > 85 && (temperatura > 0 && temperatura < 2))
		{
			LedOn(LED_3); // led rojo
			AnalogInputReadSingle(CH1, &temperatura);
			UartSendString(UART_PC, ">Temperatura: ");
			UartSendString(UART_PC, (char *)UartItoa(temperatura, 10));
			UartSendString(UART_PC, "°C- ");
			AnalogInputReadSingle(CH2, &humedad);
			UartSendString(UART_PC, ">Humedad: ");
			UartSendString(UART_PC, (char *)UartItoa(humedad, 10));
			UartSendString(UART_PC, "%");
			UartSendString(UART_PC, " - RIESGO DE NEVADA");
			UartSendString(UART_PC, "\r\n");
		}
		else
		{
			LedOn(LED_1); // led verde
			AnalogInputReadSingle(CH1, &temperatura);
			UartSendString(UART_PC, ">Temperatura: ");
			UartSendString(UART_PC, (char *)UartItoa(temperatura, 10));
			UartSendString(UART_PC, "°C- ");
			AnalogInputReadSingle(CH2, &humedad);
			UartSendString(UART_PC, ">Humedad: ");
			UartSendString(UART_PC, (char *)UartItoa(humedad, 10));
			UartSendString(UART_PC, "%");
			UartSendString(UART_PC, "\r\n");
		}
			vTaskDelay(CONFIG_BLINK_PERIOD_sensor_presion_temperatura / portTICK_PERIOD_MS);

		


	}
}

void medir_radicacion();
{
}

void radiacion_elevada()
{
	LedOn(LED_2); // led amarillo
}

void riesgo_nevada()
{
	LedOn(LED_3); // led rojo
}
/*==================[external functions definition]==========================*/
void app_main(void)
{

	dht11Init(GPIO_6);
	LedsInit();
	SwitchesInit();


	SwitchActivInt(SWITCH_1, On_off_pantalla, NULL);
	SwitchActivInt(SWITCH_2, Hold, NULL);
	xTaskCreate(&medir_presion_y_temeratura, "Medir", 2048, NULL, 5, &medir_presion_y_temeratura_task_handle);
}
/*==================[end of file]============================================*/