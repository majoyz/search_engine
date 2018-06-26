#include "Webpage.h"
#include <stdlib.h>
#include <string>
using std::string;
#include <queue>
using std::priority_queue;
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
Webpage::Webpage(string & doc,My_conf & conf, Word_segment & jieba)
    :_doc(doc)
{
    _topword.reserve(10);
    processDoc(_doc, conf,jieba);
}
bool operator == (const Webpage & lhs, const Webpage & rhs)
{
    int commNum = 0;
    for(auto & it :lhs._topword)
    {
        commNum += std::count(rhs._topword.begin(),rhs._topword.end(),it);
    }
    int lhsnum = lhs._topword.size();
    int rhsnum = rhs._topword.size();
    int totalnum = lhsnum>rhsnum?lhsnum:rhsnum;
    if((double)(commNum/totalnum) > 0.75)
    {
        return 1;
    }
    return 0;
}
bool operator < (const Webpage & lhs, const Webpage & rhs)
{
    return lhs._docid<rhs._docid;
}
void Webpage::processDoc(const string & doc, My_conf & conf,Word_segment & jieba)
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
    _docid = atoi(docid.c_str());
    //获取doctitle
    stt = doc.find(doctihead);
    end = doc.find(doctiend);
    _doctitle = doc.substr(stt+doctihead.size(),
                end-stt-doctihead.size());
    //获取doclink
    stt = doc.find(docurlhead);
    end = doc.find(docurlend);
    _docurl = doc.substr(stt+docurlhead.size(),
                end-stt-docurlend.size());
    //获取doccontent
    stt = doc.find(docconhead);
    end = doc.find(docconend);
    _doccontent = doc.substr(stt + docconhead.size(),
					end - stt - docconhead.size());
//    std::cout<<_doccontent<<"\n";
    //进行分词
    vector<string> s1 = jieba(_doccontent.c_str());
    //计算TOPK
    set<string> stopwordlist = conf.get_stopwordlist();
    calcTopk(s1,TOPK_NUMBER,stopwordlist);
//    std::cout<<"----------------------------------\n";
}
void Webpage::calcTopk(vector<string> & wordsVec, int k, set<string> & stopwordlist)
{
    for(auto & it :wordsVec){
        auto word = stopwordlist.find(it);
        if(word == stopwordlist.end())
            ++_wordMap[it];
    }
    std::priority_queue<std::pair<string,int>, 
        vector<std::pair<string,int>>,
        wordfreqorder>
        p1(_wordMap.begin(),_wordMap.end());
    while(!p1.empty())
    {
        string top = p1.top().first;
        p1.pop();
        if(top.size()==1 && ((int)top[0] == 10||
           (int)top[0] == 13))
        {continue;}
        _topword.push_back(top);
//	std::cout<<"topword: "<<top<<"\n";
        if(_topword.size() >= (size_t)10)
            break;
    }
}
#ifdef HELLO    
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
#endif

