#pragma once
#ifndef __WORDSEGMENT_H__
#define __WORDSEGMENT_H__

#include "cppjieba1/Jieba.hpp"
#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <vector>
using std::vector;

const char * const DICT_PATH = "./data/jiebadict/jieba.dict.utf8";
const char * const HMM_PATH = "./data/jiebadict/hmm_model.utf8";
const char * const USER_DICT_PATH = "./data/jiebadict/user.dict.utf8";
const char * const IDF_PATH = "./data/jiebadict/idf.utf8";
const char * const STOP_WORD_PATH = "./data/jiebadict/stop_words.utf8";

class Word_segment
{
    cppjieba::Jieba _jieba;
public:
    Word_segment()
        :_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
    {
//        cout<<"jieba init done\n";
    }
    vector<string> operator()(const char * src)
    {
        vector<string> words;
        _jieba.Cut(src,words,true);
        return words;
    }

};

#endif
