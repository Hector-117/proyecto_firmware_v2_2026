/**
 * @file timer_2026.c
 * @brief Low-Level timer Driver for ESP32 header file.
 *
 * @details
 * This file contains register definitions and low-level macro functions
 * for direct timer control by ESP32 registers.
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-03
 *
 * @note Based on:
 * ESP32 Technical Reference Manual v4.6.
 * esp32_technical_reference_manual_v5.5.
 */

#include <stdbool.h>

#define LLT_HWREG32(x)        (*((volatile uint32_t *)(x)))
#define LLT_HW_PTR_REG32(x)   ((volatile uint32_t *)(x))

#define LL_ADVANCED_PERIPHERAL_BUS_CLK_FREQ 80
/**
 * @brief advanced pheripheral bus clock frequency
 * 
 * @details
 * APB (ADVANCED_PERIPHERAL_BUS)
 * Designed for low-bandwidth, low-power peripherals.
 * Simple interface with minimal control signals.
 * Typically used for UARTs, timers, GPIOs, and other simple I/O blocks.
 * 
 * @note https://www.linkedin.com/posts/kshitij-vaze-dubistweltmeister05_the-arm-bus-interface-is-a-thing-of-beauty-activity-7370645131835822080-3EjC/
 */


typedef enum{
	LL_TIMG0_T0 = 0,
	LL_TIMG0_T1 = 1,
	LL_TIMG1_T0 = 2,
	LL_TIMG1_T1 = 3,
} ll_timer_t;

typedef enum{
	LL_UP,
	LL_DOWN
} ll_count_mode_t;

typedef enum{
	LL_LEVEL,
	LL_EDGE
} ll_int_mode_t;

//#define LL_TIMG0_T0CONFIG_REG (LLT_HWREG32(0x3FF5F000))
#define LL_TIMG0_T0CONFIG_REG   LLT_HW_PTR_REG32(0x3FF5F000)
#define LL_TIMG1_T0CONFIG_REG   LLT_HW_PTR_REG32(0x3FF60000)
#define LL_TIMG0_T1CONFIG_REG   LLT_HW_PTR_REG32(0x3FF5F024)
#define LL_TIMG1_T1CONFIG_REG   LLT_HW_PTR_REG32(0x3FF60024)

/**
  * @brief  Timer 0 configuration register (0x3FF5F000)
  *
  * @details
  * 					[31   30   29   28...13   12   11   10]
  * 					[0     1    1   0x0001    0     0    0]
  *
  * TIMGn_TxCONFIG_REG bit fields
  * [31] 	TIMGn_Tx_EN: 	 	    When set, the timer x time-base counter is enabled. (R/W)
  * [30] 	TIMGn_Tx_INCREASE:      When set, the timer x time-base counter will increment every clock tick.
  *							        When cleared, the timer x time-base counter will decrement. (R/W)
  * [29] 	TIMGn_Tx_AUTORELOAD:    When set, timer x auto-reload at alarm is enabled. (R/W)
  * [28-13] TIMGn_Tx_DIVIDER:       Timer x clock (Tx_clk) prescale value. (R/W)
  * [12] 	TIMGn_Tx_EDGE_INT_EN:   When set, an alarm will generate an edge type interrupt. (R/W)
  * [11] 	TIMGn_Tx_LEVEL_INT_EN:  When set, an alarm will generate a level type interrupt. (R/W)
  * [10] 	TIMGn_Tx_ALARM_EN: 		When set, the alarm is enabled. This bit is automatically cleared once an alarm occurs. (R/W)
  *
  * @note 
  * Timer must be disabled before changin the prescaler divisor
  * page 497
  */
 
#define LL_TIMGn_Tx_EN 		      (1<<31)
#define LL_TIMGn_Tx_INCREASE      (1<<30)
#define LL_TIMGn_Tx_AUTORELOAD    (1<<29)
#define LL_CLEAR_TIMGn_Tx_DIVIDER (13)
#define LL_TIMGn_Tx_EDGE_INT_EN   (1<<12)
#define LL_TIMGn_Tx_LEVEL_INT_EN  (1<<11)
#define LL_TIMGn_Tx_ALARM_EN	  (1<<10)

#define LL_ENABLE_COUNTING(x)  (*(x) |= LL_TIMGn_Tx_EN)
#define LL_DISABLE_COUNTING(x) (*(x) &= ~LL_TIMGn_Tx_EN)

#define LL_UP_COUNTING(x)   (*(x) |= LL_TIMGn_Tx_INCREASE)
#define LL_DOWN_COUNTING(x) (*(x) &= ~LL_TIMGn_Tx_INCREASE)

#define LL_ENABLE_AUTORELOAD(x)   (*(x) |= LL_TIMGn_Tx_AUTORELOAD)
#define LL_DISABLE_AUTORELOAD(x)  (*(x) &= ~LL_TIMGn_Tx_AUTORELOAD)

#define LL_ENABLE_EDGE_INT(x)  (*(x) = ((*(x) & ~LL_TIMGn_Tx_LEVEL_INT_EN) | LL_TIMGn_Tx_EDGE_INT_EN))
#define LL_ENABLE_LEVEL_INT(x) (*(x) = ((*(x) & ~LL_TIMGn_Tx_EDGE_INT_EN) | LL_TIMGn_Tx_LEVEL_INT_EN))

#define LL_ENABLE_ALARM(x)  (*(x) |= LL_TIMGn_Tx_ALARM_EN)
#define LL_DISABLE_ALARM(x) (*(x) &= ~LL_TIMGn_Tx_ALARM_EN)

#define LL_CLEAR_DIVIDER(x) (*(x) &= ((~0xFFFF)<<13))
#define LL_SET_DIVIDER(x,prescaler)   (*(x) |= (prescaler << 13))


#define LL_TIMG0_T0ALARMLO_REG (LLT_HW_PTR_REG32(0x3FF5F010))
/**
 * @brief Timer x alarm trigger time-base counter value, low 32 bits. (R/W)
 *
 * @details
 *     [x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
 *								0x000000000								Reset
 * @note Page 498.
 */

#define LL_TIMG0_T0ALARMHI_REG (LLT_HW_PTR_REG32(0x3FF5F014))
/**
 * @brief Timer x alarm trigger time-base counter value, high 32 bits. (R/W)
 *
 * @details
 *     [x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
 *								0x000000000								Reset
 * @note Page 498.
 */

/**
 * a reload will cause the contents of these registers to be copied to the counter itself. 
 * A reload event can be triggered by an alarm auto-reload at alarm) or by software (software instant reload)
 * 		LL_TIMG0_T0LOADLO_REG
 * 		LL_TIMG0_T0LOADHI_REG
 * @note page 223
 */

#define LL_TIMG0_T0LOADLO_REG (LLT_HW_PTR_REG32(0x3FF5F018))
/**
 * @brief Timer 0 reload value, low 32 bits
 *
 * @details
 *     [x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
 *								0x000000000								Reset
 * @note Page 244 (esp32_technical_reference_manual_v5.5).
 */

#define LL_TIMG0_T0LOADHI_REG (LLT_HW_PTR_REG32(0x3FF5F01C))
/**
 * @brief Timer 0 reload value, high 32 bits
 *
 * @details
 *     [x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
 *								0x000000000								Reset
 * @note Page 224 (esp32_technical_reference_manual_v5.5).
 */

#define LL_CLEAR_32BIT_REG(x) (*(x) = 0)
//#define LL_SET_32BIT_REG(x,value)  (*(x) |= value)
#define LL_SET_32BIT_REG(x,value)  (*(x) = value)


#define LL_TIMG0_INT_RAW_REG (LLT_HW_PTR_REG32(0x3FF5F09C))
/**
 * @brief Raw interrupt status
 *
 * @details
 *	   [31                 (reserved)                           3 2 1 0]
 *     [x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
 *	   [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]Reset
 *
 * [0] TIMGn_INT_WDT_INT_RAW: The raw interrupt status bit for the TIMGn_INT_WDT_INT interrupt. (RO)
 * [1] TIMGn_INT_T1_INT_RAW: The raw interrupt status bit for the TIMGn_INT_T1_INT interrupt. (RO)
 * [2] TIMGn_INT_T0_INT_RAW: The raw interrupt status bit for the TIMGn_INT_T0_INT interrupt. (RO)
 * @note Page 503.
 */

#define LL_TIMGn_INT_T0_INT_RAW  (1<<0)
#define LL_TIMGn_INT_T1_INT_RAW  (1<<1)
#define LL_TIMGn_INT_WDT_INT_RAW (1<<2)

#define LL_READ_STATE_WDT_INT_RAW(x) ((*(x) & LL_TIMGn_INT_WDT_INT_RAW) != 0)
#define LL_READ_STATE_T1_INT_RAW(x)  ((*(x) & LL_TIMGn_INT_T1_INT_RAW) != 0)
#define LL_READ_STATE_T0_INT_RAW(x)  ((*(x) & LL_TIMGn_INT_T0_INT_RAW) != 0)


#define LL_TIMG0_INT_CLR_REG (LLT_HW_PTR_REG32(0x3FF5F0A4))
/**
 * @brief Interrupt clear bits
 *
 * @details
 *	   [31                 (reserved)                           3 2 1 0]
 *     [x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
 *	   [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]Reset
 *
 * [0] TIMGn_Tx_INT_WDT_INT_CLR Set this bit to clear the TIMGn_Tx_INT_WDT_INT interrupt. (WO)
 * [1] TIMGn_Tx_INT_T1_INT_CLR Set this bit to clear the TIMGn_Tx_INT_T1_INT interrupt. (WO)
 * [2] TIMGn_Tx_INT_T0_INT_CLR Set this bit to clear the TIMGn_Tx_INT_T0_INT interrupt. (WO)
 * @note Page 504.
 */
 
#define LL_TIMGn_Tx_INT_T0_INT_CLR  (1<<0)
#define LL_TIMGn_Tx_INT_T1_INT_CLR  (1<<1)
#define LL_TIMGn_Tx_INT_WDT_INT_CLR (1<<2)

#define LL_SET_BIT_WDT_INT_CLR(x) (*(x) = LL_TIMGn_Tx_INT_WDT_INT_CLR)
#define LL_SET_BIT_T1_INT_CLR(x)  (*(x) = LL_TIMGn_Tx_INT_T1_INT_CLR)
#define LL_SET_BIT_T0_INT_CLR(x)  (*(x) = LL_TIMGn_Tx_INT_T0_INT_CLR)

#define LL_TIMG0_T0LOAD_REG (LLT_HW_PTR_REG32(0x3FF5F020))
#define LL_TIMG0_T0UPDATE_REG (LLT_HW_PTR_REG32(0x3FF5F00C))
#define LL_TIMG0_T0LO_REG (LLT_HWREG32(0x3FF5F004))

void ll_timer_enable(ll_timer_t timer, bool bit_state);
void ll_timer_count_mode(ll_timer_t timer, ll_count_mode_t mode);
void ll_timer_autoreload(ll_timer_t timer, bool mode);
void ll_timer_int_mode(ll_timer_t timer, ll_int_mode_t mode);
void ll_timer_alarm (ll_timer_t timer, bool mode);
void ll_set_freq_divider (ll_timer_t timer, int divider);