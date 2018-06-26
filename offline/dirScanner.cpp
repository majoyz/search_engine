 ///
 /// @file    dirScanner.cpp
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-21 20:00:37
 ///
 
#include "dirScanner.h"
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

DirScanner::DirScanner(Configuration & conf)
:_conf(conf)
{}

void DirScanner::operator()(){
	this->traverse(_conf.getConfigMap()["myrssrootdir"]);
}

vector<string> DirScanner::files(){
	return _vecFilefiles;
}

void DirScanner::traverse(const string & dirname){
	for(directory_iterator dit(dirname);dit != directory_iterator();++dit){//空的构造函数生成一个逾尾end迭代器
		if(is_regular_file(*dit)){
			cout << dit->path().filename() << endl;
			_vecFilefiles.push_back(dit->path().string());
		}
	}
}
