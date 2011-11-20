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
	bool isEmpty(bool bBrowsed=false);
	void Push(unsigned int nParent, unsigned int nIndex, ELEMENT_TYPE weight=0);
	int Pop(unsigned int *pParent=0, unsigned int *pIndex=0, ELEMENT_TYPE *weight=0);
	int GetFirst(unsigned int *pParent=0, unsigned int *pIndex=0, ELEMENT_TYPE *weight=0);
	int GetNext(unsigned int *pParent=0, unsigned int *pIndex=0, ELEMENT_TYPE *weight=0);
	void OutputElement(int *pOutput, unsigned int *nLenght=0);
	bool CanGetNext();
private:
	PQUEUE_ELEM m_pHead;
	PQUEUE_ELEM m_pLastAccess;
};
#endif