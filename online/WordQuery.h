 ///
 /// @file    WordQuery.h
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-06-27 10:38:18
 ///
 
#ifndef __WORDQUERY_H__
#define __WORDQUERY_H__

#include "webPage.h"
using namespace std;
class Configuration;
class WebPage;

class wordQuery{
    Configuration & _conf;//配置文件的引用
    Word_segment _jieba;//jieba分词库对象
    unordered_map<int, WebPage> _pagelib;//网页库
    unordered_map<int,pair<int,int>> _offsetlib;//偏移库
    unordered_map<string ,set<pair<int,double>>> _invertIndexTable;//倒排索引表
    void loadLibrary();//加载库文件
    vector<double> getQueryWordsWeightVector(vector<string> &);//计算查询词的权重值
    bool executeQuery(const vector<string> &, vector<pair<int,vector<double>>> &);//执行查询
    string createJson(vector<int> & , const vector<string> &);
    string returnNoAnswer();
public:
    wordQuery(Configuration & conf);//构造函数
    string doQeury(const string & str);//执行查询返回结果
};

#endif
