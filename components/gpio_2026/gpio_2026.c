/**
 * @file gpio_2026.c
 * @brief Low-Level GPIO Driver for ESP32 source file.
 *
 * @details
 * This file contains function implementations
 * for direct GPIO control by ESP32 registers.
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-01
 *
 * @note Based on ESP32 Technical Reference Manual v4.6. 
 */
 
#include <stdio.h>
#include "gpio_2026.h"


/**
 * @brief Array for mapping GPIO with his IO_MUX_GPIOx register
 *
 * @details
 * It let you to access to the register just knowing the GPIO
 *
 * @note designated initializers technique
 */
volatile uint32_t *LL_IO_MUX_x_REG[] = {
	[LL_GPIO0]  = LL_IO_MUX_GPIO0_REG,
	[LL_GPIO1]  = LL_IO_MUX_GPIO1_REG,
	[LL_GPIO2]  = LL_IO_MUX_GPIO2_REG,
	[LL_GPIO3]  = LL_IO_MUX_GPIO3_REG,
	[LL_GPIO4]  = LL_IO_MUX_GPIO4_REG,
	[LL_GPIO5]  = LL_IO_MUX_GPIO5_REG,
	[LL_GPIO12] = LL_IO_MUX_GPIO12_REG,
	[LL_GPIO13] = LL_IO_MUX_GPIO13_REG,
	[LL_GPIO14] = LL_IO_MUX_GPIO14_REG,
	[LL_GPIO15] = LL_IO_MUX_GPIO15_REG,
	[LL_GPIO16] = LL_IO_MUX_GPIO16_REG,
	[LL_GPIO17] = LL_IO_MUX_GPIO17_REG,
	[LL_GPIO18] = LL_IO_MUX_GPIO18_REG,
	[LL_GPIO19] = LL_IO_MUX_GPIO19_REG,
	[LL_GPIO21] = LL_IO_MUX_GPIO21_REG,
	[LL_GPIO22] = LL_IO_MUX_GPIO22_REG,
	[LL_GPIO23] = LL_IO_MUX_GPIO23_REG,
	[LL_GPIO25] = LL_IO_MUX_GPIO25_REG,
	[LL_GPIO26] = LL_IO_MUX_GPIO26_REG,
	[LL_GPIO27] = LL_IO_MUX_GPIO27_REG,
	[LL_GPIO32] = LL_IO_MUX_GPIO32_REG,
	[LL_GPIO33] = LL_IO_MUX_GPIO33_REG,
	[LL_GPIO34] = LL_IO_MUX_GPIO34_REG,
	[LL_GPIO35] = LL_IO_MUX_GPIO35_REG,
	[LL_GPIO36] = LL_IO_MUX_GPIO36_REG,
	[LL_GPIO37] = LL_IO_MUX_GPIO37_REG,
	[LL_GPIO38] = LL_IO_MUX_GPIO38_REG,
	[LL_GPIO39] = LL_IO_MUX_GPIO39_REG,
};

bool ll_gpio_read(ll_gpio_t pin){
	if (pin < 32){
		return (LL_READ_BIT_GPIO_IN_REG(pin) != 0); // Force to bool 0 or 1 value
	} else {
		return (LL_READ_BIT_GPIO_IN1_REG(pin) != 0); // Force to bool 0 or 1 value
	}
}

void ll_gpio_write(ll_gpio_t pin, bool valor){
	if (valor){
		if(pin < 32){
			LL_SET_BIT_GPIO_OUT_W1TS(pin);
		} else{
			LL_SET_BIT_GPIO_OUT1_W1TS(pin);
		}
	}
	else{
		if(pin < 32){
			LL_CLEAR_BIT_GPIO_OUT_W1TC(pin);
		} else{
			LL_CLEAR_BIT_GPIO_OUT1_W1TC(pin);
		}
	}	
}

void ll_enable_output (ll_gpio_t pin, bool valor){
	if (valor){
		if(pin < 32){
			LL_SET_BIT_GPIO_ENABLE_W1TS(pin);
		} else{
			LL_SET_BIT_GPIO_ENABLE1_W1TS(pin);
		}
	}
	else{
		if(pin < 32){
			LL_CLEAR_BIT_GPIO_ENABLE_W1TC(pin);
		} else{
			LL_CLEAR_BIT_GPIO_ENABLE1_W1TC(pin);
		}
	}
}

void ll_gpio_config_in (ll_gpio_t pin, ll_input_mode_t modo_pull){
	//disaable at first to stop GPIO conducing output
	ll_enable_output(pin, false); //disable pin as output

	LL_ENABLE_PAD(LL_IO_MUX_x_REG[pin]); //Enable GPIO as input
	LL_SET_MCU_SEL_FUN2(LL_IO_MUX_x_REG[pin]); //set function 2 (GPIO mode)

	if (modo_pull == LL_PULL_UP){
		LL_ENABLE_PULLUP(LL_IO_MUX_x_REG[pin]);
	} else if (modo_pull == LL_PULL_DOWN){
		LL_ENABLE_PULLDOWN(LL_IO_MUX_x_REG[pin]);
	} else{
		LL_FLOATING_GPIO(LL_IO_MUX_x_REG[pin]);
	}
}

void ll_gpio_config_out(ll_gpio_t pin){
	LL_SET_MCU_SEL_FUN2(LL_IO_MUX_x_REG[pin]); //set function 2 (GPIO mode)
	ll_gpio_write(pin, false); //GPIO starts with low value
	ll_enable_output(pin, true); //enable pin as output
	//enable at end to start GPIO conducing
}

// ========= EXTRA =========
void ll_gpio_config2026(ll_gpio_t pin, ll_input_mode_t modo, ll_gpio_mode_t modo_gpio){
	if(modo_gpio == LL_INPUT){
		ll_gpio_config_in(pin, modo);
	}
	else{
		ll_gpio_config_out(pin);
	}
}