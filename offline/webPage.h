 ///
 /// @file    webPage.h
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-22 15:35:02
 ///
 
#ifndef _WD_WEBPAGE_H
#define _WD_WEBPAGE_H

#include "configuration.h"
#include "WordSegment.h"

#include <string>
#include <vector>
#include <map>

class WebPage{//网页类
	public:
		WebPage(string & doc,Configuration & conf,Word_segment & jieba);//构造函数
		int getDocId();//获取文档的docid
		string getDoc();//获取文档
		map<string,int> & getWordMap();//获取文档的词频统计map
		uint64_t getSimhash();
	private:
		void processDoc(const string & doc,Configuration & configi,Word_segment & jieba);//对格式化文档进行处理
		void calcTopK(vector<string> & wordsVec,int k,set<string> & stopWordList);//求取文档的topk词集
	private:
		string _doc;//整篇文档，包含xml在内
		int _docId;//文档id
		string _docTitle;//文档标题
		string _docUrl;//文档URL
		string _docContent;//文档内容
		uint64_t _simhash;
		string _docSummary;//文档摘要，需自动生成，不是固定的
		vector<string> _topWords;//词频最高的前20个词
		map<string,int> _wordsMap;//保存每篇文档的所有词语和词频，不包括停用词

	const static int TOPK_NUMBER = 20;
	friend bool operator==(WebPage & lhs,WebPage & rhs);//判断两篇文档是否相等
	friend bool operator<(const WebPage & lhs,const WebPage & rhs);//对文档按Docid进行排序
};

#endif
