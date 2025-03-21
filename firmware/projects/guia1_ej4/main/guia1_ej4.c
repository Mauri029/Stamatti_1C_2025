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
 * | 21/03/2025 | Document creation		                         |
 *
 * @author Mauricio Stamatti
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <array>
#include <cstdint>
using namespace std;

array <uint8_t, 31> bcd_number;
int8_t  convertToBcdArray(uint32_t data, uint8_t digits, uint8_t * bcd_number)
{
	
}


void app_main(void){
	
}