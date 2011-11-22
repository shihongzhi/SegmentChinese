#include "Dictionary.h"
#include "Utility.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>


CDictionary::CDictionary()
{
	memset(m_IndexTable, 0, sizeof(m_IndexTable));
	m_pModifyTable = NULL;
}

CDictionary::~CDictionary()
{
	for (int i=0; i<CC_NUM; ++i)
	{
		for (int j=0; j<m_IndexTable[i].nCount; ++j)
		{
			delete [] m_IndexTable[i].pWordItemHead[j].sWord;
		}
		delete [] m_IndexTable[i].pWordItemHead;
	}
}

bool CDictionary::Load(char *sFilename, bool bReset)
{
	FILE *fp;
	if ((fp=fopen(sFilename,"rb")) == NULL)
		return false;
	int i,j,nBuffer[3];
	//release memory
	for (i=0; i<CC_NUM; ++i)
	{
		for (int j=0; j<m_IndexTable[i].nCount; ++j)
		{
			delete [] m_IndexTable[i].pWordItemHead[j].sWord;
		}
		delete [] m_IndexTable[i].pWordItemHead;
	}
	DelModifyTable();
	for (i=0; i<CC_NUM; ++i)
	{
		fread(&(m_IndexTable[i].nCount), sizeof(int), 1, fp);
		if (m_IndexTable[i].nCount > 0)
			m_IndexTable[i].pWordItemHead = new WORD_ITEM[m_IndexTable[i].nCount];
		else
		{
			m_IndexTable[i].pWordItemHead = 0;
			continue;
		}
		for (j=0; j<m_IndexTable[i].nCount; ++j)
		{
			fread(nBuffer, sizeof(int), 3, fp);
			m_IndexTable[i].pWordItemHead[j].sWord = new char[nBuffer[1]+1];
			if (nBuffer[1] > 0)
			{
				fread(m_IndexTable[i].pWordItemHead[j].sWord, sizeof(char), nBuffer[1], fp);
			}
			m_IndexTable[i].pWordItemHead[j].sWord[nBuffer[1]] = 0;
			if(bReset)
				m_IndexTable[i].pWordItemHead[j].nFrequency = 0;
			else
				m_IndexTable[i].pWordItemHead[j].nFrequency = nBuffer[0];
			m_IndexTable[i].pWordItemHead[j].nWordLength = nBuffer[1];
			m_IndexTable[i].pWordItemHead[j].nHandle = nBuffer[2];
		}
	}
	fclose(fp);
	return true;
}

bool CDictionary::DelModifyTable()
{
	PWORD_CHAIN pCurr,pTemp;
	if (!m_pModifyTable)
		return true;
	for (int i=0; i<CC_NUM; ++i)
	{
		pCurr = m_pModifyTable[i].pWordItemHead;
		while (pCurr != NULL)
		{
			pTemp = pCurr;
			pCurr = pCurr->next;
			delete [] pTemp->data.sWord;  //应该要加[]的吧
			delete pTemp;
		}
	}
	delete [] m_pModifyTable;
	m_pModifyTable = NULL;
	return true;
}

bool CDictionary::FindInOriginalTable(int nInnerCode, char *sWord, int nHandle)
{
	PWORD_ITEM pItems = m_IndexTable[nInnerCode].pWordItemHead;
	int nStart, nEnd, nMid, nCmpValue;
	nStart =0;
	nEnd = m_IndexTable[nInnerCode].nCount - 1;
	nMid = (nStart + nEnd) / 2;
	while (nStart <= nEnd)
	{
		nCmpValue = strcmp(pItems[nMid].sWord, sWord);
		if (nCmpValue==0 && (pItems[nMid].nHandle==nHandle || nHandle == -1))
			return true;
		else if (nCmpValue<0 || (nCmpValue==0 && pItems[nMid].nHandle<nHandle && nHandle!=-1))
			nStart = nMid + 1;
		else if (nCmpValue>0 || (nCmpValue==0 && pItems[nMid].nHandle>nHandle && nHandle!=-1))
			nEnd = nMid -1;
		nMid = (nStart + nEnd) / 2;
	}
	return false;
}

bool CDictionary::FindInOriginalTable(int nInnerCode, char *sWord, int nHandle, int *nPosRet)
{
	PWORD_ITEM pItems = m_IndexTable[nInnerCode].pWordItemHead;
	int nStart, nEnd, nMid, nCmpValue;
	nStart =0;
	nEnd = m_IndexTable[nInnerCode].nCount - 1;
	nMid = (nStart + nEnd) / 2;
	while (nStart <= nEnd)
	{
		nCmpValue = strcmp(pItems[nMid].sWord, sWord);
		if (nCmpValue==0 && (pItems[nMid].nHandle==nHandle || nHandle == -1))
		{
			if (nHandle == -1)  //not strict match, then search the first match sWord without consider the nHandle
			{
				nMid--;
				while (nMid>=0 && strcmp(pItems[nMid].sWord, sWord)==0)
					nMid--;
				if (nMid<0 || strcmp(pItems[nMid].sWord, sWord)!=0)
					nMid++;
			}
			*nPosRet = nMid;
			return true;
		}
		else if (nCmpValue<0 || (nCmpValue==0 && pItems[nMid].nHandle<nHandle && nHandle!=-1))
			nStart = nMid + 1;
		else if (nCmpValue>0 || (nCmpValue==0 && pItems[nMid].nHandle>nHandle && nHandle!=-1))
			nEnd = nMid -1;
		nMid = (nStart + nEnd) / 2;
	}
	*nPosRet = nMid - 1;
	return false;
}

bool CDictionary::PreProcess(char *sWord, int *nInnerCode, char *sWordRet)
{
	int nLen, nBegin, nEnd, nType;
	nType = charType((unsigned char *)sWord);
	//去除sWord的trim
	nLen = strlen(sWord);
	nBegin = 0;
	nEnd = nLen - 1;
	if (nLen==0)
		return false;
	while (nBegin<nLen && sWord[nBegin]==' ')
		nBegin++;
	while (nEnd>=0 && sWord[nEnd]==' ')
		nEnd--;
	if (nBegin>nEnd)
		return false;
	if (nEnd!=nLen-1 || nBegin!=0)
	{
		strncpy(sWord, sWord+nBegin, nEnd-nBegin+1);
		sWord[nEnd-nBegin+1] = 0;
	}

	if (nType == CT_CHINESE)
	{
		*nInnerCode = CC_ID(sWord[0], sWord[1]);
		strcpy(sWordRet, &sWord[2]);
		return true;
	}
	
	if (nType == CT_DELIMITER)
	{
		*nInnerCode = 3755;
		strcpy(sWordRet, sWord);
		return true;
	}
	return false;
}

bool CDictionary::GetMaxMatch(char *sWord, char *sWordRet)
{
	char sWordGet[MAX_WORD_LEN-2], sFirstChar[3];
	int nInnerCode, nFoundPos, i;

	if (!PreProcess(sWord, &nInnerCode, sWordGet))
		return false;
	//设置sFirstChar
	strncpy(sFirstChar, sWord, strlen(sWord)-strlen(sWordGet));
	sFirstChar[strlen(sWord)-strlen(sWordGet)] = 0;

	//FindInOriginalTable(nInnerCode, sWordGet, -1, &nFoundPos);
	i = 0;
	while (i<m_IndexTable[nInnerCode].nCount&&CC_Find(m_IndexTable[nInnerCode].pWordItemHead[i].sWord, sWordGet)!=m_IndexTable[nInnerCode].pWordItemHead[i].sWord)
		i++;
	if (i<m_IndexTable[nInnerCode].nCount&&CC_Find(m_IndexTable[nInnerCode].pWordItemHead[i].sWord, sWordGet)==m_IndexTable[nInnerCode].pWordItemHead[i].sWord)
	{
		strcpy(sWordRet, sFirstChar);
		strcat(sWordRet, m_IndexTable[nInnerCode].pWordItemHead[i].sWord);
		return true;
	}
	return false;
}

bool CDictionary::GetHandle(char *sWord, int *nMatchCount, int *nMatchHandle, int *nMatchFreq)
{
	char sWordGet[MAX_WORD_LEN-2];
	int nInnerCode, nFoundPos, nFoundCur;

	if (!PreProcess(sWord, &nInnerCode, sWordGet))
		return false;

	*nMatchCount = 0;
	if (FindInOriginalTable(nInnerCode, sWordGet, -1, &nFoundPos))
	{
		nMatchHandle[*nMatchCount] = m_IndexTable[nInnerCode].pWordItemHead[nFoundPos].nHandle;
		nMatchFreq[*nMatchCount] = m_IndexTable[nInnerCode].pWordItemHead[nFoundPos].nFrequency;
		(*nMatchCount)++;
		nFoundCur = nFoundPos + 1;
		while (nFoundCur<m_IndexTable[nInnerCode].nCount && strcmp(sWordGet, m_IndexTable[nInnerCode].pWordItemHead[nFoundCur].sWord)==0)
		{
			
			nMatchHandle[*nMatchCount] = m_IndexTable[nInnerCode].pWordItemHead[nFoundCur].nHandle;
			nMatchFreq[*nMatchCount] = m_IndexTable[nInnerCode].pWordItemHead[nFoundCur].nFrequency;
			nFoundCur++;
			(*nMatchCount)++;
		}
		return true;
	}
	return false;
}

//找不到就为0 ，不能为-1或者其他，因为BiGraphGenerate中需要计算dValue的，即eWeight
int CDictionary::GetFrequency(char *sWord, int nHandle)
{
	char sWordGet[MAX_WORD_LEN-2];
	int nInnerCode, nPosRet;

	if(!PreProcess(sWord, &nInnerCode, sWordGet))
		return 0;

	if(FindInOriginalTable(nInnerCode, sWordGet, nHandle, &nPosRet))
	{
		return m_IndexTable[nInnerCode].pWordItemHead[nPosRet].nFrequency;
	}
	return 0;
}