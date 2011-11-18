#include "Dictionary.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

CDictionary::CDictionary()
{
	memset(m_IndexTable, 0, sizeof(m_IndexTable));
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