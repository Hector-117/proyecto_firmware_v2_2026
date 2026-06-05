#include <stdio.h>
#include "timer_2026.h"

volatile uint32_t *TIMGn_TxCONFIG_REG[] = {
	[LL_TIMG0_T0]  = LL_TIMG0_T0CONFIG_REG,
	[LL_TIMG1_T0]  = LL_TIMG1_T0CONFIG_REG,
	[LL_TIMG0_T1]  = LL_TIMG0_T1CONFIG_REG,
	[LL_TIMG1_T1]  = LL_TIMG1_T1CONFIG_REG,
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

void ll_timer_alarm (ll_timer_t timer, bool mode){
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


