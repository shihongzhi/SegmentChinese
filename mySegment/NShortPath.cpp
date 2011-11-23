#include "NShortPath.h"

CNShortPath::CNShortPath(CColFirstDynamicArray *pArray, unsigned int nValueKind)
{
	m_pArray = pArray;
	m_nVertex = m_pArray->m_nCol + 1;
	m_nValueKind = nValueKind;
	if (m_nVertex < m_pArray->m_nRow+1)
		m_nVertex = m_pArray->m_nRow + 1;
	m_pParent = new CQueue*[m_nVertex-1];
	m_pWeight = new ELEMENT_TYPE*[m_nVertex-1];

	for (unsigned int i=0; i<m_nVertex-1; ++i)
	{
		m_pParent[i] = new CQueue[nValueKind];
		m_pWeight[i] = new ELEMENT_TYPE[nValueKind];
	}
}

CNShortPath::~CNShortPath()
{
	for (unsigned int i=0; i<m_nVertex-1; ++i)
	{
		delete [] m_pParent[i];
		delete [] m_pWeight[i];
	}
	delete [] m_pParent;
	delete [] m_pWeight;
}

void CNShortPath::ShortPath()
{
	unsigned int nCur=1, nPre, i, nIndex;
	ELEMENT_TYPE eWeight;
	PARRAY_CHAIN pColList;

	for (; nCur<m_nVertex; ++nCur)
	{
		CQueue qParent;
		eWeight = m_pArray->GetFirstElementOfCol(nCur, 0, &pColList);
		while (pColList!=NULL && pColList->col==nCur)
		{
			nPre = pColList->row;
			eWeight = pColList->value;
			for (i=0; i<m_nValueKind; ++i)
			{
				if (nPre>0)
				{
					//如果到nPre节点处时weight为INFINITE_VALUE，则说明此节点不可达 
					if (m_pWeight[nPre-1][i] == INFINITE_VALUE)  
						break;
					qParent.Push(nPre, i, eWeight+m_pWeight[nPre-1][i]);
				}
				else
				{
					qParent.Push(nPre, i, eWeight);
					break;  //这里需要break的，因为nPre=0时只有一种情况，而不是所有i都需要push的
				}
			}
			pColList = pColList->next;
		}


		for (i=0; i<m_nValueKind; ++i)
			m_pWeight[nCur-1][i] = INFINITE_VALUE;
		i = 0;
		while (i<m_nValueKind && qParent.Pop(&nPre, &nIndex, &eWeight)!=-1)
		{
			if (m_pWeight[nCur-1][i] == INFINITE_VALUE)
			{
				m_pWeight[nCur-1][i] = eWeight;
			} 
			else if(m_pWeight[nCur-1][i] < eWeight)
			{
				i++;
				if (i==m_nValueKind)
					break;
				m_pWeight[nCur-1][i] = eWeight;
			}
			m_pParent[nCur-1][i].Push(nPre, nIndex);  //只需要直接push就行了，所以eWeight=0
		}
	}
}

void CNShortPath::GetPaths(unsigned int nNode, unsigned int nKindIndex, int **nResult)
{
	CQueue stack;
	unsigned int nCurNode, nCurIndex, nParentNode, nParentIndex, nResultIndex=0;
	int nResultLenght;
	stack.Push(nNode, nKindIndex);
	nCurNode = nNode;
	nCurIndex = nKindIndex;
	//nResult[nResultCount][nResultIndex] = -1;
	while (!stack.isEmpty())
	{
		//通过压栈得到路径
		while (nCurNode>0)
		{
			if (m_pParent[nCurNode-1][nCurIndex].GetFirst(&nParentNode, &nParentIndex, 0)!=-1)
			{
				nCurNode = nParentNode;
				nCurIndex = nParentIndex;
			}
			if (nCurNode>0)
				stack.Push(nCurNode, nCurIndex);
		}
		//Set the nResult data
		if (nCurNode==0)
		{
			nResult[m_nResultCount][nResultIndex++] = nCurNode;
			nResultLenght = 0;
			stack.OutputElement(&nResult[m_nResultCount][nResultIndex], &nResultLenght);
			nResult[m_nResultCount][nResultLenght+1] = -1; //Set the end;

			m_nResultCount++;
			if (m_nResultCount>=MAX_SEGMENT_NUM)
				return ;
			nResultIndex = 0;	
		}
		//出栈以检查是否还有其它路径
		do 
		{
			stack.Pop(&nCurNode, &nCurIndex);
		} while (nCurNode<1 || (!stack.isEmpty() && m_pParent[nCurNode-1][nCurIndex].isSingle()));
		if (!m_pParent[nCurNode-1][nCurIndex].isSingle() && m_pParent[nCurNode-1][nCurIndex].Pop(&nParentNode, &nParentIndex)!=-1)
		{
			nCurNode = nParentNode;
			nCurIndex = nParentIndex;
		}
		else
			break;
	}
}


void CNShortPath::Output(int **nResult, int *nCount)
{
	if (m_nVertex<2)
	{
		nResult[0][1] = 0;
		*nCount = 1;
		return ;
	}
	*nCount = 0;
	m_nResultCount = 0;
	for (int i=0; i<m_nValueKind&&m_pWeight[m_nVertex-2][i]<INFINITE_VALUE; ++i)
	{
		GetPaths(m_nVertex-1, i, nResult);
		*nCount = m_nResultCount;
		if (m_nResultCount>=MAX_SEGMENT_NUM)
			return ;
	}
}
