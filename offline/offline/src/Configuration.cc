#include "Configuration.h"
#include <iostream>
#include <fstream>
#include <sstream>
My_conf::My_conf(const string & filepath)//构造函数
    :_confpath(filepath)
{
    std::ifstream ifs(_confpath);
    string line;
    while(getline(ifs,line))//获取各个conf信息
    {
        std::istringstream iss(line);
        string key, value;
        iss>>key>>value;
        _confmap[key] = value;
    }
    ifs.close();
}
map<string, string> & My_conf::get_config_map()//
{
    return _confmap;
}
set<string> My_conf::get_stopwordlist()//
{
    std::ifstream ifs(_confmap["StopWordList"]);
    string word;
    while(ifs>>word)
    {
        _stoplistword.insert(word);
    }
    ifs.close();
    _stoplistword.insert(" ");
    return _stoplistword;
}
void My_conf::debug()//测试
{
    for(auto & it: _confmap)
    {
        std::cout<<it.first<<" "<<it.second<<"\n";
    }
}

#ifdef HELLO
class My_conf{
    string _confpath;
    map<string,string> _confmap;
    set<string> _stoplistword;
public:
    My_conf(string &);
    map<string,string> & get_config_map();
    set<string> get_stopwordlist();
};

#endif
