#include "DynamicArray.h"

CDynamicArray::CDynamicArray()
{
	m_pArrayHead = NULL;
	m_nRow = 0;
	m_nCol = 0;
}

CDynamicArray::~CDynamicArray()
{
	PARRAY_CHAIN pCur, pTemp;
	pCur = m_pArrayHead;
	while (pCur!=NULL)
	{
		pTemp = pCur;
		pCur = pCur->next;
		if (pTemp->sWord!=NULL)
			delete [] pTemp->sWord;
		free(pTemp);
	}
}

ELEMENT_TYPE CDynamicArray::GetElement(int nRow, int nCol)
{
	PARRAY_CHAIN pCur=m_pArrayHead;
	if (nRow>(int)m_nRow || nCol>(int)m_nCol)
		return INFINITE_VALUE;
	while (pCur!=NULL && !(pCur->row==nRow && pCur->col==nCol))
		pCur = pCur->next;
	if (pCur == NULL)
		return INFINITE_VALUE;
	return pCur->value;
}

PARRAY_CHAIN CDynamicArray::GetHead()
{
	return m_pArrayHead;
}

void CDynamicArray::SetEmpty()
{
	PARRAY_CHAIN pCur, pTemp;
	pCur = m_pArrayHead;
	while (pCur != NULL)
	{
		pTemp = pCur;
		pCur = pCur->next;
		if (pTemp->sWord!=NULL)
			delete [] pTemp->sWord;
		 free(pTemp);
	}
	m_pArrayHead = NULL;
	m_nCol = 0;
	m_nRow = 0;
}

CRowFirstDynamicArray::CRowFirstDynamicArray()
{
	m_nRow = 0;
	m_nCol = 0;
	m_pArrayHead = NULL;
}

CColFirstDynamicArray::CColFirstDynamicArray()
{
	m_nRow = 0;
	m_nCol = 0;
	m_pArrayHead = NULL;
}

ELEMENT_TYPE CRowFirstDynamicArray::GetFirstElementOfRow(int nRow, PARRAY_CHAIN pStart, PARRAY_CHAIN *pRet)
{
	PARRAY_CHAIN pCur = pStart;
	if (pStart == 0)
		pCur = m_pArrayHead;
	while (pCur!=NULL && pCur->row!=nRow)
		pCur = pCur->next;
	if (pRet!=0)
		*pRet = pCur;
	if (pCur == NULL)
		return INFINITE_VALUE;
	return pCur->value;
}

ELEMENT_TYPE CColFirstDynamicArray::GetFirstElementOfCol(int nCol, PARRAY_CHAIN pStart, PARRAY_CHAIN *pRet)
{
	PARRAY_CHAIN pCur = pStart;
	if (pStart == 0)
		pCur = m_pArrayHead;
	while (pCur!=NULL && pCur->col!=nCol)
		pCur = pCur->next;
	if (pRet!=0)
		*pRet = pCur;
	if (pCur == NULL)
		return INFINITE_VALUE;
	return pCur->value;

}

void CRowFirstDynamicArray::SetElement(int nRow, int nCol, ELEMENT_TYPE value, int nPos, char *sWord)
{
	PARRAY_CHAIN pCur = m_pArrayHead, pPre = 0, pAdd;
	if (nRow>(int)m_nRow)
		m_nRow = nRow;
	if (nCol>(int)m_nCol)
		m_nCol = nCol;
	while (pCur!=NULL && ((int)pCur->row<nRow || ((int)pCur->row==nRow&&(int)pCur->col<nCol)))
	{
		pPre = pCur;
		pCur = pCur->next;
	}
	if (pCur!=NULL && (int)pCur->row==nRow && (int)pCur->col==nCol)  //searched the element
	{	
		pCur->value = value;
		pCur->nPos = nPos;
	}
	else //new a element
	{
		pAdd = (PARRAY_CHAIN)malloc(sizeof(ARRAY_CHAIN));
		pAdd->row = nRow;
		pAdd->col = nCol;
		pAdd->value = value;
		pAdd->nPos = nPos;

		if (sWord!=0)
		{
			pAdd->nWordLenght = strlen(sWord);  //strlen don't count the end bit \0
			pAdd->sWord = new char[pAdd->nWordLenght+1];
			strcpy(pAdd->sWord, sWord);
		}
		else
		{
			pAdd->nWordLenght = 0;
			pAdd->sWord = NULL;
		}
		pAdd->next = pCur;

		if (pPre==NULL)
			m_pArrayHead = pAdd;
		else
			pPre->next = pAdd;
	}
}

void CColFirstDynamicArray::SetElement(int nRow, int nCol, double value, int nPos, char *sWord)
{
	PARRAY_CHAIN pPre=0, pCur=m_pArrayHead, pAdd;
	if (nRow>(int)m_nRow)
		m_nRow = nRow;
	if (nCol>(int)m_nCol)
		m_nCol = nCol;
	while (pCur!=NULL && ((int)pCur->col<nCol || ((int)pCur->col==nCol&&(int)pCur->row<nRow)))
	{
		pPre = pCur;
		pCur = pCur->next;
	}
	if(pCur!=NULL && (int)pCur->col==nCol && (int)pCur->row==nRow)
	{
		pCur->value = value;
		pCur->nPos = nPos;
	}
	else
	{
		pAdd = (PARRAY_CHAIN)malloc(sizeof(ARRAY_CHAIN));
		pAdd->row = nRow;
		pAdd->col = nCol;
		pAdd->value = value;
		pAdd->nPos = nPos;

		if (sWord!=0)
		{
			pAdd->nWordLenght = strlen(sWord);  //strlen don't count the end bit \0
			pAdd->sWord = new char[pAdd->nWordLenght+1];
			strcpy(pAdd->sWord, sWord);
		}
		else
		{
			pAdd->nWordLenght = 0;
			pAdd->sWord = NULL;
		}
		pAdd->next = pCur;

		if (pPre==NULL)
			m_pArrayHead = pAdd;
		else
			pPre->next = pAdd;
	}
}



