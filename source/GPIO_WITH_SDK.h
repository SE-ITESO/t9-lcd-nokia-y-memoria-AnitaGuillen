/** @file GPIO_WITH_SDK.h
*
* @brief GPIO module driver for the K64 Sub-Family of MCUs
* that uses the SDK's functions and adds functionality for
* easy get-set and callback handling of the pit's interrupts.
*
* @par
* Patricio Coz Puig. Mar, 26, 2024
*/

#ifndef GPIO_WITH_SDK_H_
#define GPIO_WITH_SDK_H_

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

#define NUM_OF_PORTS 5

typedef struct
{
	uint8_t  flag_port;
	uint32_t pins;
} port_interrupt_flags_t;

typedef struct
{
	port_interrupt_flags_t flags[NUM_OF_PORTS];
	uint8_t flag_port_any;
} gpio_interrupt_flags_t;

typedef enum
{
	PORT_A_NUM,
	PORT_B_NUM,
	PORT_C_NUM,
	PORT_D_NUM,
	PORT_E_NUM,
	PORT_ANY_NUM,
}port_num_t;

typedef struct
{
	clock_ip_name_t portClock;
	PORT_Type * portPort;
	GPIO_Type * gpioPort;
	port_num_t portNum;
	uint32_t pin;
} gpio_pin_t;


void gpio_init_callback (port_num_t portNum, void (*handler)(void));
void gpio_clear_irq_port_flag (port_num_t portNum);
void gpio_clear_irq_pin_flag (port_num_t portNum, uint8_t pin);
uint8_t gpio_get_irq_port_flag (port_num_t portNum);
uint32_t gpio_get_irq_pin_flags(port_num_t portNum);

#endif /* GPIO_WITH_SDK_H_ */
