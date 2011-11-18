#ifndef DICTINARY_H
#define DICTINARY_H

#define CC_NUM 6768

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
	INDEX_ITEM m_IndexTable[CC_NUM];
	PMODIFY_TABLE m_pModifyTable;
};

#endif