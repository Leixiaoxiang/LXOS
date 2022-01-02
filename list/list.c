/*****************************************************************************************************
 * �ļ���list.c
 * ���ܣ������ⲿ�ӿ�ʵ��
 * ���ߣ�lxx
 * ���ڣ�2022/01/01
 *      �µ�һ�꣬�µĿ�ʼ��ףԸ�����Զ���ꡣ����
*****************************************************************************************************/

#include "list.h"

/*****************************************************************************************************
 * ������ListMonoHeadInsert
 * ���ܣ�����������ڵ�ӵ���������Դ��ͷ������
 * ���룺pListResc		 ����������Դָ��
 * 		pListNode		��������ڵ�ָ��
 * �����LIST_RET_NONE	 ����������
 * 		LIST_RET_ARGS	��������쳣������ʧ��
*****************************************************************************************************/
list_return_t ListMonoHeadInsert (plist_mono_resc_t  pListResc, 
                                    plist_mono_t  pListNode)
{
    /*
        1���ж���Դ����ͷָ���Ƿ�Ϊ��
        2���ж�����ڵ�ָ���Ƿ�Ϊ��
    */
    if (!pListResc || !pListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
    /*
        Ϊ�˱�֤����Ĵ��������ڽڵ��Ԫ��ҲҪ�����ж�
    */
    if (pListNode->pListMonoNext) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1������ǰ������û�нڵ㣬��ͷβָ��ȫ��ָ��ýڵ�
		2������ǰ�������нڵ㣬�����нڵ�ŵ��ýڵ�֮��
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
 * ���ܣ�����������ڵ�ӵ���������Դ��β������
 * ���룺pListResc		 ����������Դָ��
 * 		pListNode		��������ڵ�ָ��
 * �����LIST_RET_NONE	 ����������
 * 		LIST_RET_ARGS	��������쳣������ʧ��
*****************************************************************************************************/
list_return_t ListMonoTailInsert (plist_mono_resc_t  pListResc, 
                                    plist_mono_t  pListNode)
{
    /*
        1���ж���Դ����ͷָ���Ƿ�Ϊ��
        2���ж�����ڵ�ָ���Ƿ�Ϊ��
    */
    if (!pListResc || !pListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
    /*
        Ϊ�˱�֤����Ĵ��������ڽڵ��Ԫ��ҲҪ�����ж�
    */
    if (pListNode->pListMonoNext) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1������ǰ������û�нڵ㣬��ͷβָ��ȫ��ָ��ýڵ�
		2������ǰ�������нڵ㣬�����нڵ�ŵ��ýڵ�֮ǰ
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
 * ���ܣ�����������Դ��ͷ���ڵ�ɾ��
 * ���룺pListResc		 ����������Դָ��
 * 		ppListNode		�洢��ɾ���Ľڵ�ָ��
 * �����LIST_RET_NONE	 ����������
 * 		LIST_RET_ARGS	��������쳣������ʧ��
*****************************************************************************************************/
list_return_t ListMonoHeadDelete (plist_mono_resc_t  pListResc,
                                    plist_mono_t * ppListNode)
{
    /*
        1���ж���Դ����ͷָ���Ƿ�Ϊ��
		2���жϱ��汻ɾ���ڵ��ָ���Ƿ����
    */
    if (!pListResc || !ppListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1������ǰ������û�нڵ㣬��ֱ�ӷ���
		2������ǰ������ֻ��һ���ڵ㣬��ʹͷβ��ָ��Ϊ��
		3������ǰ�������ж���ڵ㣬��ͷָ��ָ����һ���ڵ�
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
 * ���ܣ�����������Դ��β���ڵ�ɾ��
 * ���룺pListResc		 ����������Դָ��
 * 		ppListNode		�洢��ɾ���Ľڵ�ָ��
 * �����LIST_RET_NONE	 ����������
 * 		LIST_RET_ARGS	��������쳣������ʧ��
*****************************************************************************************************/
list_return_t ListMonoTailDelete (plist_mono_resc_t  pListResc,
                                    plist_mono_t * ppListNode)
{
	plist_mono_t  pTempNode = NULL;
    /*
        1���ж���Դ����ͷָ���Ƿ�Ϊ��
		2���жϱ��汻ɾ���ڵ��ָ���Ƿ����
    */
    if (!pListResc || !ppListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1������ǰ������û�нڵ㣬��ֱ�ӷ���
		2������ǰ������ֻ��һ���ڵ㣬��ʹͷβ��ָ��Ϊ��
		3������ǰ�������ж���ڵ㣬��βָ��ָ��ǰһ���ڵ�
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
 * ���ܣ�����������Դ���еĽڵ�ɾ��
 * ���룺pListResc		 ����������Դָ��
 * 		pListNode		��ɾ���Ľڵ�ָ��
 * �����LIST_RET_NONE	 ����������
 * 		LIST_RET_ARGS	��������쳣������ʧ��
*****************************************************************************************************/
list_return_t ListMonoNodeDelete (plist_mono_resc_t  pListResc,
                                    plist_mono_t  pListNode)
{
	plist_mono_t   pTempNode  = NULL;
    /*
        1���ж���Դ����ͷָ���Ƿ�Ϊ��
		2���жϱ�ɾ���ڵ��ָ���Ƿ����
    */
    if (!pListResc || !pListNode) {
        os_err("%s %d input args error! \r\n", __FUNCTION__, __LINE__);
        return(LIST_RET_ARGS);
    }
	/*
		1����ǰ�������޽ڵ㣬��ֱ�ӷ���
	*/
NoNode:
	if (!pListResc->plistmonoHead) {
		pListResc->plistmonoTail = NULL;
		goto End;
	}

	pTempNode = pListResc->plistmonoHead;
	/*
		2����ǰ������ֻ��һ���ڵ㣬�Ҹýڵ�Ϊ��Ҫɾ���Ľڵ㣬����Ҫ��ͷβָ���ÿ�
	*/	
	if (pListResc->plistmonoHead == pListNode) {
		pListResc->plistmonoHead = pListResc->plistmonoHead->pListMonoNext;
		goto NoNode;
	}
	/*
		3��ѭ���ҵ�Ҫɾ���Ľڵ㣬������ɾ��
	*/
	while (pTempNode->pListMonoNext) {
		if (pTempNode->pListMonoNext == pListNode) {
			pTempNode->pListMonoNext = pTempNode->pListMonoNext->pListMonoNext;
			break;
		}
		pTempNode = pTempNode->pListMonoNext;
	}
	/*
		4����Ҫɾ���Ľڵ�պ���β�ڵ㣬����Ҫ��β�ڵ�ǰ��
	*/
	if (pListResc->plistmonoTail == pListNode) {
		pListResc->plistmonoTail = pTempNode;	
	}

	pListNode->pListMonoNext = NULL;
End:
	
	return (LIST_RET_NONE);
}