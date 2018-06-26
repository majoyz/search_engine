#pragma once
#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include "Webpage.h"

using std::unordered_map;
using std::pair;
using std::vector;

class PageLibPreProcessor
{
    My_conf & _conf;
    Word_segment _jieba;
    vector<Webpage> _pageLib;
    unordered_map<int,pair<int,int>> _offsetLib;
    unordered_map<string,vector<pair<int,double>>> _invertIndexTable;

    void readInfoFromFile();
    void cutRedundantPages();
    void buildInvertIndexTable();
    void storeOnDisk();
public:
    PageLibPreProcessor(My_conf & conf);
    void doProcess();
};


#endif
