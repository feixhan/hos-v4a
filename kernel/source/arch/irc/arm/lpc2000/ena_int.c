/** 
 *  Hyper Operating System V4 Advance
 *
 * @file  ena_int.c
 * @brief @brief %jp{Philips LPC2000ファミリ用}%en{Philips LPC2000 family}
 * Copyright (C) 1998-2007 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */


#include "core/core.h"
#include "object/inhobj.h"
#include "object/isrobj.h"


/* 割込みの許可 */
ER ena_int(INTNO intno)
{
	*_KERNEL_IRC_VICIntEnable = (1 << intno);
	
	return E_OK;
}


/* end of file */
