#pragma once
#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__
#include "Configuration.h"
#include "WordSegment.h"

class Webpage
{
    const static int TOPK_NUMBER = 20;
    string _doc;
    int _docid;
    string _doctitle;
    string _docurl;
    string _doccontent;
    string _docsummary;
    vector<string> _topword;
    map<string,int> _wordMap;
    friend bool operator == (const Webpage & lhs, const Webpage & rhs);
    friend bool operator < (const Webpage & lhs, const Webpage & rhs);
    void processDoc(const string & doc, My_conf & conf, Word_segment & jieba);
    void calcTopk(vector<string> & wordsVec, int k, set<string> & stopwordlist);
public:
    Webpage(string & doc, My_conf & conf,Word_segment & jieba);
    int getDocid() {return _docid;}
    map<string,int> & getwordmap() {return _wordMap;}
    string getDoc() {return _doc;}
};
#endif
