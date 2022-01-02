/*****************************************************************************************************
 * �ļ���list.h
 * ���ܣ�����ı������ͺ��ⲿ�ӿ�����
 * ���ߣ�lxx
 * ���ڣ�2022/01/01
 *      �µ�һ�꣬�µĿ�ʼ��ףԸ�����Զ���ꡣ����
*****************************************************************************************************/

#ifndef __LIST_H_
#define __LIST_H_

struct _list_mono {
    struct _list_mono *  pListMonoNext;
};
typedef struct _list_mono        list_mono_t;
typedef list_mono_t *            plist_mono_t;

struct _list_mono_resc {
    plist_mono_t        plistmonoHead;
    plist_mono_t        plistmonoTail;
};
typedef struct _list_mono_resc  list_mono_resc_t;
typedef list_mono_resc_t *      plist_mono_resc_t;


enum _list_return {
    LIST_RET_NONE = 0,
    LIST_RET_ARGS = 1
};

typedef enum _list_return        list_return_t;

#include <stdio.h>
#define os_log      printf
#define os_err      printf


list_return_t ListMonoHeadInsert(plist_mono_resc_t  pListResc, 
                                    plist_mono_t  pListNode);

list_return_t ListMonoTailInsert(plist_mono_resc_t  pListResc, 
                                    plist_mono_t  pListNode);

list_return_t ListMonoHeadDelete(plist_mono_resc_t  pListResc,
                                    plist_mono_t * ppListNode);

list_return_t ListMonoTailDelete(plist_mono_resc_t  pListResc,
                                    plist_mono_t * ppListNode);

list_return_t ListMonoNodeDelete(plist_mono_resc_t  pListResc,
                                    plist_mono_t  pListNode);
#endif                                                                  /* __LIST_H_                */
/************************************* END OF FILES *************************************************/
