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
 
//#include <stdio.h>
#include "timer_2026.h"

/**
 * @brief Array for mapping timer with his TIMGn_TxCONFIG_REG register
 *
 * @details
 * It let you to access to his register just knowing the timer
 *
 * @note designated initializers technique
 */
volatile uint32_t *TIMGn_TxCONFIG_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0CONFIG_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0CONFIG_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1CONFIG_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1CONFIG_REG
};

volatile uint32_t *TIMGn_TxLO_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LO_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LO_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LO_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LO_REG
};

volatile uint32_t *TIMGn_TxHI_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0HI_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0HI_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1HI_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1HI_REG
};

volatile uint32_t *TIMGn_TxUPDATE_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMGn_T0UPDATE_REG,
	[LL_TIMG1_T0]  = LL_TIMGn_T0UPDATE_REG,
	[LL_TIMG0_T1]  = LL_TIMGn_T1UPDATE_REG,
	[LL_TIMG1_T1]  = LL_TIMGn_T1UPDATE_REG
};

volatile uint32_t *TIMGn_TxALARMLO_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0ALARMLO_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0ALARMLO_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1ALARMLO_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1ALARMLO_REG
};

volatile uint32_t *TIMGn_TxALARMHI_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0ALARMHI_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0ALARMHI_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1ALARMHI_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1ALARMHI_REG
};

volatile uint32_t *TIMGn_TxLOADLO_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LOADLO_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LOADLO_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LOADLO_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LOADLO_REG
};

volatile uint32_t *TIMGn_TxLOADHI_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LOADHI_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LOADHI_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LOADHI_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LOADHI_REG
};

volatile uint32_t *TIMGn_INT_RAW_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_INT_RAW_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_INT_RAW_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_INT_RAW_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_INT_RAW_REG
};

volatile uint32_t *TIMGn_INT_CLR_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_INT_CLR_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_INT_CLR_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_INT_CLR_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_INT_CLR_REG
};

volatile uint32_t *TIMGn_TxLOAD_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0LOAD_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0LOAD_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1LOAD_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1LOAD_REG
};

void ll_timer_enable(ll_timer_t timer, bool bit_state){
	if(bit_state){
		LL_ENABLE_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	} else {
		LL_DISABLE_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	}
}

void ll_timer_count_mode(ll_timer_t timer, ll_count_mode_t mode){
	if(mode == LL_UP){
		LL_UP_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	} else {
		LL_DOWN_COUNTING(TIMGn_TxCONFIG_REG[timer]);
	}
}

void ll_timer_autoreload(ll_timer_t timer, bool mode){
	if (mode){
		LL_ENABLE_AUTORELOAD(TIMGn_TxCONFIG_REG[timer]);
	} else{
		LL_DISABLE_AUTORELOAD(TIMGn_TxCONFIG_REG[timer]);
	}
}

void ll_timer_int_mode(ll_timer_t timer, ll_int_mode_t mode){
	if(mode == LL_LEVEL){
		LL_ENABLE_LEVEL_INT(TIMGn_TxCONFIG_REG[timer]);
	} else{
		LL_ENABLE_EDGE_INT(TIMGn_TxCONFIG_REG[timer]);
	}
}

void ll_timer_alarm_enable(ll_timer_t timer, bool mode){
	if (mode){
		LL_ENABLE_ALARM(TIMGn_TxCONFIG_REG[timer]);
	} else {
		LL_DISABLE_ALARM(TIMGn_TxCONFIG_REG[timer]);
	}
}

void ll_set_freq_divider (ll_timer_t timer, int divider){
	LL_CLEAR_DIVIDER(TIMGn_TxCONFIG_REG[timer]);
	LL_SET_DIVIDER(TIMGn_TxCONFIG_REG[timer], divider);
}


void ll_set_alarm_value(ll_timer_t timer, uint64_t value){
	uint32_t valueLO = value;
	uint32_t valueHI = value>>32;
	
	LL_CLEAR_32BIT_REG(TIMGn_TxALARMLO_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxALARMLO_REG[timer],valueLO);
	
	LL_CLEAR_32BIT_REG(TIMGn_TxALARMHI_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxALARMHI_REG[timer],valueHI);
}

void ll_set_load_value(ll_timer_t timer, uint64_t value){
	uint32_t valueLO = value;
	uint32_t valueHI = value>>32;
	
	LL_CLEAR_32BIT_REG(TIMGn_TxLOADLO_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxLOADLO_REG[timer],valueLO);
	
	LL_CLEAR_32BIT_REG(TIMGn_TxLOADHI_REG[timer]);
	LL_SET_32BIT_REG(TIMGn_TxLOADHI_REG[timer],valueHI);
}

void ll_charge_load_value(ll_timer_t timer){
	LL_SET_32BIT_REG(TIMGn_TxLOAD_REG[timer],1);
}

void ll_update_current_time_value(ll_timer_t timer){
	LL_SET_32BIT_REG(TIMGn_TxUPDATE_REG[timer],1);
}

void ll_clear_int(ll_timer_t timer, ll_clear_int_t interruption){
	switch (interruption){
		case LL_CLR_T0_INT:
			LL_SET_BIT_T0_INT_CLR(TIMGn_INT_CLR_REG[timer]);
			break;
		case LL_CLR_T1_INT:
			LL_SET_BIT_T1_INT_CLR(TIMGn_INT_CLR_REG[timer]);
			break;
		default:
			LL_SET_BIT_WDT_INT_CLR(TIMGn_INT_CLR_REG[timer]);
	}
}