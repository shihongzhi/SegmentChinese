#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#define ELEMENT_TYPE double
#define INFINITE_VALUE 10000.00

struct tagArrayChain{
	unsigned int row,col;
	int nPos;
	int nWordLenght;
	ELEMENT_TYPE value;  //weight
	char *sWord;
	struct tagArrayChain *next;
};
typedef struct tagArrayChain ARRAY_CHAIN,*PARRAY_CHAIN;

class CDynamicArray
{
public:
	CDynamicArray();
	virtual ~CDynamicArray();
	ELEMENT_TYPE GetElement(int nRow, int nCol);
	virtual void SetElement(int nRow, int nCol, ELEMENT_TYPE value, int nPos, char *sWord=0)=0;
	PARRAY_CHAIN GetHead();
	unsigned int GetTail(PARRAY_CHAIN *pTailRet);
	void SetEmpty();
	unsigned int m_nRow,m_nCol;
protected:
	PARRAY_CHAIN m_pArrayHead;
};

class CRowFirstDynamicArray : public CDynamicArray{
public:
	CRowFirstDynamicArray();
	~CRowFirstDynamicArray(){};
	ELEMENT_TYPE GetFirstElementOfRow(int nRow, PARRAY_CHAIN pStart=0, PARRAY_CHAIN *pRet=0);
	virtual void SetElement(int nRow, int nCol, ELEMENT_TYPE value, int nPos, char *sWord=0);
};

class CColFirstDynamicArray : public CDynamicArray{
public:
	CColFirstDynamicArray();
	~CColFirstDynamicArray(){};
	ELEMENT_TYPE GetFirstElementOfCol(int nCol, PARRAY_CHAIN pStart=0, PARRAY_CHAIN *pRet=0);
	virtual void SetElement(int nRow, int nCol, ELEMENT_TYPE value, int nPos, char *sWord=0);
};

#endif