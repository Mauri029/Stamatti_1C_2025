#ifndef SENSOR_PIR_H
#define SENSOR_PIR_H
/** \addtogroup Drivers_Programable Drivers Programable
 ** @{ */
/** \addtogroup Drivers_Devices Drivers devices
 ** @{ */
/** \addtogroup PIR
 ** @{ */

/** \brief PIR driver for the ESP-EDU Board.
 * 
 * @note ESP-EDU have a PIR sensor connected to GPIO. 
 *
 * @author Maurico Stamatti
 *
 * @section changelog
 *
 * |   Date	    | Description                                    						|
 * |:----------:|:----------------------------------------------------------------------|
 * | 30/05/2025 | Document creation		                         						|
 * 
 **/

/*==================[inclusions]=============================================*/
#include <stdbool.h>
#include <stdint.h>
#include "gpio_mcu.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief ESP-EDU Pir sensor initialization
 * 
 * @return int8_t 
 */
int8_t PirInit(gpio_t GPIO);

/**
 * @brief Read Pir sensor state.
 * 
 * @return bool false if no motion detected, true if motion detected.
 */
bool PirRead(void);

/**
 * @brief Enables the interruption and assigns a callback function.
 * 
 * @param ptrIntFunc Pointer to callback function
 * @param args Pointer to callback function parameters
 */
void PirActivInt(void *ptr_int_func, void *args);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif

/*==================[end of file]============================================*/
