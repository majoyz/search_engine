#pragma once
#ifndef __PAGELIB_H__
#define __PAGELIB_H__
#include <string>
using std::string;
#include <vector>
using std::vector;

class My_conf;
class Dir_scanner;
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
