#include "RssReader.h"
#include "boost/regex.hpp"
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
using boost::regex;
using boost::regex_replace;

Rss_reader::Rss_reader(vector<string> & files)
    :_files(files)
{}
Rss_reader::~Rss_reader()
{}
void Rss_reader::loadsrcfiles()
{
    for(auto & it: _files)
    {
        loadsrcfile(it);
    }
}
void Rss_reader::loadsrcfile(const string & srcfile)
{
    XMLDocument doc;
    doc.LoadFile(srcfile.c_str());
    std::cout<<">load "<<srcfile<<"\n";
    XMLError errorID = doc.ErrorID();
    if(errorID){
        std::cout<<"XMLDocument LoadFile err\n";
        return ;
    }
    parseRss(doc);
}
void Rss_reader::parseRss(XMLDocument & doc)
{
  //segmentfault
    XMLElement * rss = doc.FirstChildElement();
    XMLElement *channel = rss->FirstChildElement("channel");
    XMLElement *itemnode = channel->FirstChildElement("item");
    std::cout << "parsRss(XMLDocument & doc)\n" ;
    while(itemnode)
    {
        shared_ptr<Rssitem> rssItemptr(new Rssitem);
        const char * itemtitle = itemnode->FirstChildElement("title")->GetText();
//	std::cout<<"itemtitle: "<<itemtitle<<"\n";
        const char * itemlink = itemnode->FirstChildElement("link")->GetText();
//	std::cout<<"itemlink: "<<itemlink<<"\n";
	if(!strcmp(itemnode->Value(),"author")){
            const char * itemauthor = itemnode->FirstChildElement("author")->GetText();
	    rssItemptr->_author = itemauthor;
//	    std::cout<<"author: "<<itemauthor<<"\n";

	}
        const char * itemdescription = itemnode->FirstChildElement("description")->GetText();
        const char * icontent = nullptr;
        XMLElement * contentencoded = itemnode->FirstChildElement("content:encoded");
	if(contentencoded)
        {
            icontent = contentencoded->GetText();
        }
        else
        {
            icontent = itemdescription;
        }
        string itemcontent;
        regex e0("(<){1}?(.){1,}?(>){1}"),
//              e1("(\&\#\\d{4};)|(\&[a-z]{4}\;)"),
              e2("(&lt;)|(&gt;)|(&amp;)|(&nbsp;)");
        char s1[20] = {0};
        strcpy(s1,"(?1<)(?2>)(?3&)(?4 )");
        itemcontent = regex_replace(string(icontent),e0,"");
//        itemcontent = regex_replace(itemcontent,e1,"");
        itemcontent = regex_replace(itemcontent,e2,s1,boost::match_default|boost::format_all);
        rssItemptr->_content = itemcontent;
        rssItemptr->_title = itemtitle;
        rssItemptr->_link = itemlink;
        _items.push_back(rssItemptr);
        itemnode = itemnode->NextSiblingElement();
    }
}
void Rss_reader::makepage(vector<string> & pages)
{
    std::cout<<"page size" <<_items.size()<<"\n";
    std::ostringstream oss1;
    int i = 0;
    for(auto & it :_items)
    {
        oss1.str("");
	oss1<<"<doc>\n"
            <<"\t<docid>"<<i++<<"</docid>\n"
            <<"\t<title>"<<it->_title<<"</title>\n"
            <<"\t<link>"<<it->_link<<"</link>\n"
            <<"\t<author>"<<it->_author<<"</author>\n"
            <<"\t<content>"<<it->_content<<"</content>\n"
            <<"</doc>\n";
        string page = oss1.str();
        pages.push_back(page);
    }
}

#ifdef HELLO
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
    void initwithrssstring(const string & rss);
    void makepage(vector<string> & pages);
    void debug();
    void dump();
};
#endif
