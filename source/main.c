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

/** Memory addresses in which the images are stored**/
#define IMAGE_1_ADDR		0x40000U
#define IMAGE_2_ADDR		0x41000U
#define IMAGE_3_ADDR		0x42000U
#define IMAGE_4_ADDR		0x43000U
#define IMAGE_5_ADDR		0x44000U

/** Total amount of images **/
#define IMAGES_QUANTITY    6

/** 3 seconds to shoe the image **/
#define PIT_VALUE_3_SECONDS 31500000

#include "LCD_nokia.h"
#include "LCD_nokia_images.h"
#include "SPI.h"

/*! This array hold the initial picture that is shown in the LCD.
 *  Note that extern should be avoided*/
extern const uint8_t ITESO[NOKIA_IMAGE_SIZE];

/** Arrays to store the memory images**/
uint8_t IMAGE_1[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_2[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_3[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_4[NOKIA_IMAGE_SIZE] = {0};

uint8_t IMAGE_5[NOKIA_IMAGE_SIZE] = {0};

static uint8_t g_three_scnds = 0;

/** Function to modify the flag value to show a new image **/
void flag_three_scnds()
{
	g_three_scnds = 1;
}

/** NVIC function to set interruptions**/
void NVIC_init(void)
{
	/**Sets the threshold for interrupts, if the interrupt
	 * has higher priority constant
	 *  that the BASEPRI, the interrupt will not be attended **/
	NVIC_set_basepri_threshold(PRIORITY_6);

	/**Enables and sets the priority for the PIT timers interrupts**/
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_1);
}

/*!
 * @brief Main code: controls the image flow.
 * 1. Sets an array for the image address and the image arrays
 * 2. Initialize PIT and Memory
 * 3. Stores the images from memory into the image arrays
 * 4. Initialize SPI module for display and the nokia display
 * 5. Rotates the image in display each 3 seconds
 */
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
			LCD_nokia_clear(); /*! Clears the information printed in the LCD*/
			switch(image)
			{
			case 0:
				//Imagen iteso
				LCD_nokia_bitmap(ITESO);
				break;
			case 1:
				//Imagen 2
				LCD_nokia_bitmap(IMAGE_1);
				break;
			case 2:
				//Imagen 3
				LCD_nokia_bitmap(IMAGE_2);
				break;
			case 3:
				//Imagen 4
				LCD_nokia_bitmap(IMAGE_3);
				break;
			case 4:
				//Imagen 5
				LCD_nokia_bitmap(IMAGE_4);
				break;
			case 5:
				//Imagen 6
				LCD_nokia_bitmap(IMAGE_5);
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
