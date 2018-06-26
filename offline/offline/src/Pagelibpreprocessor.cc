#include "Pagelibpreprocessor.h"
#include "math.h"
#include "time.h"
#include "iterator"
using std::make_pair;
PageLibPreProcessor::PageLibPreProcessor(My_conf & conf):
    _conf(conf)
{}
void PageLibPreProcessor::doProcess()
{
    readInfoFromFile();
    time_t time1 = time(NULL);
    cutRedundantPages();
    buildInvertIndexTable();
    time_t time2 = time(NULL);
    std::cout<<"build invert index table, processtime: "<<(time1-time2)<<"seconds\n";
    storeOnDisk();
    time_t time3 = time(NULL);
    std::cout<<"save newpaglib on disk, processtime: "<<(time3-time2)<<"seconds\n";
}
void PageLibPreProcessor::readInfoFromFile()
{
    map<string,string> pagetemp = _conf.get_config_map();
    std::ifstream ifspl(pagetemp["RawPageLib"]);
    std::ifstream ifsos(pagetemp["RawOffsetLib"]);
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
        Webpage wp1(doc,_conf,_jieba);
        _pageLib.push_back(wp1);
        _offsetLib.insert(make_pair(docid,make_pair(dochead,doclen)));
    }
}
void PageLibPreProcessor::cutRedundantPages()
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
            }
        }
    }
}
void PageLibPreProcessor::buildInvertIndexTable()
{
    //获取词频表，建立索引表 内容为 <单词，<docid,词频>>
    for(size_t idx = 0; idx < _pageLib.size(); ++idx)
    {
        map<string,int> wordfreqmap = _pageLib[idx].getwordmap();
        for(auto & it:wordfreqmap)
        {
            _invertIndexTable[it.first].push_back(make_pair(_pageLib[idx].getDocid(),it.second));
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
void PageLibPreProcessor::storeOnDisk()
{
    sort(_pageLib.begin(),_pageLib.end());
    map<string,string> pagetemp = _conf.get_config_map();
    std::ofstream ofspl(pagetemp["NewPageLib"]);
    std::ofstream ofsos(pagetemp["NewOffsetLib"]);
    if(!ofspl.good() || !ofsos.good())
    {
        std::cout<<"err open file\n";
        return;
    }
    vector<Webpage>::iterator it = _pageLib.begin();
    while(it != _pageLib.end())
    {
       string doc = it->Webpage::getDoc();
       int len = doc.size();
       int docid = it->Webpage::getDocid();
       unsigned long offset = ofspl.tellp();
       ofsos << docid << '\t' << offset << '\t' << len<< '\n';
       ofspl<<doc;
       ++it;
    }
}
#ifdef HELLO
{
    My_conf & _conf;
    Word_segment _jieba;
    vector<Webpage> _pageLib;
    unordered_map<int,pair<int,int>> _offsetLib;
    unordered_map<string,pair<int,double>> _invertIndexTable;
    void readInfoFromFile();
    void cutRedundantPages();
    void buildInvertIndexTable();
    void storeOnDisk();
public:
    PageLibPreProcessor(My_conf & conf);
    void doProcess();
};
#endif

