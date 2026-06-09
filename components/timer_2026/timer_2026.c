/**
 * @file timer_2026.c
 * @brief Low-Level timer Driver for ESP32 source file.
 *
 * @details
 * This file contains function implementations
 * for direct timer control by ESP32 registers.
 *
 * @author Hector Said Herrera Niño
 * @author José Francisco Padilla Torres
 * @date 2026-06-04
 *
 * @note Based on ESP32 Technical Reference Manual v4.6. 
 */
 
#include <stdio.h>
#include "timer_2026.h"

/**
 * @brief Timer configuration register mapping table.
 *
 * @details
 * Each timer is associated with its configuration register.
 * This allows selecting the timer using the ll_timer_t enum.
 */
volatile uint32_t *TIMGn_TxCONFIG_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0CONFIG_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0CONFIG_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1CONFIG_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1CONFIG_REG
};

/**
 * @brief Timer low counter register mapping table.
 *
 * @details
 * Stores the register address used to read the lower
 * 32 bits of the current timer value.
 */
volatile uint32_t *TIMGn_TxLO_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LO_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LO_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LO_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LO_REG
};

/**
 * @brief Timer high counter register mapping table.
 *
 * @details
 * Stores the register address used to read the upper
 * 32 bits of the current timer value.
 */
volatile uint32_t *TIMGn_TxHI_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0HI_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0HI_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1HI_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1HI_REG
};

/**
 * @brief Timer update register mapping table.
 *
 * @details
 * Writing to this register updates the current counter value
 * into the LO and HI registers.
 */
volatile uint32_t *TIMGn_TxUPDATE_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0UPDATE_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0UPDATE_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1UPDATE_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1UPDATE_REG
};

/**
 * @brief Timer alarm low register mapping table.
 *
 * @details
 * Stores the register address used to configure the lower
 * 32 bits of the alarm value.
 */
volatile uint32_t *TIMGn_TxALARMLO_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0ALARMLO_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0ALARMLO_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1ALARMLO_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1ALARMLO_REG
};

/**
 * @brief Timer alarm high register mapping table.
 *
 * @details
 * Stores the register address used to configure the upper
 * 32 bits of the alarm value.
 */
volatile uint32_t *TIMGn_TxALARMHI_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0ALARMHI_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0ALARMHI_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1ALARMHI_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1ALARMHI_REG
};

/**
 * @brief Timer load low register mapping table.
 *
 * @details
 * Stores the register address used to configure the lower
 * 32 bits of the reload value.
 */
volatile uint32_t *TIMGn_TxLOADLO_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LOADLO_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LOADLO_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LOADLO_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LOADLO_REG
};

/**
 * @brief Timer load high register mapping table.
 *
 * @details
 * Stores the register address used to configure the upper
 * 32 bits of the reload value.
 */
volatile uint32_t *TIMGn_TxLOADHI_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LOADHI_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LOADHI_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LOADHI_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LOADHI_REG
};

/**
 * @brief Timer raw interrupt register mapping table.
 *
 * @details
 * Stores the interrupt status register according to
 * the timer group used.
 */
volatile uint32_t *TIMGn_INT_RAW_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_INT_RAW_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_INT_RAW_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_INT_RAW_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_INT_RAW_REG
};

/**
 * @brief Timer interrupt clear register mapping table.
 *
 * @details
 * Stores the register used to clear the interrupt flag
 * according to the timer group used.
 */
volatile uint32_t *TIMGn_INT_CLR_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_INT_CLR_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_INT_CLR_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_INT_CLR_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_INT_CLR_REG
};

/**
 * @brief Timer load trigger register mapping table.
 *
 * @details
 * Writing any value to this register loads the configured
 * reload value into the timer counter.
 */
volatile uint32_t *TIMGn_TxLOAD_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LOAD_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LOAD_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LOAD_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LOAD_REG
};

/**
 * @brief Enable or disable timer counting.
 * @param timer Timer to configure.
 * @param bit_state true = enable counting, false = disable counting.
 */

void ll_timer_enable(ll_timer_t timer, bool bit_state){
	if(bit_state){
		LL_ENABLE_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	} else {
		LL_DISABLE_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	}
}
/**
 * @brief Select timer count direction.
 * @param timer Timer to configure.
 * @param mode LL_UP for upward count, LL_DOWN for downward count.
 */

void ll_timer_count_mode(ll_timer_t timer, ll_count_mode_t mode){
	if(mode == LL_UP){
		LL_UP_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	} else {
		LL_DOWN_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	}
}
/**
 * @brief Enable or disable timer autoreload.
 * @param timer Timer to configure.
 * @param mode true = autoreload enabled, false = autoreload disabled.
 */

void ll_timer_autoreload(ll_timer_t timer, bool mode){
	if (mode){
		LL_ENABLE_AUTORELOAD(TIMGn_TxCONFIG_REG[timer]);
	} else{
		LL_DISABLE_AUTORELOAD(TIMGn_TxCONFIG_REG[timer]);
	}
}
/**
 * @brief Select timer interrupt mode.
 * @param timer Timer to configure.
 * @param mode LL_LEVEL for level interrupt, LL_EDGE for edge interrupt.
 */

void ll_timer_int_mode(ll_timer_t timer, ll_int_mode_t mode){
	if(mode == LL_LEVEL){
		LL_ENABLE_LEVEL_INT(TIMGn_TxCONFIG_REG[timer]);
	} else{
		LL_ENABLE_EDGE_INT(TIMGn_TxCONFIG_REG[timer]);
	}
}
/**
 * @brief Enable or disable timer alarm.
 * @param timer Timer to configure.
 * @param mode true = alarm enabled, false = alarm disabled.
 */

void ll_timer_alarm_enable(ll_timer_t timer, bool mode){
	if (mode){
		LL_ENABLE_ALARM(TIMGn_TxCONFIG_REG[timer]);
	} else {
		LL_DISABLE_ALARM(TIMGn_TxCONFIG_REG[timer]);
	}
}
/**
 * @brief Set timer frequency divider.
 * @param timer Timer to configure.
 * @param divider Prescaler divider value.
 * @details
 * The timer clock is divided from the APB clock.
 * For example, with 80 MHz APB and divider 80,
 * the timer counts at 1 MHz.
 */

void ll_set_freq_divider (ll_timer_t timer, int divider){
	LL_CLEAR_DIVIDER(TIMGn_TxCONFIG_REG[timer]);
	LL_SET_DIVIDER(TIMGn_TxCONFIG_REG[timer], divider);
}

/**
 * @brief Set timer alarm value.
 * @param timer Timer to configure.
 * @param value Alarm value in timer ticks.
 * @details
 * The 64-bit value is divided into low and high
 * 32-bit registers.
 */

void ll_set_alarm_value(ll_timer_t timer, uint64_t value){
	uint32_t valueLO = value;
	uint32_t valueHI = value>>32;
	
	LL_CLEAR_32BIT_REG(TIMGn_TxALARMLO_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxALARMLO_REG[timer],valueLO);
	
	LL_CLEAR_32BIT_REG(TIMGn_TxALARMHI_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxALARMHI_REG[timer],valueHI);
}

/**
 * @brief Set timer load value.
 * @param timer Timer to configure.
 * @param value Load value in timer ticks.
 * @details
 * The 64-bit value is divided into low and high
 * 32-bit registers.
 */

void ll_set_load_value(ll_timer_t timer, uint64_t value){
	uint32_t valueLO = value;
	uint32_t valueHI = value>>32;
	
	LL_CLEAR_32BIT_REG(TIMGn_TxLOADLO_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxLOADLO_REG[timer],valueLO);
	
	LL_CLEAR_32BIT_REG(TIMGn_TxLOADHI_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxLOADHI_REG[timer],valueHI);
}
/**
 * @brief Load configured value into timer counter.
 * @param timer Timer to configure.
 * @details
 * Writing any value to the LOAD register forces
 * the timer to reload its counter value.
 */

void ll_charge_load_value(ll_timer_t timer){
	LL_SET_32BIT_REG(TIMGn_TxLOAD_REG[timer],1);
}
/**
 * @brief Update current timer counter value.
 * @param timer Timer to update.
 * @details
 * Writing to the UPDATE register copies the current
 * counter value into the LO and HI registers.
 */

void ll_update_current_time_value(ll_timer_t timer){
	LL_SET_32BIT_REG(TIMGn_TxUPDATE_REG[timer],1);
}

/**
 * @brief Clear timer interrupt flag.
 * @param timer Timer whose interrupt flag will be cleared.
 * @details
 * Selects the correct clear bit depending on the timer used.
 */

void ll_clear_int(ll_timer_t timer){
	switch (timer){
		case LL_TIMG0_T0:
			LL_SET_BIT_T0_INT_CLR(TIMGn_INT_CLR_REG[timer]);
			break;
		case LL_TIMG0_T1:
			LL_SET_BIT_T1_INT_CLR(TIMGn_INT_CLR_REG[timer]);
			break;
		case LL_TIMG1_T0:
			LL_SET_BIT_T0_INT_CLR(TIMGn_INT_CLR_REG[timer]);
			break;
		case LL_TIMG1_T1:
			LL_SET_BIT_T1_INT_CLR(TIMGn_INT_CLR_REG[timer]);
			break;
		default:
			LL_SET_BIT_WDT_INT_CLR(TIMGn_INT_CLR_REG[timer]);
	}
}