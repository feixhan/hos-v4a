/** 
 *  Hyper Operating System V4 Advance
 *
 * @file  tsnd_dtq.c
 * @brief %jp{データキューへの送信(タイムアウトあり)}%en{Send to Data Queue(with Timeout)}
 *
 * Copyright (C) 1998-2009 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */



#include "core/core.h"
#include "object/dtqobj.h"



#if _KERNEL_SPT_TSND_DTQ


/** %jp{データキューへの送信(タイムアウトあり)}%en{Send to Data Queue(with Timeout)}
 * @param  dtqid    %jp{送信対象のデータキューのID番号}%en{ID number of the data queue to which the data element is sent}
 * @param  data     %jp{データキューへ送信するデータ}%en{Data element to be sent}
 * @param  tmout    %jp{タイムアウト指定}%en{Specified timeout}
 * @retval E_OK     %jp{正常終了}%en{Normal completion}
 * @retval E_ID     %jp{不正ID番号(dtqidが不正あるいは使用できない)}%en{Invalid ID number(dtqid is invalid or unusable)}
 * @retval E_CTX    %jp{コンテキストエラー}%en{Context error}
 * @retval E_NOEXS  %jp{オブジェクト未生成(対象データキューが未登録)}%en{Non-existant object(specified data queue is not registerd)}
 * @retval E_PAR    %jp{パラメータエラー(tmoutが不正)}%en{Parameter error(tmout is invalid)}
 * @retval E_RLWAI  %jp{待ち状態の強制解除(待ち状態の間にrel_waiを受付)}%en{Forced release from waiting(accept rel_wai while waiting)}
 * @retval E_TMOUT  %jp{タイムアウト}%en{Timeout}
 * @retval E_DLT    %jp{待ちオブジェクトの削除(待ち状態の間に対象セマフォが削除)}%en{Waiting object deleted(semaphore is deleted waiting)}
 */
ER tsnd_dtq(ID dtqid, VP_INT data, TMO tmout)
{
	return _kernel_snd_dtq(dtqid, data, tmout);
}


#else	/* _KERNEL_SPT_TSND_DTQ */


#if _KERNEL_SPT_TSND_DTQ_E_NOSPT

/** %jp{データキューへの送信(タイムアウトあり)}%en{Send to Data Queue(with Timeout)}
 * @param  dtqid    %jp{送信対象のデータキューのID番号}%en{ID number of the data queue to which the data element is sent}
 * @param  data     %jp{データキューへ送信するデータ}%en{Data element to be sent}
 * @param  tmout    %jp{タイムアウト指定}%en{Specified timeout}
 * @retval E_NOSPT  %jp{未サポート機能}%en{Unsupported function}
 */
ER tsnd_dtq(ID dtqid, VP_INT data, TMO tmout)
{
	return E_NOSPT;
}

#endif


#endif	/* _KERNEL_SPT_TSND_DTQ */



#if _KERNEL_SPT_KSND_DTQ

ER _kernel_snd_dtq(ID dtqid, VP_INT data, TMO tmout)
{
	_KERNEL_T_DTQCB  *dtqcb;
	_KERNEL_T_TSKHDL tskhdl;
	_KERNEL_T_TCB    *tcb;
	ER               ercd;
	
	/* %jp{コンテキストチェック} */
#if _KERNEL_SPT_TSND_DTQ_E_CTX
	if ( _KERNEL_SYS_SNS_DPN() )
	{
		return E_CTX;			/* %jp{コンテキストエラー}%en{Context error} */
	}
#endif
	
	/* %jp{ID のチェック} */
#if _KERNEL_SPT_TSND_DTQ_E_ID
	if ( !_KERNEL_DTQ_CHECK_DTQID(dtqid) )
	{
		return E_ID;	/* %jp{不正ID番号}%en{Invalid ID number} */
	}
#endif

	/* %jp{パラメータのチェック} */
#if _KERNEL_SPT_TSND_DTQ_E_PAR
	if ( tmout < 0 && tmout != TMO_FEVR && tmout != TMO_POL)
	{
		return E_PAR;	/* %jp{パラメータエラー(tmoutが不正)}%en{Parameter error(tmout is invalid)} */
	}
#endif
	
	_KERNEL_ENTER_SVC();	/* %jp{サービスコールに入る}%en{enter service-call} */
	
	/* %jp{オブジェクト存在チェック} */
#if _KERNEL_SPT_TSND_DTQ_E_NOEXS
	if ( !_KERNEL_DTQ_CHECK_EXS(dtqid) )
	{
		_KERNEL_LEAVE_SVC();	/* %jp{サービスコール終了} */
		return E_NOEXS;			/* %jp{オブジェクト未生成} */
	}
#endif
	
	/* %jp{データキューコントロールブロック取得} */
	dtqcb = _KERNEL_DTQ_ID2DTQCB(dtqid);
	
	/* %jp{受信待ち行列先頭からタスク取り出し} */
	tskhdl = _KERNEL_DTQ_RMH_RQUE(dtqcb);
	if ( tskhdl != _KERNEL_TSKHDL_NULL )
	{
		VP_INT *p_data;

		/* %jp{待ちタスクがあれば待ち解除} */
		tcb = _KERNEL_TSK_TSKHDL2TCB(tskhdl);		/* %jp{TCB取得} */
		_KERNEL_TSK_SET_ERCD(tcb, E_OK);			/* %jp{エラーコード設定} */
		p_data = (VP_INT *)_KERNEL_TSK_GET_DATA(tcb);
		*p_data = data;
		_KERNEL_DSP_WUP_TSK(tskhdl);				/* %jp{タスクの待ち解除} */
		_KERNEL_DTQ_RMV_RTOQ(tskhdl);
		
		/* %jp{タスクディスパッチの実行} */
		_KERNEL_DSP_TSK();
		
		ercd = E_OK;	/* %jp{正常終了}%en{Normal completion} */
	}
	else
	{
		const _KERNEL_T_DTQCB_RO *dtqcb_ro;
		_KERNEL_DTQ_T_DTQCNT     sdtqcnt;
		_KERNEL_DTQ_T_DTQCNT     dtqcnt;
		
		/* %jp{RO部取得} */
		dtqcb_ro = _KERNEL_DTQ_GET_DTQCB_RO(dtqid, dtqcb);

		/* %jp{データキューカウンタ取得} */
		sdtqcnt = _KERNEL_DTQ_GET_SDTQCNT(dtqcb);
		dtqcnt  = _KERNEL_DTQ_GET_DTQCNT(dtqcb_ro);

		if ( sdtqcnt < dtqcnt )		/* %jp{キューに空きはあるか？} */
		{
			_KERNEL_DTQ_T_DTQCNT head;
			VP_INT               *dtq;

			/* %jp{データキュー情報取得} */
			head = _KERNEL_DTQ_GET_HEAD(dtqcb);
			dtq  = _KERNEL_DTQ_GET_DTQ(dtqcb_ro);
			
			/* %jp{データキュー末尾に追加} */
			if ( head < dtqcnt - sdtqcnt - 1 )
			{
				dtq[head + sdtqcnt] = data;
			}
			else
			{
				dtq[head + sdtqcnt - dtqcnt] = data;
			}
			
			/* %jp{データ個数加算} */
			sdtqcnt++;
			_KERNEL_DTQ_SET_SDTQCNT(dtqcb, sdtqcnt);

			ercd = E_OK;	/* %jp{正常終了}%en{Normal completion} */
		}
		else
		{
#if _KERNEL_SPT_TSND_DTQ || _KERNEL_SPT_PSND_DTQ
			if ( tmout == TMO_POL )
			{
				ercd = E_TMOUT;  /* %jp{タイムアウト}%en{Timeout} */
			}
			else
#endif
			{
				/* %jp{タスクを待ち状態にする} */
				tskhdl = _KERNEL_SYS_GET_RUNTSK();
				tcb    = _KERNEL_TSK_TSKHDL2TCB(tskhdl);			/* %jp{TCB取得} */
				_KERNEL_TSK_SET_TSKWAIT(tcb, _KERNEL_TTW_SDTQ);
				_KERNEL_TSK_SET_WOBJID(tcb, dtqid);
				_KERNEL_TSK_SET_DATA(tcb, data);
				_KERNEL_DSP_WAI_TSK(tskhdl);
				_KERNEL_DTQ_ADD_SQUE(dtqcb, dtqcb_ro, tskhdl);		/* %jp{待ち行列に追加} */

#if _KERNEL_SPT_TSND_DTQ
			if ( tmout != TMO_FEVR )
			{
				_KERNEL_DTQ_ADD_STOQ(tskhdl, tmout);				/* %jp{タイムアウトキューに追加} */
			}
#endif
				/* %jp{タスクディスパッチの実行} */
				_KERNEL_DSP_TSK();
				
				/* %jp{エラーコードの取得} */
				ercd = _KERNEL_TSK_GET_ERCD(tcb);
			}
		}		
	}
	
	_KERNEL_LEAVE_SVC();	/* %jp{サービスコールから出る}%en{leave service-call} */
	
	return ercd;
}

#endif	/* _KERNEL_SPT_KSND_DTQ */



/* end of file */
