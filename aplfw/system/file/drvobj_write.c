/** 
 *  Hyper Operating System  Application Framework
 *
 * @file  drvobj.h
 * @brief %jp{デバイスドライバオブジェクトの基本クラス}
 *
 * Copyright (C) 2006-2007 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */


#include "drvobj_local.h"


FILE_SIZE DrvObj_Write(C_DRVOBJ *self, struct c_fileobj *pFileObj, const void *pData, FILE_SIZE Size)
{
	return FILE_ERR_NG;
}


/* end of file */
