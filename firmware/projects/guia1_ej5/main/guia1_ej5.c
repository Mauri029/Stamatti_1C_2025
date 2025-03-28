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
 * @author Mauricio Stamatti
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <gpio_mcu.h>

typedef struct
{
	gpio_t pin; /*!< GPIO pin number */
	io_t dir;	/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

int8_t convertToBcdArray(uint32_t data, uint8_t digits, uint8_t *bcd_number)
{
	for (int i = 0; i < digits; i++)
	{
		bcd_number[digits - i - 1] = (data % 10);
		data /= 10;
	}
	return 0;
}

void config_gpio(gpioConf_t *gpio, uint8_t bcd_number)
{
	for (int i = 0; i < 4; i++)
	{
		if ((1 << i) & bcd_number)
		{
			GPIOOn(gpio[i].pin);
		}
		else
		{
			GPIOOff(gpio[i].pin);
		}
	}
}

void Grafica_bcd(uint32_t datos, uint8_t digitos_salida, gpioConf_t *gpio_1, gpioConf_t *gpio_2)
{
	uint8_t numero_por_partes[3] = {0};
	convertToBcdArray(datos, digitos_salida, &numero_por_partes[0]);

	for (int i = 0; i < 3; i++)
	{
		config_gpio(gpio_2, numero_por_partes[i]);
		GPIOOn(gpio_1[i].pin);
		GPIOOff(gpio_1[i].pin);
	}
}

void app_main(void)
{

	uint8_t bcd_numer[5] = {0};
	gpioConf_t gpio[4] =
		{
			{GPIO_20, GPIO_OUTPUT},
			{GPIO_21, GPIO_OUTPUT},
			{GPIO_22, GPIO_OUTPUT},
			{GPIO_23, GPIO_OUTPUT}};

	for (int i = 0; i < 4; i++)
	{
		GPIOInit(gpio[i].pin, gpio[i].dir);
	}

	gpioConf_t gpio_pantalla[3] =
		{
			{GPIO_19, GPIO_OUTPUT},
			{GPIO_18, GPIO_OUTPUT},
			{GPIO_9, GPIO_OUTPUT}};

	for (int i = 0; i < 3; i++)
	{
		GPIOInit(gpio_pantalla[i].pin, gpio_pantalla[i].dir);
	}

	// convertToBcdArray(123, 3, &bcd_numer[0]);
	Grafica_bcd(69, 3, &gpio_pantalla[0], &gpio[0]);

	printf("BCD: %d %d %d\n", bcd_numer[0], bcd_numer[1], bcd_numer[2]);
}