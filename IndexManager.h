#ifndef _INDEX_H_
#define _INDEX_H_
#include "BufferManager.h"
#include "Sth.h"
#include <list>
#define POINTERLENGTH 5
extern BufferManager buf;

class IndexLeaf{
public:
	string key;
	int offsetInFile;
	int offsetInBlock;
	IndexLeaf():
		key(""), offsetInFile(0), offsetInBlock(0){}
	IndexLeaf(string k, int oif, int oib):
		key(k), offsetInFile(oif), offsetInBlock(oib){}
};

class IndexBranch{
public:
	string key;
	int ptrChild;	
	IndexBranch():
		key(""), ptrChild(0){}
	IndexBranch(string k, int ptrC):
		key(k), ptrChild(ptrC){}
};

class BPlusTree{
public:
	bool isRoot;
	int bufferNum;
	int ptrFather;		
	int recordNum;
	int columnLength;
	BPlusTree(){}
	BPlusTree(int vbufNum): bufferNum(vbufNum), recordNum(0){}
    int getPtr(int pos);
    int getRecordNum();
};

//��Ҷ�ӽڵ�
class Branch: public BPlusTree
{
public:
	list<IndexBranch> nodelist;
	Branch(){}
	Branch(int vbufNum): BPlusTree(vbufNum){}
    Branch(int vbufNum, const Index& indexinfor);
    ~Branch();

    void insert(IndexBranch node);	
    IndexBranch pop();	
    IndexBranch getfront();
};

class Leaf: public BPlusTree
{
public:	
	int nextSibling;
	int lastSibling;
	list<IndexLeaf> nodelist;
    Leaf(int vbufNum);
    Leaf(int vbufNum, const Index& indexinfor);
    ~Leaf();

    void insert(IndexLeaf node);
    IndexLeaf pop();
    IndexLeaf getfront();
};

class IndexManager{
public:
    //ȱʡ���캯����������û��Ա����...
    //ȱʡ��������
    void createIndex(const Table& tableinfor, Index& indexinfor);
    IndexBranch insertValue(Index& indexinfor, IndexLeaf node, int blockOffset = 0);	
    Data selectEqual(const Table& tableinfor, const Index& indexinfor, string key, int blockOffset = 0);
    Data selectBetween(const Table& tableinfor, const Index& indexinfor, string keyFrom, string keyTo, int blockOffset = 0);
    Row splitRow(Table tableinfor, string row);
    string getColumnValue(const Table& tableinfor, const Index& indexinfor, string row);
    void dropIndex(Index& indexinfor);
	void deleteValue(){}
};

#endif
