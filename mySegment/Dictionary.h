#ifndef DICTINARY_H
#define DICTINARY_H

#define CC_NUM 6768
#define MAX_WORD_LEN 100

#define CC_ID(c1,c2) ((unsigned char)(c1)-176)*94+((unsigned char)(c2)-161)

struct tagWordResult{
	char sWord[MAX_WORD_LEN];
	int nHandle;
	double dValue;
};
typedef struct tagWordResult WORD_RESULT,*PWORD_RESULT;

struct tagWordItem{
	int nWordLength;
	int nFrequency;
	int nHandle;
	char *sWord;
};
typedef struct tagWordItem WORD_ITEM,*PWORD_ITEM;

struct tagIndexItem{
	int nCount;
	PWORD_ITEM pWordItemHead;
};
typedef struct tagIndexItem INDEX_ITEM;

struct tagWordChain{
	WORD_ITEM data;
	struct tagWordChain *next;
};
typedef struct tagWordChain WORD_CHAIN,*PWORD_CHAIN;

struct tagModifyTable{
	int nCount;
	int nDelete;
	PWORD_CHAIN pWordItemHead;
};
typedef struct tagModifyTable MODIFY_TABLE,*PMODIFY_TABLE;

class CDictionary
{
public:
	CDictionary();
	~CDictionary();
	bool Load(char *sFilename, bool bReset=false);
	bool DelModifyTable();
	bool FindInOriginalTable(int nInnerCode, char *sWord, int nHandle);
	bool FindInOriginalTable(int nInnerCode, char *sWord, int nHandle, int *nPosRet);
	bool PreProcess(char *sWord, int *nInnerCode, char *sWordRet);
	//判断是否有匹配的词
	bool GetMaxMatch(char *sWord, char *sWordRet);
	//返回匹配的词的数据
	bool GetHandle(char *sWord, int *nMatchCount, int *nMatchHandle, int *nMatchFreq);
	int GetFrequency(char *sWord, int nHandle);
	INDEX_ITEM m_IndexTable[CC_NUM];
	PMODIFY_TABLE m_pModifyTable;
};

#endif