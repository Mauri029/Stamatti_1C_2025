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
#define CONFIG_BLINK_PERIOD_sensor_radiacion 5000

bool estado_sensores_y_notificacion = true; // Variable para controlar el estado de los sensores y la notificación
float humedad = 0, temperatura = 0;
uint16_t radicacion_leida=0;

/*==================[internal data definition]===============================*/
TaskHandle_t medir_presion_y_temperatura_task_handle = NULL;
TaskHandle_t medir_radiacion_task_handle = NULL;

/*==================[internal functions declaration]=========================*/
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
				LedOn(LED_1);  // led verde
				LedOff(LED_3); // led rojo
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
uint16_t leer_entrada_analogica()
{
	AnalogInputReadSingle(CH1, &radicacion_leida);
	radicacion_leida = (radicacion_leida /4095)*100; // lo convierte a mR/h, siendo 100 el maximo
	return radicacion_leida;

}

static void medir_radicacion_task()
{
	while (true)
	{
		if (estado_sensores_y_notificacion)
		{
			if(leer_entrada_analogica()>40)
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
				LedOff(LED_2); // led amarillo
				LedOn(LED_1);  // led verde
				UartSendString(UART_PC, ">Radiacion: ");
				UartSendString(UART_PC, (char *)UartItoa(radicacion_leida, 10));
				UartSendString(UART_PC, " mR/h ");
				UartSendString(UART_PC, "\r\n");
			}

		}
		vTaskDelay(CONFIG_BLINK_PERIOD_sensor_presion_temperatura / portTICK_PERIOD_MS);
	}
}

void radiacion_elevada()
{
	LedOn(LED_2); // led amarillo
}

void apagar_sensores_y_notificacion()
{
	estado_sensores_y_notificacion = false;
}

void prender_sensores_y_notificacion()
{
	estado_sensores_y_notificacion = true;
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
	xTaskCreate(&medir_presion_y_temperatura_task, "Medir", 2048, NULL, 5, &medir_presion_y_temperatura_task_handle);
}
/*==================[end of file]============================================*/