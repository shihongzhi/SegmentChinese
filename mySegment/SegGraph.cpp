#include "SegGraph.h"
#include "Utility.h"
#include "NShortPath.h"
#include <math.h>

CSegGraph::CSegGraph()
{
	m_npWordPosMapTable = NULL;
}

CSegGraph::~CSegGraph()
{

}

void CSegGraph::DoSegment(char *sSentence, CDictionary &dictCore, CDictionary &dictBinary, unsigned int nResultCount)
{
	//原子切分
	AtomSegment(sSentence);

	//生成segGraph
	GenerateWordNet(sSentence, dictCore);
	
	//生成BiSegGraph
	CColFirstDynamicArray tempBiGraph;
	BiGraphGenerate(m_segGraph, tempBiGraph, 0.1, dictCore, dictBinary);

	int **nSegRoute;//The segmentation route
	nSegRoute=new int*[MAX_SEGMENT_NUM];
	unsigned int nLen=strlen(sSentence)+10;
	for(int i=0;i<MAX_SEGMENT_NUM;i++)
	{
		nSegRoute[i]=new int[nLen/2];
		memset(nSegRoute[i],-1,nLen/2*sizeof(int));
	}
	int nCount;
	CNShortPath sp(&tempBiGraph, nResultCount);
	sp.ShortPath();
	sp.Output(nSegRoute, &nCount);

	//test printf;
	printf("=====NShortPath优化=====：\n");
	int testCount;
	PARRAY_CHAIN pTestCur;
	for (int i=0; i<MAX_SEGMENT_NUM; ++i)
	{
		pTestCur = m_segGraph.GetHead();
		testCount = 0;
		for (int j=0; j<nLen/2&&nSegRoute[i][j]!=-1; ++j)
		{
			while (testCount < nSegRoute[i][j])
			{
				testCount++;
				pTestCur = pTestCur->next;
			}
			printf("%s,", pTestCur->sWord);
		}
		printf("\n");
	}

}

//原子分词
void CSegGraph::AtomSegment(char *sSentence)
{
	int i=0,j=0, nCurType, nNextType;
	char sChar[3];
	sChar[2] = 0;
	m_sAtomChar[j][0] = 0;
	m_nAtomLen[j] = 0;
	if (strncmp(sSentence, SENTENCE_BEGIN, strlen(SENTENCE_BEGIN))==0)
	{
		strcpy(m_sAtomChar[j], SENTENCE_BEGIN);
		m_nAtomLen[j] = strlen(SENTENCE_BEGIN);
		m_nAtomPos[j] = CT_SENTENCE_BEGIN;
		j++;
		i += strlen(SENTENCE_BEGIN);
		m_sAtomChar[j][0] = 0;
		m_nAtomLen[j] = 0;
	}
	while (i<strlen(sSentence))
	{
		if (strncmp(sSentence+i, SENTENCE_END, strlen(SENTENCE_END))==0)
		{
			strcpy(m_sAtomChar[j], SENTENCE_END);
			m_nAtomLen[j] = strlen(SENTENCE_END);
			m_nAtomPos[j] = CT_SENTENCE_END;
			j++;
			i += strlen(SENTENCE_END);
			m_sAtomChar[j][0] = 0;
			m_nAtomLen[j] = 0;
			break;  //结束了，不管sSentence有没有结束，遇到SENTENCE_END就表示结束了吧
		}
		sChar[0] = sSentence[i++];
		sChar[1] = 0;
		if (sChar[0] < 0)  //Two byte char
			sChar[1] = sSentence[i++];
		strcat(m_sAtomChar[j], sChar);
		nCurType = charType((unsigned char *)sChar);
		//如果"."后面跟着的是数字，那么"."也是数字
		if (sChar[0]=='.' && (charType((unsigned char *)(sSentence+i))==CT_NUM || ((*(sSentence+i))>'0'&&(*(sSentence+i))<'9')))
			nCurType = CT_NUM;

		m_nAtomPos[j] = nCurType;
		if (nCurType==CT_DELIMITER || nCurType==CT_CHINESE || nCurType==CT_INDEX || nCurType==CT_OTHER)
		{
			m_nAtomLen[j] = strlen(m_sAtomChar[j]);
			j++;
			m_sAtomChar[j][0] = 0;  //这个因为strcat的原因，所以需要init the end char
			m_nAtomLen[j] = 0;
		}
		else
		{
			nNextType = -1;
			if (i<strlen(sSentence))
				nNextType = charType((unsigned char *)(sSentence+i));
			if (nNextType!=nCurType || i>=strlen(sSentence))
			{
				m_nAtomLen[j] = strlen(m_sAtomChar[j]);
				j++;
				m_sAtomChar[j][0] = 0;  //这个因为strcat的原因，所以需要init the end char
				m_nAtomLen[j] = 0;
			}
		}
	}
	m_nAtomCount = j;

	//test printf
	printf("=====原子切分=====:\n\n");
	for (i=0; i<m_nAtomCount; i++)
	{
		printf("%s,", m_sAtomChar[i]);
	}
	printf("\n\n");
}

void CSegGraph::GenerateWordNet(char *sSentence, CDictionary &dict)
{
	int i, j, k, nPos;
	int nMatchFreq[20], nMatchHandle[20], nTotalFreq, nMatchCount;
	char sWord[MAX_WORD_LEN], sWordMatch[MAX_WORD_LEN];
	double dValue;
	//AtomSegment(sSentence);

	for (i=0; i<m_nAtomCount; ++i)
	{
		if (m_nAtomPos[i] == CT_CHINESE)
			m_segGraph.SetElement(i, i+1, 0, 0, m_sAtomChar[i]);
		else  //这块不懂.....
		{
			strcpy(sWord, m_sAtomChar[i]);
			dValue = MAX_FREQUENCE;
			switch (m_nAtomPos[i])
			{
			case CT_INDEX:
			case CT_NUM:
				strcpy(sWord, "未##数");
				dValue = 0;
				nPos = -27904;
				break;
			case CT_DELIMITER:
				nPos = 30464;
				break;
			case CT_LETTER:
				nPos=-'n'*256-'x';
				dValue = 0;
				strcpy(sWord, "未##串");
				break;
			case CT_SINGLE:
				if (GetCharCount("+-1234567890", m_sAtomChar[i])==(int)strlen(m_sAtomChar[i]))
				{
					nPos = -27904;
					strcpy(sWord, "未##数");
				}
				else
				{
					nPos=-'n'*256-'x';//
					strcpy(sWord,"未##串");
				}
				dValue = 0;
				break;
			default:
				nPos = m_nAtomPos[i];
				break;
			}
			m_segGraph.SetElement(i, i+1, dValue, nPos, sWord);
		}
	}

	i = 0;
	while (i<m_nAtomCount)
	{
		strcpy(sWord, m_sAtomChar[i]);
		j = i + 1;
		//月份特殊处理
		if (m_sAtomChar[i]=="月" && m_sAtomChar[i+1]=="份")
			j++;

		while (j<m_nAtomCount && dict.GetMaxMatch(sWord, sWordMatch))
		{
			if (strcmp(sWord, sWordMatch)==0)
			{
				//Adding a rule to exclude some words to be formed.
				if(strlen(sWord)==4&&i>=1&&(IsAllNum((unsigned char *)m_sAtomChar[i-1])||IsAllChineseNum(m_sAtomChar[i-1]))&&(strncmp(sWord,"年",2)==0||strncmp(sWord,"月",2)==0))
				{//1年内、1999年末
					if(CC_Find("末内中底前间初",sWord+2))
						break;
				}

				dict.GetHandle(sWord, &nMatchCount, nMatchHandle, nMatchFreq);
				
				nTotalFreq = 0;
				for (k=0; k<nMatchCount; ++k)
				{
					nTotalFreq += nMatchFreq[k];
				}

				if (nMatchCount==1)
					m_segGraph.SetElement(i, j, nTotalFreq, nMatchHandle[0], sWord);
				else
					m_segGraph.SetElement(i, j, nTotalFreq, 0, sWord);

			}
			strcat(sWord, m_sAtomChar[j++]);
		}
		i++;
	}

	//test printf
	printf("=====生成SegGraph=====:\n\n");
	PARRAY_CHAIN testCur = m_segGraph.GetHead();
	while (testCur!=NULL)
	{
		printf("row: %d\tcol: %d\teWeight: \t%.2lf\tnPos: %d\tsWord: %s\n", testCur->row, testCur->col, testCur->value, testCur->nPos, testCur->sWord);
		testCur = testCur->next;
	}
	printf("\n\n");
}

//BiSegFraph的生成
void CSegGraph::BiGraphGenerate(CRowFirstDynamicArray &aWord, CColFirstDynamicArray &aBinaryWordNet, double dSmoothingPara, CDictionary &dictCore, CDictionary &dictBinary)
{
	PARRAY_CHAIN pTail, pCur, pNextWords;
	int nWordIndex, nTwoWordsFreq, nCurWordIndex, nNextWordIndex;
	double dTemp, dValue, dCurFreq;
	char sTwoWords[MAX_WORD_LEN];

	m_nWordCount = aWord.GetTail(&pTail);
	
	if (m_npWordPosMapTable)
	{
		delete [] m_npWordPosMapTable;
		m_npWordPosMapTable = 0;
	}
	m_npWordPosMapTable = new int[m_nWordCount];
	nWordIndex = 0;
	pCur = aWord.GetHead();
	while (pCur!=NULL)
	{
		m_npWordPosMapTable[nWordIndex++] = pCur->row * MAX_SENTENCE_LEN + pCur->col;
		pCur = pCur->next;
	}

	pCur = aWord.GetHead();
	while (pCur!=NULL)
	{
		if (pCur->nPos>=0)
			dCurFreq = pCur->value;
		else
			dCurFreq = dictCore.GetFrequency(pCur->sWord, 2);

		aWord.GetFirstElementOfRow(pCur->col, pCur, &pNextWords);
		while (pNextWords!=NULL&&pNextWords->row==pCur->col)
		{
			strcpy(sTwoWords, pCur->sWord);
			strcat(sTwoWords, WORD_SEGMENTER);
			strcat(sTwoWords, pNextWords->sWord);
			nTwoWordsFreq = dictBinary.GetFrequency(sTwoWords, 3);

			//计算dValue,也就是权重 weight
			//Two linked Words frequency
			dTemp = (double)1/MAX_FREQUENCE;
			//Smoothing
			dValue = -log(dSmoothingPara*(1+dCurFreq)/(MAX_FREQUENCE+80000)+(1-dSmoothingPara)*((1-dTemp)*nTwoWordsFreq/(1+dCurFreq)+dTemp));
			//-log{a*P(Ci-1)+(1-a)P(Ci|Ci-1)} Note 0<a<1
			if(pCur->nPos<0)//Unknown words: P(Wi|Ci);while known words:1
				dValue += pCur->value;
		
			nCurWordIndex = BinarySearch(pCur->row*MAX_SENTENCE_LEN+pCur->col, m_npWordPosMapTable, m_nWordCount);
			nNextWordIndex = BinarySearch(pNextWords->row*MAX_SENTENCE_LEN+pNextWords->col, m_npWordPosMapTable, m_nWordCount);
			aBinaryWordNet.SetElement(nCurWordIndex, nNextWordIndex, dValue, pCur->nPos, sTwoWords);
			
			pNextWords = pNextWords->next;
		}
		pCur = pCur->next;
	}

	//test printf
	printf("=====生成SegGraph=====:\n\n");
	PARRAY_CHAIN testCur = aBinaryWordNet.GetHead();
	while (testCur!=NULL)
	{
		printf("row: %d\tcol: %d\teWeight: \t%.2lf\tnPos: %d\tsWord: %s\n", testCur->row, testCur->col, testCur->value, testCur->nPos, testCur->sWord);
		testCur = testCur->next;
	}
	printf("\n\n");
}

//int CSegGraph::BinarySearch(int nValue, int *nTable, int nCount)
//{
//	int nBegin=0, nEnd=nCount-1, nMid;
//	nMid = (nBegin + nEnd) / 2;
//	while (nBegin<=nEnd)
//	{
//		if (nTable[nMid] == nValue)
//			return nMid;
//		else if (nTable[nMid]<nValue)
//			nBegin = nMid + 1;
//		else
//			nEnd = nMid - 1;
//		nMid = (nBegin + nEnd) / 2;
//	}
//	return -1;
//}