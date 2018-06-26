 ///
 /// @file    pageLibPreprocessor.h
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-22 15:06:05
 ///
 
#ifndef _WD_PAGELIBPREPROCESSOR_H
#define _WD_PAGELIBPREPROCESSOR_H

#include "configuration.h"
#include "webPage.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class PageLibPreprocessor{
	public:
		PageLibPreprocessor(Configuration & conf);//构造函数
		void doProcess();//执行预处理
	private:
		void readInfoFromFile();//根据配置信息读取网页库和位置偏移库的内容
		void cutRedundantPages();//对冗余的网页进行去重
		void buildInvertIndexTable();//创建倒排索引表
		void storeOnDisk();//将经过预处理之后的网页库、位置偏移库和倒排索引表写回到磁盘上
	private:
		Configuration & _conf;//配置文件对象的引用
		Word_segment _jieba;//分词对象
		vector<WebPage> _pageLib;//网页库的容器对象
		unordered_map<int,pair<int,int>> _offsetLib;//网页偏移库对象
		unordered_map<string,vector<pair<int,double>>> _invertIndexTable;//倒排索引表对象
};

#endif
