 ///
 /// @file    pageLib.h
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-21 16:40:48
 ///
 
#ifndef _WD_PAGELIB_H
#define _WD_PAGELIB_H

#include "configuration.h"
#include "dirScanner.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class PageLib{
	public:
		PageLib(Configuration & conf,DirScanner & dirScanner);//构造函数
		void create();//创建网页库
		void store();//存储网页库和位置偏移库
	private:
		Configuration & _conf;//配置文件对象的引用
		DirScanner & _dirScanner;//目录扫描对象的引用
		vector<string> _vecFilefiles;//存放格式化之后的网页的容器
		map<int,pair<int,int>> _offsetLib;//存放每篇文档在网页库的位置信息
};

#endif
