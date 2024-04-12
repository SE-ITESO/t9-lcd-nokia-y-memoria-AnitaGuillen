/** @file PIT_WITH_SDK.h
*
* @brief PIT module driver for the K64 Sub-Family of MCUs
* that uses the SDK's functions and adds functionality for
* easy get-set and callback handling of the pit's interrupts.
*
* @par
* Patricio Coz Puig. Mar, 28, 2024
*/

#ifndef PIT_WITH_SDK_H_
#define PIT_WITH_SDK_H_

#include <stdint.h>
#include <stdbool.h>
#include <fsl_pit.h>

typedef struct
{
	uint8_t flag_timer_0 : 1;
	uint8_t flag_timer_1 : 1;
	uint8_t flag_timer_2 : 1;
	uint8_t flag_timer_3 : 1;
}pit_interrupt_flags_t;

void pit_init_module(void);
void pit_timer_init_callback (pit_chnl_t pitChnl, void (*handler)(void));
void pit_clear_irq_flag (pit_chnl_t pitChnl);
uint8_t pit_get_irq_flag (pit_chnl_t pitChnl);

#endif /* PIT_WITH_SDK_H_ */
