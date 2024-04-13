/** Assignment 9: Nokia.
 *
 * Code that displays a different image each 3 seconds.
 *
 * There are 6 available images
 *
 * Ana Isabel Guillen Gonzalez 	739422
 * Patricio Coz Puig			739393
 *
 * 04/05/2024, Electronic Engineering ITESO.
 */

#include <stdint.h>
#include <stdbool.h>
#include "NVIC.h"
#include "PIT_WITH_SDK.h"
#include "GPIO_WITH_SDK.h"
#include "S25FL164K.h"

#define NOKIA_IMAGE_SIZE 504U

#define IMAGEN1		0x40000U
#define IMAGEN2		0x41000U
#define IMAGEN3		0x42000U
#define IMAGEN4		0x43000U
#define IMAGEN5		0x44000U




uint8_t g_three_scnds = 0;

void flag_three_scnds()
{
	g_three_scnds = 1;
}

void NVIC_init(void)
{
	/**Sets the threshold for interrupts, if the interrupt has higher priority constant
	 *  that the BASEPRI, the interrupt will not be attended **/
	NVIC_set_basepri_threshold(PRIORITY_6);

	/**Enables and sets the priority for the PIT timers interrupts**/
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_1);
}

int main(void)
{

	CLOCK_SetSimSafeDivs();
	/**Initialize the PIT timers needed**/
	pit_init_module();
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, 31500000);

	pit_timer_init_callback(kPIT_Chnl_0, flag_three_scnds);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);


	NVIC_init();
	NVIC_global_enable_interrupts;

	gpio_pin_config_t output_config = {
			kGPIO_DigitalOutput,
			0,
	};

	uint32_t address = IMAGEN1;
	uint8_t readData[NOKIA_IMAGE_SIZE] = {0};
	s25fl_init();
	s25fl_read_data(address, readData, NOKIA_IMAGE_SIZE);

	CLOCK_EnableClock(kCLOCK_PortD);
	PORT_SetPinMux(PORTD, 0, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, 0, &output_config);
	g_three_scnds = 0;
	PIT_StartTimer (PIT, kPIT_Chnl_0);
	uint8_t image = 0;
	for(;;)
	{

		if (1 == g_three_scnds)
		{
			GPIO_PortToggle(GPIOD, 1);
			image++;
			g_three_scnds = 0;
		}
		switch(image)
		{
		case 0:
			//imagen iteso
			break;
		case 1:
			//Imagen 2
			break;
		case 2:
			//Imagen 3
			break;
		case 3:
			//Imagen 4
			break;
		case 4:
			//Imagen 5
			break;
		case 5:
			//Imagen 6
			break;
		default:
			image = 0;
			break;
		}

	}
	return 0;
}
