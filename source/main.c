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

#define IMAGE_1_ADDR		0x40000U
#define IMAGE_2_ADDR		0x41000U
#define IMAGE_3_ADDR		0x42000U
#define IMAGE_4_ADDR		0x43000U
#define IMAGE_5_ADDR		0x44000U

#define IMAGES_QUANTITY    6

#define PIT_VALUE_3_SECONDS 31500000

#include "LCD_nokia.h"
#include "LCD_nokia_images.h"
#include "SPI.h"

/*! This array hold the initial picture that is shown in the LCD. Note that extern should be avoided*/
extern const uint8_t ITESO[NOKIA_IMAGE_SIZE];

uint8_t IMAGE_1[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_2[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_3[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_4[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_5[NOKIA_IMAGE_SIZE] = {0};

static uint8_t g_three_scnds = 0;

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
	uint8_t image = 0;
	uint8_t index;
	uint32_t imageAddresses[IMAGES_QUANTITY - 1] =
	{
			IMAGE_1_ADDR,
			IMAGE_2_ADDR,
			IMAGE_3_ADDR,
			IMAGE_4_ADDR,
			IMAGE_5_ADDR
	};

	uint8_t * images[IMAGES_QUANTITY - 1] =
	{
			IMAGE_1,
			IMAGE_2,
			IMAGE_3,
			IMAGE_4,
			IMAGE_5
	};

	CLOCK_SetSimSafeDivs();
	/**Initialize the PIT timers needed**/
	pit_init_module();
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, PIT_VALUE_3_SECONDS);

	pit_timer_init_callback(kPIT_Chnl_0, flag_three_scnds);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);


	NVIC_init();
	NVIC_global_enable_interrupts;

	s25fl_init();
	PIT_StartTimer (PIT, kPIT_Chnl_0);

	for(index = 0; index < IMAGES_QUANTITY - 1; index++)
	{
		s25fl_read_data(imageAddresses[index], images[index], NOKIA_IMAGE_SIZE);
	}

	SPI_config();
	LCD_nokia_init();
	LCD_nokia_clear();
	for(;;)
	{
		if (1 == g_three_scnds)
		{
			g_three_scnds = 0;
			if(image >= IMAGES_QUANTITY)
			{
				image = 0;
			}
			LCD_nokia_clear();/*! It clears the information printed in the LCD*/
			switch(image)
			{
			case 0:
				//Imagen iteso
				LCD_nokia_bitmap(ITESO); /*! It prints an array that hold an image, in this case is the initial picture*/
				break;
			case 1:
				//Imagen 2
				LCD_nokia_bitmap(IMAGE_1); /*! It prints an array that hold an image, in this case is the initial picture*/
				break;
			case 2:
				//Imagen 3
				LCD_nokia_bitmap(IMAGE_2); /*! It prints an array that hold an image, in this case is the initial picture*/
				break;
			case 3:
				//Imagen 4
				LCD_nokia_bitmap(IMAGE_3); /*! It prints an array that hold an image, in this case is the initial picture*/
				break;
			case 4:
				//Imagen 5
				LCD_nokia_bitmap(IMAGE_4); /*! It prints an array that hold an image, in this case is the initial picture*/
				break;
			case 5:
				//Imagen 6
				LCD_nokia_bitmap(IMAGE_5); /*! It prints an array that hold an image, in this case is the initial picture*/
				break;
			default:
				image = 0;
				break;
			}
			image++;
		}
	}
	return 0;
}
