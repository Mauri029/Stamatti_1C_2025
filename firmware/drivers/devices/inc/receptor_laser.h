#ifndef RECEPTOR-LASER_H
#define RECEPTOR-LASER_H
/** \addtogroup Drivers_Programable Drivers Programable
 ** @{ */
/** \addtogroup Drivers_Devices Drivers devices
 ** @{ */
/** \addtogroup Laser receiver
 ** @{ */

/** \brief Laser receiver driver for the ESP-EDU Board.
 * 
 * @note ESP-EDU have a Laser receiver connected to GPIO. 
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
/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/**
 * @brief ESP-EDU Laser receiver initialization
 * 
 * @return int8_t 
 */
int8_t Laser_receiverPirInit(gpio_t GPIO);

/**
 * @brief Read Laser receiver state.
 * 
 * @return bool false if the laser barrier is not interrupted , true if if the laser barrier is interrupted
 */
bool Laser_receiverRead(void);

/**
 * @brief Enables the interruption and assigns a callback function.
 * 
 * @param ptrIntFunc Pointer to callback function
 * @param args Pointer to callback function parameters
 */
void Laser_receiverActivInt(void *ptr_int_func, void *args);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif

/*==================[end of file]============================================*/
