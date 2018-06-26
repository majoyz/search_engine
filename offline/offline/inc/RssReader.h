#pragma once
#ifndef __RSSREADER__
#define __RSSREADER__
#include "tinyxml2.h"
using namespace tinyxml2;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
struct Rssitem
{
    string _title;
    string _link;
    string _author;
    string _pubdate;
    string _content;
};
class Rss_reader
{
    vector<shared_ptr<Rssitem>> _items;
    vector<string> & _files;
    void parseRss(XMLDocument & doc);
public:
    Rss_reader(vector<string> & files);
    ~Rss_reader();
    void loadsrcfiles();
    void loadsrcfile(const string & srcfile); 
    void makepage(vector<string> & pages);
};
#endif
