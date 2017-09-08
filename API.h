#ifndef _API_h_
#define _API_h_
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "BufferManager.h"
#include "CatalogManager.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include "Interpret.h"
#include "Sth.h"
using namespace std;

class API {
public:
    bool fileflag;  //�����ִ���ļ�����ִ������
    ifstream file; //��������ű�
    FILE *fp; //��������ű�
    void ShowResult(Data data, Table tableinfor, vector<Attribute> column);
    void Execute();
    void AddSeperator(char *command);
    short int IsComEnd(char *input);
};

#endif
