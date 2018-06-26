///
/// @file    conf.cc
/// @author  majoyz(zmj-miss@live.com)
/// @date    2018-05-14 11:05:59
///

#include "configuration.h"

Configuration::Configuration(const string & filepath)
	:_filepath(filepath)
{
	fstream fs(_filepath);
	if(!fs.is_open()){
		cout << "open false" << endl;
		return;
	}
	char tmp[256] = {0};
	string tmpkey;
	string tmpvalue;
	while(!fs.eof()){
		fs.getline(tmp,256);
		string line(tmp);
		size_t pos = line.find("=");
		if(pos == string::npos){
			cout << "read finish or false" << endl;
			return;
		}
		tmpkey = line.substr(0,pos);
		tmpvalue = line.substr(pos+1);
		_configMap.insert(make_pair(tmpkey,tmpvalue));
	}//配置文件内容获得

	string stopWordsEPath = _configMap["mystopewords"];
	string stopWordsZPath = _configMap["mystopzwords"];
	fstream efs(stopWordsEPath);
	if(!efs.is_open()){
		cout << "open false" << endl;
		return;
	}
	while(!efs.eof()){
		efs.getline(tmp,256);
		string stopword(tmp);
		_stopWordList.insert(stopword);
	}//获取英文停用词词集
	fstream zfs(stopWordsZPath);
	if(!zfs.is_open()){
		cout << "open false" << endl;
		return;
	}
	while(!zfs.eof()){
		zfs.getline(tmp,256);
		string stopword(tmp);
		_stopWordList.insert(stopword);
	}//获取中文停用词词集
}

map<string,string> & Configuration::getConfigMap(){
	return _configMap;
}

set<string> Configuration::getStopWordList(){
	return _stopWordList;
}
