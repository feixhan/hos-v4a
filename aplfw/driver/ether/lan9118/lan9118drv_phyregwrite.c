/** 
 *  Hyper Operating System  Application Framework
 *
 * @file  lan9118drv_open.c
 * @brief %jp{SMSC LAN9118シリーズ用デバイスドライバ}
 *
 * Copyright (C) 2006-2007 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */


#include "lan9118drv_local.h"


void Lan9118Drv_PhyRegWrite(C_LAN9118DRV *self, unsigned char ubAddr, unsigned short uhData)
{
	/* Busy解除待ち */
	while ( (Lan9118Drv_CsrRegRead(self, LAN9118_CSR_MII_ACC) & 0x00000001) )
	{
		SysTim_Wait(50);
	}
	
	/* 書き込み */
	Lan9118Drv_CsrRegWrite(self, LAN9118_CSR_MII_DATA, uhData);
	Lan9118Drv_CsrRegWrite(self, LAN9118_CSR_MII_ACC, 0x00000803 | (ubAddr << 6));
	
	/* Busy解除待ち */
	while ( (Lan9118Drv_CsrRegRead(self, LAN9118_CSR_MII_ACC) & 0x00000001) )
	{
		SysTim_Wait(50);
	}
}
