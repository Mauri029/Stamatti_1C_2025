/*! @mainpage Blinking switch
 *
 * @section genDesc General Description
 *
 * Este ejemplo hace parpadear LED_1 o LED_2 cuando se presiona SWITCH_1 o SWITCH_2.  
 * También se puede configurar un LED específico para que parpadee, se encienda o se apague según una estructura de control.
 *
 * @section changelog Changelog
 *
 * |   Date     | Description          |
 * |:----------:|:---------------------|
 * | 12/09/2023 | Document creation    |
 *
 * @author Mauricio Stamatti
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"

/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 1000
#define ON 0
#define OFF 1
#define TOGGLE 2

/**
 * @struct leds
 * @brief Estructura que define la configuración de un LED.
 */
struct leds {
    uint8_t mode;       /*!< Modo de operación: ON, OFF o TOGGLE */
    uint8_t n_led;      /*!< Número de LED a controlar (1, 2 o 3) */
    uint8_t n_ciclos;   /*!< Cantidad de ciclos de parpadeo (solo si mode == TOGGLE) */
    uint16_t periodo;   /*!< Período entre cambios de estado en milisegundos */
};

/*==================[internal functions definition]==========================*/

/**
 * @fn void Funcion_prueba(struct leds *led)
 * @brief Controla el encendido, apagado o parpadeo de un LED según los parámetros recibidos.
 * @param led Puntero a estructura con la configuración del LED.
 * @return None
 */
void Funcion_prueba(struct leds *led)
{
    switch (led->mode)
    {
    case ON:
        switch (led->n_led)
        {
        case 1: LedOn(LED_1); break;
        case 2: LedOn(LED_2); break;
        default: LedOn(LED_3); break;
        }
        break;

    case OFF:
        switch (led->n_led)
        {
        case 1: LedOff(LED_1); break;
        case 2: LedOff(LED_2); break;
        default: LedOff(LED_3); break;
        }
        break;

    case TOGGLE:
        for (int i = 0; i < led->n_ciclos * 2; i++)
        {
            switch (led->n_led)
            {
            case 1: LedToggle(LED_1); break;
            case 2: LedToggle(LED_2); break;
            default: LedToggle(LED_3); break;
            }

            for (size_t j = 0; j < 10; j++)
            {
                vTaskDelay(led->periodo / portTICK_PERIOD_MS);
            }
        }
        break;

    default:
        break;
    }
}

/*==================[external functions definition]==========================*/


void app_main(void)
{
    LedsInit();
    SwitchesInit();

    struct leds my_leds;
    my_leds.mode = TOGGLE;
    my_leds.n_led = 3;
    my_leds.n_ciclos = 15;
    my_leds.periodo = 250;

    Funcion_prueba(&my_leds);
}

/*==================[end of file]============================================*/
