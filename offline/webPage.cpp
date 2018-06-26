 ///
 /// @file    webPage.cpp
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-22 15:59:31
 ///
 
#include "webPage.h"
#include "boost/regex.hpp"

struct wordfreqorder
{
    bool operator ()(const std::pair<string,int> & lhs,const std::pair<string,int> & rhs)
    {
        if(lhs.second<rhs.second){
            return true;
        }else if(lhs.second == rhs.second && lhs.first < rhs.first)
        {
            return true;
        }else
        {
            return false;
        }
    }
};

WebPage::WebPage(string & doc,Configuration & conf,Word_segment & jieba)
:_doc(doc)
{
	_topWords.reserve(10);
	processDoc(_doc,conf,jieba);
}

int WebPage::getDocId(){
	return _docId;
}

string WebPage::getDoc(){
	return _doc;
}

uint64_t WebPage::getSimhash(){
	return _simhash;
}

bool operator==(WebPage & lhs,WebPage & rhs){
    int commNum = 0;
    for(auto & it :lhs._topWords)
    {
        commNum += std::count(rhs._topWords.begin(),rhs._topWords.end(),it);
    }
    int lhsnum = lhs._topWords.size();
    int rhsnum = rhs._topWords.size();
    int totalnum = lhsnum>rhsnum?lhsnum:rhsnum;
    if((double)(commNum/totalnum) > 0.75)
    {
        return 1;
    }
    return 0;
}

bool operator<(WebPage & lhs,WebPage & rhs){//对文档按docid进行排序
	return lhs.getDocId()<rhs.getDocId();
}

map<string,int> & WebPage::getWordMap(){//获取文档的词频统计map
	return _wordsMap;
}

void WebPage::processDoc(const string & doc, Configuration & conf,Word_segment & jieba)
{
    string docidhead = "<docid>";
    string docidend = "</docid>";
    string doctihead = "<title>";
    string doctiend = "</title>";
    string docurlhead = "<link>";
    string docurlend = "</link>";
    string docconhead = "<content>";
    string docconend = "</content>";
    //获取docid
    int stt = doc.find(docidhead);
    int end = doc.find(docidend);
    string docid = doc.substr(stt+docidhead.size(),
                end-stt-docidhead.size());
    _docId = atoi(docid.c_str());
    //获取doctitle
    stt = doc.find(doctihead);
    end = doc.find(doctiend);
    _docTitle = doc.substr(stt+doctihead.size(),
                end-stt-doctihead.size());
    //获取doclink
    stt = doc.find(docurlhead);
    end = doc.find(docurlend);
    _docUrl = doc.substr(stt+docurlhead.size(),
                end-stt-docurlend.size());
    //获取doccontent
    stt = doc.find(docconhead);
    end = doc.find(docconend);
    _docContent = doc.substr(stt + docconhead.size(),
					end - stt - docconhead.size());
//    std::cout<<_doccontent<<"\n";
    //进行分词
    vector<string> s1 = jieba(_docContent.c_str());
    //计算TOPK
    set<string> stopwordlist = conf.getStopWordList();
    calcTopK(s1,TOPK_NUMBER,stopwordlist);
//    std::cout<<"----------------------------------\n";
}

void WebPage::calcTopK(vector<string> & wordsVec, int k, set<string> & stopwordlist)
{
    for(auto & it :wordsVec){
        auto word = stopwordlist.find(it);
        if(word == stopwordlist.end())
            ++_wordsMap[it];
    }
    std::priority_queue<std::pair<string,int>, vector<std::pair<string,int>>,wordfreqorder> p1(_wordsMap.begin(),_wordsMap.end());
    while(!p1.empty())
    {
        string top = p1.top().first;
        p1.pop();
        if(top.size()==1 && ((int)top[0] == 10||
           (int)top[0] == 13))
        {continue;}
        _topWords.push_back(top);
//	std::cout<<"topword: "<<top<<"\n";
        if(_topWords.size() >= (size_t)10)
            break;
    }
}
