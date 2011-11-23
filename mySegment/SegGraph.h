#ifndef SEG_GRAPH_H
#define SEG_GRAPH_H

#include "Dictionary.h"
#include "DynamicArray.h"

#define MAX_SENTENCE_LEN 2000
#define MAX_FREQUENCE 2079997

class CSegGraph
{
public:
	CSegGraph();
	~CSegGraph();
	void DoSegment(char *sSentence, CDictionary &dictCore, CDictionary &dictBinary, unsigned int nResultCount);
	void AtomSegment(char *sSentence);
	void GenerateWordNet(char *sSentence, CDictionary &dict);
	void BiGraphGenerate(CRowFirstDynamicArray &aWord, CColFirstDynamicArray &aBinaryWordNet, double dSmoothingPara, CDictionary &dictCore, CDictionary &dictBinary);
	CRowFirstDynamicArray m_segGraph;
	//int BinarySearch(int nValue, int *nTable, int nCount);
	char m_sAtomChar[MAX_SENTENCE_LEN][MAX_WORD_LEN];
	int m_nAtomLen[MAX_SENTENCE_LEN];
	int m_nAtomPos[MAX_SENTENCE_LEN];
	int m_nAtomCount;  //原子数量
	int m_nWordCount;  //词的数量，即m_segGraph的大小
	int *m_npWordPosMapTable;
	
private:
	
};

#endif