/*****************************************************************************************************
 * 文件：list.c
 * 介绍：链表外部接口实现
 * 作者：lxx
 * 日期：2022/01/01
 *      新的一年，新的开始，祝愿大家永远少年。。。
*****************************************************************************************************/

#include "list.h"

/*****************************************************************************************************
 * 函数：ListMonoHeadInsert
 * 介绍：将单向链表节点从单向链表资源的头部插入
 * 输入：pListResc		 单向链表资源指针
 * 		pListNode		单向链表节点指针
 * 输出：LIST_RET_NONE	 链表插入完成
 * 		LIST_RET_ARGS	传入参数异常，插入失败
*****************************************************************************************************/
list_return_t ListMonoHeadInsert (plist_mono_resc_t  pListResc, 
                                    plist_mono_t  pListNode)
{
    /*
        1、判断资源链表头指针是否为空
        2、判断链表节点指针是否为空
    */
    if (!pListResc || !pListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
    /*
        为了保证链表的纯净，对于节点的元素也要进行判断
    */
    if (pListNode->pListMonoNext) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1、若当前链表中没有节点，则头尾指针全部指向该节点
		2、若当前链表中有节点，则将所有节点放到该节点之后
	*/
    if (!pListResc->plistmonoHead) {
		pListResc->plistmonoTail = pListNode;
	} else {
		pListNode->pListMonoNext = pListResc->plistmonoHead;
	}
	pListResc->plistmonoHead = pListNode;

	return (LIST_RET_NONE);
}
/*****************************************************************************************************
 * ListMonoTailInsert
 * 介绍：将单向链表节点从单向链表资源的尾部插入
 * 输入：pListResc		 单向链表资源指针
 * 		pListNode		单向链表节点指针
 * 输出：LIST_RET_NONE	 链表插入完成
 * 		LIST_RET_ARGS	传入参数异常，插入失败
*****************************************************************************************************/
list_return_t ListMonoTailInsert (plist_mono_resc_t  pListResc, 
                                    plist_mono_t  pListNode)
{
    /*
        1、判断资源链表头指针是否为空
        2、判断链表节点指针是否为空
    */
    if (!pListResc || !pListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
    /*
        为了保证链表的纯净，对于节点的元素也要进行判断
    */
    if (pListNode->pListMonoNext) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1、若当前链表中没有节点，则头尾指针全部指向该节点
		2、若当前链表中有节点，则将所有节点放到该节点之前
	*/
    if (!pListResc->plistmonoTail) {
		pListResc->plistmonoHead = pListNode;
	} else {
		pListResc->plistmonoTail->pListMonoNext = pListNode;
	}
	pListResc->plistmonoTail = pListNode;

	return (LIST_RET_NONE);
}
/*****************************************************************************************************
 * ListMonoHeadDelete
 * 介绍：单向链表资源的头部节点删除
 * 输入：pListResc		 单向链表资源指针
 * 		ppListNode		存储被删除的节点指针
 * 输出：LIST_RET_NONE	 链表插入完成
 * 		LIST_RET_ARGS	传入参数异常，插入失败
*****************************************************************************************************/
list_return_t ListMonoHeadDelete (plist_mono_resc_t  pListResc,
                                    plist_mono_t * ppListNode)
{
    /*
        1、判断资源链表头指针是否为空
		2、判断保存被删除节点的指针是否存在
    */
    if (!pListResc || !ppListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1、若当前链表中没有节点，则直接返回
		2、若当前链表中只有一个节点，则使头尾部指针为空
		3、若当前链表中有多个节点，则头指针指向下一个节点
	*/
    if (!pListResc->plistmonoHead) {
		*ppListNode = NULL;
	} else {
		*ppListNode = pListResc->plistmonoHead;
		(*ppListNode)->pListMonoNext = NULL;

		if (pListResc->plistmonoHead == pListResc->plistmonoTail) {
			pListResc->plistmonoHead = NULL;
			pListResc->plistmonoTail = NULL;
		} else {
			pListResc->plistmonoHead = pListResc->plistmonoHead->pListMonoNext;
		}
	}

	return (LIST_RET_NONE);
}
/*****************************************************************************************************
 * ListMonoTailDelete
 * 介绍：单向链表资源的尾部节点删除
 * 输入：pListResc		 单向链表资源指针
 * 		ppListNode		存储被删除的节点指针
 * 输出：LIST_RET_NONE	 链表插入完成
 * 		LIST_RET_ARGS	传入参数异常，插入失败
*****************************************************************************************************/
list_return_t ListMonoTailDelete (plist_mono_resc_t  pListResc,
                                    plist_mono_t * ppListNode)
{
	plist_mono_t  pTempNode = NULL;
    /*
        1、判断资源链表头指针是否为空
		2、判断保存被删除节点的指针是否存在
    */
    if (!pListResc || !ppListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1、若当前链表中没有节点，则直接返回
		2、若当前链表中只有一个节点，则使头尾部指针为空
		3、若当前链表中有多个节点，则尾指针指向前一个节点
	*/
    if (!pListResc->plistmonoTail) {
		*ppListNode = NULL;
	} else {
		*ppListNode = pListResc->plistmonoTail;

		if (pListResc->plistmonoHead == pListResc->plistmonoTail){
			pListResc->plistmonoHead = NULL;
			pListResc->plistmonoTail = NULL;
		} else {
			pTempNode = pListResc->plistmonoHead;
			while (pTempNode->pListMonoNext != pListResc->plistmonoTail) {
				pTempNode = pTempNode->pListMonoNext;
			}
			pTempNode->pListMonoNext = NULL;
			pListResc->plistmonoTail = pTempNode;
		}
	}

	return (LIST_RET_NONE);
}
/*****************************************************************************************************
 * ListMonoTailDelete
 * 介绍：单向链表资源的中的节点删除
 * 输入：pListResc		 单向链表资源指针
 * 		pListNode		被删除的节点指针
 * 输出：LIST_RET_NONE	 链表插入完成
 * 		LIST_RET_ARGS	传入参数异常，插入失败
*****************************************************************************************************/
list_return_t ListMonoNodeDelete (plist_mono_resc_t  pListResc,
                                    plist_mono_t  pListNode)
{
	plist_mono_t   pTempNode  = NULL;
    /*
        1、判断资源链表头指针是否为空
		2、判断被删除节点的指针是否存在
    */
    if (!pListResc || !pListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1、当前链表中无节点，则直接返回
	*/
NoNode:
	if (!pListResc->plistmonoHead) {
		pListResc->plistmonoTail = NULL;
		goto End;
	}

	pTempNode = pListResc->plistmonoHead;
	/*
		2、当前链表中只有一个节点，且该节点为需要删除的节点，则需要将头尾指针置空
	*/	
	if (pListResc->plistmonoHead == pListNode) {
		pListResc->plistmonoHead = pListResc->plistmonoHead->pListMonoNext;
		goto NoNode;
	}
	/*
		3、循环找到要删除的节点，并将其删除
	*/
	while (pTempNode->pListMonoNext) {
		if (pTempNode->pListMonoNext == pListNode) {
			pTempNode->pListMonoNext = pTempNode->pListMonoNext->pListMonoNext;
			break;
		}
		pTempNode = pTempNode->pListMonoNext;
	}
	/*
		4、若要删除的节点刚好是尾节点，则需要将尾节点前置
	*/
	if (pListResc->plistmonoTail == pListNode) {
		pListResc->plistmonoTail = pTempNode;	
	}

	pListNode->pListMonoNext = NULL;
End:
	
	return (LIST_RET_NONE);
}