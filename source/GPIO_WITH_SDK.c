/** @file GPIO_WITH_SDK.c
*
* @brief GPIO module driver for the K64 Sub-Family of MCUs
* that uses the SDK's functions and adds functionality for
* easy get-set and callback handling of the pit's interrupts.
*
* @par
* Patricio Coz Puig. Mar, 26, 2024
*/

#include "GPIO_WITH_SDK.h"

static void (*gpio_A_callback)(void) = 0;
static void (*gpio_B_callback)(void) = 0;
static void (*gpio_C_callback)(void) = 0;
static void (*gpio_D_callback)(void) = 0;
static void (*gpio_E_callback)(void) = 0;

volatile static gpio_interrupt_flags_t gpio_interrupt_flags = {0};

void
PORTA_IRQHandler (void)
{

	gpio_interrupt_flags.flag_port_any = true;
	gpio_interrupt_flags.flags[PORT_A_NUM].flag_port = true;
	gpio_interrupt_flags.flags[PORT_A_NUM].pins |= PORT_GetPinsInterruptFlags(PORTA);

	if(gpio_A_callback)
	{
		gpio_A_callback();
	}

	PORT_ClearPinsInterruptFlags(PORTA, 0xFFFFFFFF);
}

void
PORTB_IRQHandler (void)
{
	gpio_interrupt_flags.flag_port_any = true;
	gpio_interrupt_flags.flags[PORT_B_NUM].flag_port = true;
	gpio_interrupt_flags.flags[PORT_B_NUM].pins |= PORT_GetPinsInterruptFlags(PORTB);
	if(gpio_B_callback)
	{
		gpio_B_callback();
	}
	PORT_ClearPinsInterruptFlags(PORTB, 0xFFFFFFFF);
}

void
PORTC_IRQHandler (void)
{
	gpio_interrupt_flags.flag_port_any = true;
	gpio_interrupt_flags.flags[PORT_C_NUM].flag_port = true;
	gpio_interrupt_flags.flags[PORT_C_NUM].pins |= PORT_GetPinsInterruptFlags(PORTC);
	if(gpio_C_callback)
	{
		gpio_C_callback();
	}
	PORT_ClearPinsInterruptFlags(PORTC, 0xFFFFFFFF);
}

void
PORTD_IRQHandler (void)
{
	gpio_interrupt_flags.flag_port_any = true;
	gpio_interrupt_flags.flags[PORT_D_NUM].flag_port = true;
	gpio_interrupt_flags.flags[PORT_D_NUM].pins |= PORT_GetPinsInterruptFlags(PORTD);
	if(gpio_D_callback)
	{
		gpio_D_callback();
	}
	PORT_ClearPinsInterruptFlags(PORTD, 0xFFFFFFFF);
}

void
PORTE_IRQHandler (void)
{
	gpio_interrupt_flags.flag_port_any = true;
	gpio_interrupt_flags.flags[PORT_E_NUM].flag_port = true;
	gpio_interrupt_flags.flags[PORT_E_NUM].pins |= PORT_GetPinsInterruptFlags(PORTE);
	if(gpio_E_callback)
	{
		gpio_E_callback();
	}
	PORT_ClearPinsInterruptFlags(PORTE, 0xFFFFFFFF);
}

/*!
* @brief Assigns the given handler to the given port's callback function.
* @param[in] portNum  The port.
* @param[in] handler  Pointer to handler function.
*/
void
gpio_init_callback (port_num_t portNum, void (*handler)(void))
{
	switch(portNum)
	{
	case PORT_A_NUM:
		gpio_A_callback = handler;
		break;
	case PORT_B_NUM:
		gpio_B_callback = handler;
			break;
	case PORT_C_NUM:
		gpio_C_callback = handler;
			break;
	case PORT_D_NUM:
		gpio_D_callback = handler;
			break;
	case PORT_E_NUM:
		gpio_E_callback = handler;
			break;
	default:
			break;
	}
}

/*!
* @brief Clears the given port's software defined interrupt flag.
* @param[in] portNum  The number of the port.
*/
void
gpio_clear_irq_port_flag (port_num_t portNum)
{
	if(PORT_ANY_NUM == portNum)
	{
		gpio_interrupt_flags.flag_port_any = false;
	}
	else
	{
		gpio_interrupt_flags.flags[portNum].flag_port = false;
	}
}

/*!
* @brief Clears the given port pin's software defined interrupt flag.
* @param[in] portNum  The number of the port.
* @param[in] pin      The number of the pin.
*/
void
gpio_clear_irq_pin_flag (port_num_t portNum, uint8_t pin)
{
	gpio_interrupt_flags.flags[portNum].pins &= (~(1 << pin));
}

/*!
* @brief Gets the given port's software defined interrupt flag.
* @param[in] portNum  The number of the port.
*/
uint8_t
gpio_get_irq_port_flag (port_num_t portNum)
{
	uint8_t status = 0;
	if(PORT_ANY_NUM == portNum)
	{
		status = gpio_interrupt_flags.flag_port_any;
	}
	else
	{
		status =  gpio_interrupt_flags.flags[portNum].flag_port;
	}
	return status;
}

/*!
* @brief Gets the given port pin software defined interrupt flags.
* @param[in] portNum  The number of the port.
*/
uint32_t
gpio_get_irq_pin_flags(port_num_t portNum)
{
	uint32_t status = 0;
	if(PORT_ANY_NUM == portNum)
	{
		status = gpio_interrupt_flags.flag_port_any;
	}
	else
	{
		status =  gpio_interrupt_flags.flags[portNum].pins;
	}
	return status;
}
