#ifndef QUEUE_H
#define QUEUE_H

#include "DynamicArray.h"

struct tagQueueElem{
	unsigned int nParent;
	unsigned int nIndex;
	ELEMENT_TYPE weight;
	struct tagQueueElem *next;
};
typedef struct tagQueueElem QUEUE_ELEM, *PQUEUE_ELEM;

class CQueue
{
public:
	CQueue();
	~CQueue();
	void Clear();
	bool isSingle();
	bool isEmpty(bool bBrowsed);
	void Push(unsigned int nParent, unsigned int nIndex, ELEMENT_TYPE weight);
	int Pop(unsigned int *pParent, unsigned int *pIndex, ELEMENT_TYPE *weight);
	int GetFirst(unsigned int *pParent, unsigned int *pIndex, ELEMENT_TYPE *weight);
private:
	PQUEUE_ELEM m_pHead;
	PQUEUE_ELEM m_pLastAccess;
};
#endif