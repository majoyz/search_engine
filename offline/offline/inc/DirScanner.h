#pragma once
#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include "Configuration.h"
#include <vector>
using std::vector;

class Dir_scanner
{
    vector<string> _vecfiles;
    My_conf _conf;
public:
    Dir_scanner(My_conf & conf);
    vector<string> & files();
    void traverse(const string & dirname);
    void operator()();
    void debug();// for test;
};

#endif
