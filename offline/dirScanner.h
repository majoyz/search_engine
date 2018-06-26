 ///
 /// @file    dirScanner.h
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-21 16:51:42
 ///
 
#ifndef _WD_DIRSCANNER_H
#define _WD_DIRSCANNER_H

#include "configuration.h"

#include <vector>

using std::vector;

class DirScanner{//目录扫描类
	public:
		DirScanner(Configuration & conf);//构造函数
		void operator()();//重载函数调用运算符，调用traverse函数
		vector<string> files();//返回_vecFilefiles的引用
		void traverse(const string & dirname);//获取某一目录下的所有文件
	private:
		Configuration & _conf;//配置文件对象的引用
		vector<string> _vecFilefiles;//存放每个语料文件的绝对路径
};

#endif
