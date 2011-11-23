#include "Dictionary.h"
#include "SegGraph.h"
#include <stdio.h>

int main()
{
	CDictionary dictCore, dictBinary;
	printf("��ʼ��Dictionary......\n\n");
	dictCore.Load("Data/coreDict.dct");
	dictBinary.Load("Data/BigramDict.dct");
	//for (int i=0; i<CC_NUM; ++i)
	//{
	//	for (int j=0; j<dict.m_IndexTable[i].nCount; ++j)
	//	{
	//		printf("%d\t%d\t%s\n",dict.m_IndexTable[i].pWordItemHead[j].nFrequency, dict.m_IndexTable[i].pWordItemHead[j].nWordLength,dict.m_IndexTable[i].pWordItemHead[j].sWord);
	//	}
	//	printf("\n");
	//}
	//char testChar[] = "ʼ##ʼ����ƽ��1�·����ϴ����˵��ȷʵ����ĩ##ĩ";
	char testChar[] = "ʼ##ʼ��˵��ȷʵ����ĩ##ĩ";
	printf("=====ԭʼ����=====:\n\n");
	printf("%s\n\n",testChar);
	CSegGraph seg;
	//seg.AtomSegment(testChar);
	seg.DoSegment(testChar, dictCore, dictBinary, 10);

	getchar();
	return 0;
}