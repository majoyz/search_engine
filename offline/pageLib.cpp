///
/// @file    pageLib.cpp
/// @author  majoyz(zmj-miss@live.com)
/// @date    2018-05-21 21:26:27
///

#include "pageLib.h"
#include "tinyxml2.h"

#include <boost/regex.hpp>

PageLib::PageLib(Configuration & conf,DirScanner & dirScanner)
	:_conf(conf)
	 ,_dirScanner(dirScanner)
{}
//网页库类
void PageLib::create(){//创建网页库
	using namespace tinyxml2;
	int docid = 1;
	int itemplace = 0;
	XMLDocument doc;
	for(auto every : _dirScanner.files()){
		cout << "begin " << every << endl;
		doc.LoadFile(every.c_str());

	XMLElement * proot = doc.FirstChildElement("rss");
		XMLElement * pchannel = proot->FirstChildElement("channel");
		XMLElement * pitem = pchannel->FirstChildElement("item");
		while(pitem){
			XMLElement * ptitle = pitem->FirstChildElement("title");
			XMLElement * plink = pitem->FirstChildElement("link");
			XMLElement * pcontent = pitem->FirstChildElement("content:encoded");
			XMLElement * pdescription = pitem->FirstChildElement("description");

			string title;
			string link;
			string content;
			if(ptitle)
				title = string(ptitle->GetText());//修改了tinyxml中GetText的源码，以免当<title></title>这种情况时出bug
			if(plink)
				link = string(plink->GetText());
			if(pcontent)
				content = string(pcontent->GetText());
			else if(pdescription)
				content = string(pdescription->GetText());
			else
				cout << "no content find in " << every.c_str() << endl;

			//
			//if(docid>10&&docid<20)
			//	cout << content << endl;
			//

			//std::regex re("</?[^>]+>");
			boost::regex re("<.+?>");
			content = boost::regex_replace(content, re, string());
			
			string page = 
				"<doc><docid>" + std::to_string(docid) + 
				"</docid><url>"  + link + 
				"</url><title>" + title + 
				"</title><content>" + content + 
				"</content></doc>";
			_vecFilefiles.push_back(page);
			_offsetLib.insert(make_pair(docid,make_pair(itemplace,page.size())));

			++docid;
			pitem = pitem->NextSiblingElement();
			itemplace+=page.size();
		}
	}
}

void PageLib::store(){//存储网页库和位置偏移库
	ofstream ofspage(_conf.getConfigMap()["myripepagelib"]);
	for(auto every : _vecFilefiles){
		ofspage << every;
	}
	ofspage.close();

	ofstream ofsoffset(_conf.getConfigMap()["myoffsetlib"]);
	for(auto every : _offsetLib){
		ofsoffset << every.first << " " << every.second.first << " " << every.second.second << endl;
	}
	ofsoffset.close();
}
