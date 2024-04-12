/** @file PIT_WITH_SDK.c
*
* @brief PIT module driver for the K64 Sub-Family of MCUs
* that uses the SDK's functions and adds functionality for
* easy get-set and callback handling of the pit's interrupts.
*
* @par
* Patricio Coz Puig. Mar, 28, 2024
*/

#include "PIT_WITH_SDK.h"

static void (*pit_timer_0_callback)(void) = 0;
static void (*pit_timer_1_callback)(void) = 0;
static void (*pit_timer_2_callback)(void) = 0;
static void (*pit_timer_3_callback)(void) = 0;

volatile static pit_interrupt_flags_t pit_interrupt_flags = {0};

void
PIT0_IRQHandler(void)
{
	pit_interrupt_flags.flag_timer_0 = true;
	if(pit_timer_0_callback)
	{
		pit_timer_0_callback();
	}
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
}

void
PIT1_IRQHandler(void)
{
	pit_interrupt_flags.flag_timer_1 = true;
	if(pit_timer_1_callback)
	{
		pit_timer_1_callback();
	}
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
}

void
PIT2_IRQHandler(void)
{
	pit_interrupt_flags.flag_timer_2 = true;
	if(pit_timer_2_callback)
	{
		pit_timer_2_callback();
	}
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
}

void
PIT3_IRQHandler(void)
{
	pit_interrupt_flags.flag_timer_3 = true;
	if(pit_timer_3_callback)
	{
		pit_timer_3_callback();
	}
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);
}

/*!
* @brief Assigns the given handler to the given pit-timer callback function.
* @param[in] pitChnl      The timer.
* @param[in] handler      Pointer to handler function.
*/
void
pit_timer_init_callback (pit_chnl_t pitChnl, void (*handler)(void))
{
	switch (pitChnl)
	{
	case kPIT_Chnl_0:
		pit_timer_0_callback = handler;
		break;
	case kPIT_Chnl_1:
		pit_timer_1_callback = handler;
		break;
	case kPIT_Chnl_2:
		pit_timer_2_callback = handler;
		break;
	case kPIT_Chnl_3:
		pit_timer_3_callback = handler;
		break;
	default:
		break;
	}
}

/*!
* @brief Initializes the PIT module.
* WARNING: use this function once, because it clears
* the interrupt enable flags in all pit channels.
*/
void pit_init_module(void)
{
	pit_config_t pitConfig;

	PIT_GetDefaultConfig(&pitConfig);

	pitConfig.enableRunInDebug = true;

	/* Init pit module */
	PIT_Init(PIT, &pitConfig);
}


/*!
* @brief Clears the given pit timer's software defined interrupt flag.
* @param[in] pitChnl  The number of the pit timer.
*/
void
pit_clear_irq_flag (pit_chnl_t pitChnl)
{
	switch(pitChnl)
	{
	case kPIT_Chnl_0:
		pit_interrupt_flags.flag_timer_0 = false;
		break;
	case kPIT_Chnl_1:
		pit_interrupt_flags.flag_timer_1 = false;
		break;
	case kPIT_Chnl_2:
		pit_interrupt_flags.flag_timer_2 = false;
		break;
	case kPIT_Chnl_3:
		pit_interrupt_flags.flag_timer_3 = false;
		break;
	default:
		break;
	}
}

/*!
* @brief Gets the given pit timer's software defined interrupt flag.
* @param[in] pitChnl The number of the pit timer.
* @return    The pit timer's software defined flag.
*/
uint8_t
pit_get_irq_flag (pit_chnl_t pitChnl)
{
	uint8_t status;
	switch(pitChnl)
	{
	case kPIT_Chnl_0:
		status = pit_interrupt_flags.flag_timer_0;
		break;
	case kPIT_Chnl_1:
		status = pit_interrupt_flags.flag_timer_1;
		break;
	case kPIT_Chnl_2:
		status = pit_interrupt_flags.flag_timer_2;
		break;
	case kPIT_Chnl_3:
		status = pit_interrupt_flags.flag_timer_3;
		break;
	default:
		return false;
		break;
	}
	return status;
}
/*** end of file ***/
