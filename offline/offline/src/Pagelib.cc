#include "Pagelib.h"
#include "DirScanner.h"
#include "RssReader.h"
#include <fstream>
#include <iostream>

Pagelib::Pagelib(My_conf & conf, Dir_scanner & dirscanner):
    _conf(conf),
    _dirscanner(dirscanner)
{}
void Pagelib::create()
{
    vector<string> & vecfiles = _dirscanner.files();
    Rss_reader rss1(vecfiles);
    rss1.loadsrcfiles();
    rss1.makepage(_vecpages);
}
void Pagelib::store()
{
    map<string,string> & confmap = _conf.get_config_map();
    string Pagelibpath = confmap["RawPageLib"];
    string offsetlibpath = confmap["RawOffsetLib"];
    std::ofstream ofspage(Pagelibpath);
    std::ofstream ofsoffset(offsetlibpath);
    if(!ofspage.good() || !ofsoffset.good())
    {
        std::cout<<"ofstream open err\n";
        return ;
    }
    for(size_t idx = 0;idx != _vecpages.size();++idx)
    {
        int len = _vecpages[idx].size();
        std::ofstream::pos_type offset = ofspage.tellp();
        ofspage<<_vecpages[idx];
        ofsoffset<<idx+1<<"\t"
            <<offset<<"\t"
            <<len<<"\n";
    }
    ofspage.close();
    ofsoffset.close();
}

#ifdef HELLO
class Pagelib
{
    My_conf & _conf;
    Dir_scanner & _dirscanner;
    vector<string> _vecpages;
public:
    Pagelib(My_conf & conf,Dir_scanner & dirscanner);
    void create();
    void store();
};

#endif
