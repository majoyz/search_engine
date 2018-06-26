#pragma once
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
using std::string;

#include <map>
using std::map;

#include <set>
using std::set;

class My_conf{
    string _confpath;
    map<string,string> _confmap;
    set<string> _stoplistword;
public:
    My_conf(const string &);
    map<string,string> & get_config_map();
    set<string> get_stopwordlist();
    void debug();
};

#endif
