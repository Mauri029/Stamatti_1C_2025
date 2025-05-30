/**
 * @file receptor _laser.c
 * @author Mauricio Stamatti
 * @brief
 * @version 1.0
 * @date 2025-05-30
 *
 * @copyright Copyright (c) 2023
 *
 */

/*==================[inclusions]=============================================*/
#include "receptor_laser.h"
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/
gpio_t Laser_GPIO
    /*==================[internal data declaration]==============================*/

    /*==================[internal functions declaration]=========================*/

    /*==================[internal data definition]===============================*/

    /*==================[external data definition]===============================*/

    /*==================[internal functions definition]==========================*/

    /*==================[external functions definition]==========================*/
int8_t Laser_receiverPirInit(gpio_t GPIO)
{
    /* GPIO configurations */
    Laser_GPIO = GPIO;
    GPIOInit(Laser_GPIO, GPIO_INPUT);
    GPIOInputFilter(Laser_GPIO);
    return true;
}

bool Laser_receiverRead(void)
{
    return GPIORead (Laser_GPIO);
}

void Laser_receiverActivInt(void *ptr_int_func, void *args)
{
    GPIOActivInt(Laser_GPIO, ptr_int_func, false, args);
}

/*==================[end of file]============================================*/
