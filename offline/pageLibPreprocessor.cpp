 ///
 /// @file    pageLibPreprocessor.cpp
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-23 11:10:46
 ///
 
#include "pageLibPreprocessor.h"

#include <string.h>
#include <cmath>
#include <ctime>
#include <sstream>

PageLibPreprocessor::PageLibPreprocessor(Configuration & conf)
:_conf(conf)
{}

void PageLibPreprocessor::doProcess(){
    readInfoFromFile();
    time_t time1 = time(NULL);
    cutRedundantPages();
    buildInvertIndexTable();
    time_t time2 = time(NULL);
    std::cout<<"build invert index table, processtime: "<<(time2-time1)<<"seconds\n";
    storeOnDisk();
    time_t time3 = time(NULL);
    std::cout<<"save newpaglib on disk, processtime: "<<(time3-time2)<<"seconds\n";
}

void PageLibPreprocessor::readInfoFromFile(){
	ifstream ifspl(_conf.getConfigMap()["myripepagelib"]);
	ifstream ifsos(_conf.getConfigMap()["myoffsetlib"]);
    if(!ifspl.good() || !ifsos.good())
    {
        std::cout<<"lib file open err\n";
        return;
    }
    string line;
    int docid,dochead,doclen;
    while(getline(ifsos,line))
    {
        std::istringstream issos(line);
        issos>>docid>>dochead>>doclen;
        string doc;
        doc.resize(doclen,' ');
        ifspl.seekg(dochead,ifspl.beg);
        ifspl.read(&*doc.begin(),doclen);
        WebPage wp1(doc,_conf,_jieba);
        _pageLib.push_back(wp1);
        _offsetLib.insert(make_pair(docid,make_pair(dochead,doclen)));
    }
}

#if 0
void PageLibPreprocessor::cutRedundantPages()
{
    unsigned long i= 0,j;
    for(;i<_pageLib.size()-1;++i)
    {
        for(j=i+1; j<_pageLib.size();++j)
        {
            if(_pageLib[i] == _pageLib[j])
            {
                _pageLib[j] = _pageLib[_pageLib.size()-1];
                _pageLib.pop_back();
                --j;
				cout << "cut number " << i << endl;
            }
        }
    }
}
#endif
#if 1
void PageLibPreprocessor::cutRedundantPages(){
	auto it1 = _pageLib.begin();
	auto it2 = _pageLib.begin();
	auto every = _pageLib.begin();
	while(every!=_pageLib.end()){
		cout << "every : " << every->getDocId() << endl;
		++it1;
		it2 = it1;
		while(it2!=_pageLib.end()){
			//cout << it2->getDocId();
			++it2;
			if(*it2 == *every){
				cout << "find a same " << every->getDocId() << "----" << it2->getDocId() << endl;
				auto tmpit = it2--;
				_pageLib.erase(tmpit);
				cout << "erase one " << endl;
			}
		}
		++every;
	}
}

#endif
#if 0
void PageLibPreprocessor::buildInvertIndexTable(){
	unordered_map<string,int> DF;
	unordered_map<string,int> ws;
	for(auto every : _pageLib){
		for(auto everyone : every.getWordMap()){
			++DF[everyone.first];
		}
	}
	
	map<string,int> TF;
	for(auto every : _pageLib){//对于每一篇文章
		TF = every.getWordMap();
		for(auto everyone : TF){
			ws[everyone.first] = log(_pageLib.size()/(DF[everyone.first]+1))/log(2);
		}
		
		int pingfanghe = 0;
		for(auto everytwo : ws){
			pingfanghe += (ws[everytwo.first])*(ws[everytwo.first]);
		}

		for(auto everythree : ws){
			_invertIndexTable[everythree.first].push_back(make_pair(every.getDocId(),everythree.second/sqrt(pingfanghe)));
		}
	}
}

#endif
#if 1

void PageLibPreprocessor::storeOnDisk(){
	ofstream of1(_conf.getConfigMap()["mynewripepagelib"]);
	ofstream of2(_conf.getConfigMap()["mynewoffsetlib"]);
	ofstream of3(_conf.getConfigMap()["myinvertindexlib"]);
	int tmpsite = 0;
	for(auto every : _pageLib){
		of1 << every.getDoc();
		_offsetLib.insert(make_pair(every.getDocId(),make_pair(tmpsite,every.getDoc().size())));
		tmpsite+=every.getDoc().size();
	}
	of1.close();

	for(auto every : _offsetLib){
		of2 << every.first << " " << every.second.first << " " << every.second.second << endl;
	}
	of2.close();

	for(auto every : _invertIndexTable){
		of3 << every.first << endl;
		for(auto everyone : every.second){
			of3 << everyone.first << " " << everyone.second << " ";
		}
		of3 << endl;
	}
	of3.close();
}
#endif
#if 1
void PageLibPreprocessor::buildInvertIndexTable()
{
    //获取词频表，建立索引表内容为<单词,<docid,词频>>
    for(size_t idx = 0; idx < _pageLib.size(); ++idx)
    {
        map<string,int> wordfreqmap = _pageLib[idx].getWordMap();
        for(auto & it:wordfreqmap)
        {
            _invertIndexTable[it.first].push_back(make_pair(_pageLib[idx].getDocId(),it.second));
        }
    }
    //获取单词出现文章数目DF，计算权重weight
    int n = _pageLib.size();
    //统计文章权重weightsum <docid，weightsum>
    map<int,double> weightsum;
    for(auto & it:_invertIndexTable)
    {
        int df = it.second.size();
        for(auto & wordfreq:it.second)
        {
            double idf = (log(n)-log(df+1))/log(2);
            double weight =  idf*wordfreq.second;
            wordfreq.second = weight;
            weightsum[wordfreq.first] += weight;
        }
    }
    //归一化处理
    for(auto & it:_invertIndexTable)
    {
        for(auto & wordweight:it.second)
        {
//	    std::cout<<it.first<<"\tin\t"<<wordweight.first<<":" <<weightsum[wordweight.first] <<"\n";	    
	    wordweight.second /= weightsum[wordweight.first];
        }
    }
}
#endif
#if 0
void PageLibPreprocessor::storeOnDisk()
{
    sort(_pageLib.begin(),_pageLib.end());
    map<string,string> pagetemp = _conf.getConfigMap();
    std::ofstream ofspl(pagetemp["NewPageLib"]);
    std::ofstream ofsos(pagetemp["NewOffsetLib"]);
    if(!ofspl.good() || !ofsos.good())
    {
        std::cout<<"err open file\n";
        return;
    }
    vector<WebPage>::iterator it = _pageLib.begin();
    while(it != _pageLib.end())
    {
       string doc = it->WebPage::getDoc();
       int len = doc.size();
       int docid = it->WebPage::getDocId();
       unsigned long offset = ofspl.tellp();
       ofsos << docid << '\t' << offset << '\t' << len<< '\n';
       ofspl<<doc;
       ++it;
    }
}
#endif
