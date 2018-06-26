 ///
 /// @file    conf.h
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-14 11:02:41
 ///
 
#ifndef __WD_CONF_H__
#define __WD_CONF_H__

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <set>
using namespace std;

class Configuration{
	public:
		Configuration(const string & filepath);
		map<string,string> & getConfigMap();//获取存放配置文件内容的map
		set<string> getStopWordList();//获取停用词词集
	private:
		string _filepath;//配置文件路径
		map<string,string> _configMap;//配置文件内容
		set<string> _stopWordList;//停用词词集
};

#endif
