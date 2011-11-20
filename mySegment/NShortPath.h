#ifndef NSHORT_PATH_H
#define NSHORT_PATH_H

#include "DynamicArray.h"
#include "Queue.h"

class CNShortPath{
public:
	CNShortPath(CDynamicArray *pArray, unsigned int nValueKind);
	~CNShortPath();
	void ShortPath();
	void GetPaths(unsigned int nNode, unsigned int nKindIndex, unsigned int **nResult);
private:
	unsigned int m_nVertex;
	unsigned int m_nValueKind;
	CDynamicArray *m_pArray;
	ELEMENT_TYPE **m_pWeight;
	CQueue **m_pParent;
};

#endif