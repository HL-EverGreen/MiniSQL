#include "API.h"
#include "BufferManager.h"
#include "CatalogManager.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include "Interpret.h"
#include "Sth.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

extern RecordManager record;
extern IndexManager index;
extern CatalogManager catalog;
extern Interpret interprt;
extern BufferManager buf;

void API::ShowResult(Data data, Table tableinfor, vector<Attribute> column) {
    if (column[0].name == "*") {
        cout << endl << "+";
        for (int i = 0; i < tableinfor.attriNum; i++) {
            for (int j = 0; j < tableinfor.attributes[i].length + 1; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
        cout << "| ";
        for (int i = 0; i < tableinfor.attriNum; i++) {
            cout << tableinfor.attributes[i].name;
            int lengthLeft = tableinfor.attributes[i].length - tableinfor.attributes[i].name.length();
            for (int j = 0; j < lengthLeft; j++) {
                cout << ' ';
            }
            cout << "| ";
        }
        cout << endl;
        cout << "+";
        for (int i = 0; i < tableinfor.attriNum; i++) {
            for (int j = 0; j < tableinfor.attributes[i].length + 1; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;

        //����
        for (int i = 0; i < data.rows.size(); i++) {
            cout << "| ";
            for (int j = 0; j < tableinfor.attriNum; j++) {
                int lengthLeft = tableinfor.attributes[j].length;
                for (int k = 0; k < data.rows[i].columns[j].length(); k++) {
                    if (data.rows[i].columns[j].c_str()[k] == EMPTY) break;
                    else {
                        cout << data.rows[i].columns[j].c_str()[k];
                        lengthLeft--;
                    }
                }
                for (int k = 0; k < lengthLeft; k++) cout << " ";
                cout << "| ";
            }
            cout << endl;
        }

        cout << "+";
        for (int i = 0; i < tableinfor.attriNum; i++) {
            for (int j = 0; j < tableinfor.attributes[i].length + 1; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
    }
    else {
        cout << endl << "+";
        for (int i = 0; i < column.size(); i++) {
            int col;
            for (col = 0; col < tableinfor.attributes.size(); col++) {
                if (tableinfor.attributes[col].name == column[i].name) break;
            }
            for (int j = 0; j < tableinfor.attributes[col].length + 1; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
        cout << "| ";
        for (int i = 0; i < column.size(); i++) {
            int col;
            for (col = 0; col < tableinfor.attributes.size(); col++) {
                if (tableinfor.attributes[col].name == column[i].name) break;
            }
            cout << tableinfor.attributes[col].name;
            int lengthLeft = tableinfor.attributes[col].length - tableinfor.attributes[col].name.length();
            for (int j = 0; j < lengthLeft; j++) {
                cout << ' ';
            }
            cout << "| ";
        }
        cout << endl;
        cout << "+";
        for (int i = 0; i < column.size(); i++) {
            int col;
            for (col = 0; col < tableinfor.attributes.size(); col++) {
                if (tableinfor.attributes[col].name == column[i].name) break;
            }
            for (int j = 0; j < tableinfor.attributes[col].length + 1; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;

        //����
        for (int i = 0; i < data.rows.size(); i++) {
            cout << "| ";
            for (int j = 0; j < column.size(); j++) {
                int col;
                for (col = 0; col < tableinfor.attributes.size(); col++) {
                    if (tableinfor.attributes[col].name == column[j].name) break;
                }
                int lengthLeft = tableinfor.attributes[col].length;
                for (int k = 0; k < data.rows[i].columns[col].length(); k++) {
                    if (data.rows[i].columns[col].c_str()[k] == EMPTY) break;
                    else {
                        cout << data.rows[i].columns[col].c_str()[k];
                        lengthLeft--;
                    }
                }
                for (int k = 0; k < lengthLeft; k++) cout << " ";
                cout << "| ";
            }
            cout << endl;
        }

        cout << "+";
        for (int i = 0; i < column.size(); i++) {
            int col;
            for (col = 0; col < tableinfor.attributes.size(); col++) {
                if (tableinfor.attributes[col].name == column[i].name) break;
            }
            for (int j = 0; j < tableinfor.attributes[col].length + 1; j++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
    }
    cout << data.rows.size() << " rows have been found." << endl;
}

void API::Execute()
{
    Table tableinfor;
    Index indexinfor;
    string tempKeyValue;
    int tempPrimaryPosition = -1;
    int rowCount = 0;
    Data data;
    switch (interprt.m_operation)
    {
    case EXECFILE:
        fp = fopen(interprt.m_filename,"rb");
        if (!fp)
        {
            cout << "/(��o��)/  Fail to open " << interprt.m_filename << endl;
            break;
        }        
        fileflag = 1; //��״̬��Ϊ���ļ�����
        break;
    case CRETAB:
        interprt.getTableInfo.attriNum = interprt.getTableInfo.attributes.size();
        catalog.createTable(interprt.getTableInfo);
        record.createTable(interprt.getTableInfo);
        cout << "O(��_��)O  Table " << interprt.getTableInfo.name << " created" << endl;
        break;
    case TABLEEXISTED:
        cout << "/(��o��)/  Table has already exist" << endl;
        break;
    case DRPTAB:
        record.dropTable(interprt.getTableInfo);
        for (int i = 0; i < interprt.getTableInfo.attriNum; i++) {
            indexinfor = catalog.getIndexInformation(interprt.getTableInfo.name, i);
            if (indexinfor.index_name != "")
                index.dropIndex(indexinfor);
        }
        catalog.dropTable(interprt.getTableInfo);
        cout << "O(��_��)O  Table " << interprt.getTableInfo.name << " dropped" << endl;
        break;
    case INSERT:
        tableinfor = interprt.getTableInfo;
        //cout << interprt.PrimaryKeyPosition << endl;
        if (interprt.PrimaryKeyPosition == -1 && interprt.UniquePostion == -1) {
            record.insertValue(tableinfor, interprt.row);
            catalog.update(tableinfor);
            cout << "] has been successfully inserted" << endl;
            break;
        }
        if (interprt.PrimaryKeyPosition != -1)
        {
            data = record.select(tableinfor, interprt.condition);
            if (data.rows.size()>0) {
                cout << "/(��o��)/  Primary key aren't unique" << endl;
                break;
            }
        }
        if (interprt.UniquePostion != -1) {

            data = record.select(tableinfor, interprt.UniqueCondition);
            if (data.rows.size()>0) {
                cout << "/(��o��)/  Unique value aren't unique" << endl;
                break;
            }
        }
        record.insertValue(tableinfor, interprt.row);
        catalog.update(tableinfor);
		cout << "] has been successfully inserted" << endl;
        break;
    case INSERTERR:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case SELECT_NOWHERE_CAULSE:
        tableinfor = interprt.getTableInfo;
        data = record.select(tableinfor);
        if (data.rows.size() != 0)
            ShowResult(data, tableinfor, interprt.column);
        else {
            cout << "/(��o��)/  No content record" << endl;
        }
        break;
    case SELECT_WHERE_CAULSE:
        tableinfor = interprt.getTableInfo;
        if (interprt.condition.size() == 1) {
            for (int i = 0; i<interprt.getTableInfo.attributes.size(); i++) {
                if ((interprt.getTableInfo.attributes[i].isPrimeryKey == true || interprt.getTableInfo.attributes[i].isUnique == true) && interprt.m_colname == interprt.getTableInfo.attributes[i].name) {
                    tempPrimaryPosition = i;
                    indexinfor = catalog.getIndexInformation(tableinfor.name, i);
                    break;
                }
            }
            if (tempPrimaryPosition == interprt.condition[0].columnNum&&interprt.condition[0].op == Eq&&indexinfor.table_name != "") {

                tempKeyValue = interprt.condition[0].value;
                data = index.selectEqual(tableinfor, indexinfor, tempKeyValue);
            }
            else {

                data = record.select(tableinfor, interprt.condition);
            }
        }
        else {
            data = record.select(tableinfor, interprt.condition);
        }
        if (data.rows.size() != 0)
            ShowResult(data, tableinfor, interprt.column);
        else {
            cout << "/(��o��)/  No content record" << endl;
        }
        break;
    case DELETE:
        rowCount = record.deleteValue(interprt.getTableInfo, interprt.condition);
        cout << "O(��_��)O  " << rowCount << "  rows have been deleted." << endl;
        break;
    case CREIND:
        tableinfor = interprt.getTableInfo;
        indexinfor = interprt.getIndexInfo;
        if (!tableinfor.attributes[indexinfor.column].isPrimeryKey&&!tableinfor.attributes[indexinfor.column].isUnique) {//����primary key�������Խ�index
            cout << "O(��_��)O  " << "Column " << tableinfor.attributes[indexinfor.column].name << "  is not unique." << endl;
            break;
        }
        catalog.createIndex(indexinfor);
        index.createIndex(tableinfor, indexinfor);
        catalog.update(indexinfor);
        cout << "O(��_��)O  The index " << indexinfor.index_name << " created" << endl;
        break;
    case INDEXERROR:
        cout << "/(��o��)/  Index alreaedy existed" << endl;
        break;
    case DRPIND:
        indexinfor = catalog.getIndexInformation(interprt.m_indname);
        if (indexinfor.index_name == "") {
            cout << "/(��o��)/  Index" << interprt.m_indname << "does not exist" << endl;
        }
        index.dropIndex(indexinfor);
        catalog.dropIndex(interprt.m_indname);
        cout << "O(��_��)O  The index dropped" << endl;
        break;
    case CREINDERR:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case QUIT:
        cout << "/(��o��)/  Bye~" << endl;
        //getchar();
        exit(0);
        break;
    case EMPTY:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case UNKNOW:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case SELERR:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case CRETABERR:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case DELETEERR:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case DRPTABERR:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case DRPINDERR:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case VOIDPRI:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case VOIDUNI:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case CHARBOUD:
        cout << "/(��o��)/  Invalid number of charactors" << endl;
        break;
    case NOPRIKEY:
        cout << "/(��o��)/  Invalid query" << endl;
        break;
    case TABLEERROR:
        cout << "/(��o��)/  No such table" << endl;
        break;
    case INDEXEROR:
        cout << "/(��o��)/  No such index" << endl;
        break;
    case COLUMNERROR:
        cout << "/(��o��)/  No such column" << endl;
        break;
    case INSERTNUMBERERROR:
        cout << "/(��o��)/  The number of columns is larger than 32" << endl;
        break;
    }

}

void API::AddSeperator(char *command)
{
    unsigned len = strlen(command);
    command[len] = ' ';
    command[len + 1] = '\0';
}

short int API::IsComEnd(char *input)
{
    unsigned int next = strlen(input) - 1;
    char prev = ' ';
    while (next >= 0 && (prev == '\t' || prev == ' '))
    {
        prev = input[next];
        next--;
    }
    if (prev == ';')
    {
        input[next + 1] = '\0';
        return 1;
    }
    if (input[0] == '\n' && input[1] == ';')
    {
        input[next + 1] = '\0';
        return 1;
    }
    //cout << "prev: " << prev << endl;
    //printf("%d\n",prev);
    return 0;
}