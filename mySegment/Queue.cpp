#include "Queue.h"

CQueue::CQueue()
{
	m_pHead = NULL;
	m_pLastAccess = NULL;
}

CQueue::~CQueue()
{
	PQUEUE_ELEM pCur, pTemp;
	pCur = m_pHead;
	while (pCur!=NULL)
	{
		pTemp = pCur;
		pCur = pCur->next;
		free(pTemp);
	}
}

void CQueue::Clear()
{
	PQUEUE_ELEM pCur, pTemp;
	pCur = m_pHead;
	while (pCur!=NULL)
	{
		pTemp = pCur;
		pCur = pCur->next;
		free(pTemp);
	}
	m_pHead = NULL;
	m_pLastAccess = NULL;
}

bool CQueue::isSingle()
{
	return (m_pHead!=NULL&&m_pHead->next==NULL);
}

bool CQueue::isEmpty(bool bBrowsed)
{
	if (bBrowsed)
		return (m_pLastAccess==NULL);
	return (m_pHead==NULL);
}

void CQueue::Push(unsigned int nParent, unsigned int nIndex, double weight)
{
	PQUEUE_ELEM pCur=m_pHead, pPre=0, pAdd;
	while (pCur!=NULL && pCur->weight<weight)
	{
		pPre = pCur;
		pCur = pCur->next;
	}
	pAdd = (PQUEUE_ELEM)malloc(sizeof(QUEUE_ELEM));
	pAdd->nIndex = nIndex;
	pAdd->nParent = nParent;
	pAdd->weight = weight;

	pAdd->next = pCur;
	if (pPre==NULL)
		m_pHead = pAdd;
	else
		pPre->next = pAdd;
}

int CQueue::Pop(unsigned int *pParent, unsigned int *pIndex, ELEMENT_TYPE *weight)
{
	PQUEUE_ELEM pTemp;
	if (m_pHead==NULL)
		return -1;
	if (pParent!=NULL)
		*pParent = m_pHead->nParent;
	if (pIndex!=NULL)
		*pIndex = m_pHead->nIndex;
	if (weight!=NULL)
		*weight = m_pHead->weight;
	pTemp = m_pHead;
	m_pHead = m_pHead->next;
	free(pTemp);
	return 1;
}

int CQueue::GetFirst(unsigned int *pParent, unsigned int *pIndex, ELEMENT_TYPE *weight)
{
	if (m_pHead==NULL)
		return -1;
	m_pLastAccess = m_pHead;
	if (pParent!=NULL)
		*pParent = m_pHead->nParent;
	if (pIndex!=NULL)
		*pIndex = m_pHead->nIndex;
	if (weight!=NULL)
		*weight = m_pHead->weight;
}




