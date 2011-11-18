#include "Dictionary.h"
#include <stdio.h>

int main()
{
	CDictionary dict;
	dict.Load("Data/coreDict.dct");
	for (int i=0; i<CC_NUM; ++i)
	{
		for (int j=0; j<dict.m_IndexTable[i].nCount; ++j)
		{
			printf("%d\t%d\t%s\n",dict.m_IndexTable[i].pWordItemHead[j].nFrequency, dict.m_IndexTable[i].pWordItemHead[j].nWordLength,dict.m_IndexTable[i].pWordItemHead[j].sWord);
		}
		printf("\n");
	}
	return 0;
}