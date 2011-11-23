#ifndef NSHORT_PATH_H
#define NSHORT_PATH_H

#include "DynamicArray.h"
#include "Queue.h"

#define MAX_SEGMENT_NUM 10

class CNShortPath{
public:
	CNShortPath(CColFirstDynamicArray *pArray, unsigned int nValueKind);
	~CNShortPath();
	void ShortPath();
	void GetPaths(unsigned int nNode, unsigned int nKindIndex, int **nResult);
	void Output(int **nResult, int *nCount);
private:
	unsigned int m_nVertex;
	unsigned int m_nValueKind;
	CColFirstDynamicArray *m_pArray;
	ELEMENT_TYPE **m_pWeight;
	CQueue **m_pParent;
	int m_nResultCount;
};

#endif