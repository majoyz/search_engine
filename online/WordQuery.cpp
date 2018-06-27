 ///
 /// @file    WordQuery.cpp
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-06-27 10:43:43
 ///
 
#include "WordQuery.h"
#include "configuration.h"
#include <math.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <json/json.h>

using std::stable_sort;
using std::ifstream;
using std::istringstream;
using std::make_pair;

struct SimilarityCompare
{
    SimilarityCompare(vector<double> & base)
        :_base(base)
    {}
    bool operator()(const pair<int,vector<double>> & lhs, const pair<int,vector<double>> & rhs)
    {
        return rhs.first>lhs.first;
    }
    vector<double> _base;
};

wordQuery::wordQuery(My_conf & conf)
    :_conf(conf)
{
    loadLibrary();
}
void wordQuery::loadLibrary()
{
    ifstream ifspl(_conf.get_config_map()["NewPageLib"]);
    ifstream ifsol(_conf.get_config_map()["NewOffsetLib"]);
    if(!ifspl.good() || !ifsol.good())
    {
        std::cout<<"data lib get fail, quiting...\n";
    }
    int docid, offset, len;
    string line;
//    int cnt = 0;
    while(getline(ifsol,line))
    {
        istringstream iss(line);
        iss>>docid>>offset>>len;
        
        string doc;
        doc.resize(len,' ');
        ifspl.seekg(offset, ifspl.beg);
        ifspl.read(&*doc.begin(),len);
        
        OLWebpage owp(doc,_conf,_jieba);
        _pagelib.insert(make_pair(docid,owp));
        _offsetlib.insert(make_pair(docid,make_pair(offset, len)));
    }
    ifstream ifsiit(_conf.get_config_map()["InvertIndexTable"]);
    if(!ifsiit.good())
    {
        std::cout<<"index table get fail, quiting...\n";
    }
    string word;
    double weight;
    while(getline(ifsiit,line))
    {
        istringstream iss(line);
        iss>>word;
        std::set<pair<int,double>> setID;
        while(iss>>docid>>weight)
        {
            setID.insert(make_pair(docid,weight));
        }
        _invertIndexTable.insert(make_pair(word,setID));
    }
    ifsol.close();
    ifsiit.close();
    ifspl.close();
    std::cout << "loadLibrary() end\n";
}
string wordQuery::doQeury(const string & strin)
{
    vector<string> queryword;
    if(strin.size()>0)
    {
        queryword = _jieba(strin.c_str());
    }
    for(auto & item:queryword)
    {
        cout<<item<<" ";
        auto result = _invertIndexTable.find(item);
	cout<<_invertIndexTable[item].size();
        if(result == _invertIndexTable.end()){
            cout<<"no result "<<item<<"\n";
            return returnNoAnswer();
        }
    }
    cout <<std::endl;
    vector<double> weightlist = getQueryWordsWeightVector(queryword);
    SimilarityCompare simcmp(weightlist);
    vector<pair<int,vector<double>>> resultVec;
    if(executeQuery(queryword, resultVec))
    {
        stable_sort(resultVec.begin(),resultVec.end(),simcmp);
        vector<int> docIdVec;
        for(auto & item:resultVec)
        {
            docIdVec.push_back(item.first);
        }
        return createJson(docIdVec,queryword);
    }
    return returnNoAnswer();
}
vector<double> wordQuery::getQueryWordsWeightVector(vector<string> & queryword)
{
    map<string, int> wordFreqmap;
    for(auto & item: queryword)
    {
        ++wordFreqmap[item];
    }
    vector<double> weightlist;
    double weightsum = 0;
    int total = _offsetlib.size();
    for(auto & item: queryword)
    {
        int df = _invertIndexTable[item].size();
        double idf = (log(total)- log(df+1))/log(2);
        double weightitem = idf * wordFreqmap[item];
        weightlist.push_back(weightitem);
        weightsum += pow(weightitem,2);
    }
    for(auto & item: weightlist)
    {
        item /= weightsum;
        cout<<"weightlist elem: "<<item<<"\t";
    }
    return weightlist;
}
bool wordQuery::executeQuery(const vector<string>& queryword, vector<pair<int,vector<double>>> & resultVec)
{
    if(queryword.size()<=0)
        return 0;
    int minVec = 1<<20;
    typedef set<pair<int,double>>::iterator setIt;
    vector<pair<setIt,int>> itVec;
    //vector<pair<string,int>> temp;//全部结果查询使用
    for(auto & item:queryword)
    {
        int crosssize = _invertIndexTable[item].size();
        if(crosssize<minVec)
        {
            minVec = crosssize;
        }
        itVec.push_back(make_pair(_invertIndexTable[item].begin(),0));
        //temp.push_back(make_pair(item,cosssize));
	cout<<"item "<<_invertIndexTable[item].size() <<"\n";
    }
    cout<<"minsize "<<minVec<<"\n";
    if(minVec == 0)
	return 0;
    bool isExit = 0;
    while(!isExit)
    {
        size_t idx = 0;
        for(;idx != itVec.size()-1;++idx)
        {
            if((itVec[idx].first)->first != (itVec[idx+1].first)->first)
                break;
        }
        if(idx == itVec.size()-1)
        {
            vector<double> weightVec;
            int docid = itVec[0].first->first;
            for(idx = 0; idx != itVec.size();++idx)
            {
                weightVec.push_back(itVec[idx].first->second);
                ++(itVec[idx].first);
                ++(itVec[idx].second);
                if(itVec[idx].second == minVec)//if(itVec[itdx].second == temp[itdx].second)
                    isExit = true;
            }
            resultVec.push_back(make_pair(docid,weightVec));
        }
        else
        {
            int itdx = 0;
            int minDocid = 1<<20;
            for(idx = 0; idx != itVec.size();++idx)
            {
                if(itVec[idx].first->first < minDocid)
                {
                    minDocid = itVec[idx].first->first;
                    itdx = idx;
                }
            }
            ++(itVec[itdx].first);
            ++(itVec[itdx].second);
            if(itVec[itdx].second == minVec)//if(itVec[itdx].second == temp[itdx].second)
                isExit = 1;
        }
    }
	if(resultVec.size() > 0)
    		return true; 
	return 0;
}
string wordQuery::createJson(vector<int> & docIdVec, const vector<string> & queryword)
{
    Json::Value root;
    Json::Value arr;
    Json::Value elem;
    int cnt = 0;
    for(auto & id:docIdVec)
    {
        string summary = _pagelib[id].summary(queryword);
        string title = _pagelib[id].getDocTitle();
        string url = _pagelib[id].getDocUrl();
        
        elem["title"] = title;
        elem["summary"] = summary;
        elem["url"] = url;
        arr.append(elem);
        if(++cnt == 100)
            break;
    }
    root["files"] = arr;
    Json::StyledWriter writer;
    return writer.write(root);
}
string wordQuery::returnNoAnswer()
{
    Json::Value root;
    Json::Value arr;
    Json::Value elem;
    elem["title"] = "GG";
    elem["summary"] = "404 NOT FOUND";
    elem["url"] = "";
    arr.append(elem);
    root["files"] = arr;
    Json::StyledWriter writer;
    return writer.write(root);
}

#ifdef HELLO
class My_conf;
class OLWebPage;

class wordQuery{
    My_conf & _conf;
    Word_segment _jieba;
    unordered_map<int, OLWebpage> _pagelib;
    unordered_map<int,pair<int,int>> _offsetlib;
    unordered_map<string ,set<pair<int,double>>> 
        _invertIndexTable;
    void loadLibrary();
    vector<double> getQueryWordsWeightVector(vector<string> &);
    bool executeQuery(const vector<string> &, vector<pair<int,vector<double>>>);
    string createJson(vector<int> & , const vector<string> &);
    string returnNoAnswer();
public:
    wordQuery(My_conf & conf);
    string doQeury(const string & str);
};

#endif
