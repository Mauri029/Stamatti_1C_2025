/*! @mainpage Actividad 1 del proyecto 2
 *
 * @section genDesc General Description
 *
 *El programa mide la distancia , prende los led segun la distancia que mida, muestra la distancia en una
 * pantalla lcd y prende o apaga la medicion y mantiene el valor en pantalla
 *
 *
 * @section hardConn Hardware Connection
 * \brief Driver for using the 3 digits numeric display in ESP-EDU.
 *
 * |   Display      |   EDU-CIAA	|
 * |:--------------:|:-------------:|
 * | 	Vcc 	    |	5V      	|
 * | 	BCD1		| 	GPIO_20		|
 * | 	BCD2	 	| 	GPIO_21		|
 * | 	BCD3	 	| 	GPIO_22		|
 * | 	BCD4	 	| 	GPIO_23		|
 * | 	SEL1	 	| 	GPIO_19		|
 * | 	SEL2	 	| 	GPIO_18		|
 * | 	SEL3	 	| 	GPIO_9		|
 * | 	Gnd 	    | 	GND     	|
 *
 * \brief Driver for reading distance with HC-SR04 module.
 * |   HC_SR04      |   EDU-CIAA	|
 * |:--------------:|:-------------:|
 * | 	Vcc 	    |	5V      	|
 * | 	Echo		| 	GPIO_3		|
 * | 	Trig	 	| 	GPIO_2		|
 * | 	Gnd 	    | 	GND     	|
 *
 *
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
#include "lcditse0803.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "hc_sr04.h"
#include "lcditse0803.h"
#include "switch.h"

/*==================[macros and definitions]=================================*/
bool on_off_pantalla = true;
bool hold = false;
uint16_t distancia;
uint8_t teclas;

/*==================[internal functions definition]==========================*/

/**
 * @fn static void Medir_task()
 * @brief Tarea encargada de medir la distancia con el sensor HC-SR04.
 * @note Enciende LEDs según la distancia y muestra el valor en el display si "hold" no está activado.
 * Si el sistema está apagado, se apagan todos los LEDs y se apaga el display.
 * @param None
 * @return None
 */
static void Medir_task()
{
    while (true)
    {

        if (on_off_pantalla)
        {
            distancia = HcSr04ReadDistanceInCentimeters();

            if (distancia <= 10)
            {
                LedOff(LED_1);
                LedOff(LED_2);
                LedOff(LED_3);
            }
            else if (distancia > 10 && distancia <= 20)
            {
                LedOn(LED_1);
                LedOff(LED_2);
                LedOff(LED_3);
            }
            else if (distancia > 20 && distancia <= 30)
            {
                LedOn(LED_1);
                LedOn(LED_2);
                LedOff(LED_3);
            }
            else
            {
                LedOn(LED_1);
                LedOn(LED_2);
                LedOn(LED_3);
            }
            if (!hold)
            {
                LcdItsE0803Write(distancia);
                
            }

           
        } else
        {
            LcdItsE0803Off();
            LedsOffAll();
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
/**
 * @fn static void Teclas_task()
 * @brief Tarea encargada de leer las teclas físicas para controlar el modo de operación.
 * @note SWITCH_1 alterna entre encender/apagar pantalla y medición. SWITCH_2 activa o desactiva el "hold".
 * @param None
 * @return None
 */
static void Teclas_task()
{
    while (true)
    {
        teclas = SwitchesRead();
        switch (teclas)
        {
        case SWITCH_1:
            on_off_pantalla = !on_off_pantalla;
            break;
        case SWITCH_2:
            hold = !hold;

            break;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void app_main(void)
{
    HcSr04Init(GPIO_3, GPIO_2);
    LcdItsE0803Init();
    LedsInit();
    SwitchesInit();
    xTaskCreate(&Medir_task, "Medir", 2048, NULL, 1, NULL);
    xTaskCreate(Teclas_task, "Teclas", 2048, NULL, 1, NULL);
}
/*==================[end of file]============================================*/