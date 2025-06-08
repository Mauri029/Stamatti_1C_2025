/**
 * @file sensor_pir.c
 * @author Mauricio Stamatti
 * @brief
 * @version 1.0
 * @date 2025-05-30
 *
 * @copyright Copyright (c) 2023
 *
 */

/*==================[inclusions]=============================================*/
#include "sensor_pir.h"
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/
gpio_t Pir_GPIO;
    /*==================[internal data declaration]==============================*/

    /*==================[internal functions declaration]=========================*/

    /*==================[internal data definition]===============================*/

    /*==================[external data definition]===============================*/

    /*==================[internal functions definition]==========================*/

    /*==================[external functions definition]==========================*/
int8_t PirInit(gpio_t GPIO)
{
    /* GPIO configurations */
    Pir_GPIO = GPIO;
    GPIOInit(Pir_GPIO, GPIO_INPUT);
    GPIOInputFilter(Pir_GPIO);
    return true;
}

bool PirRead(void)
{
    return GPIORead (Pir_GPIO);
}

void PirActivInt(void *ptr_int_func, void *args)
{
    GPIOActivInt(Pir_GPIO, ptr_int_func, false, args);
}

/*==================[end of file]============================================*/
