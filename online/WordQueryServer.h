 ///
 /// @file    WordQueryServer.h
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-06-27 10:50:13
 ///
 
#ifndef __WORDQUERYSERVER_H__
#define __WORDQUERYSERVER_H__
#include "configuration.h"
#include "WordQuery.h"
#include "TcpServer.h"
#include "Threadpool.h"

namespace wd
{

class Wordqueryserver
{
    Configuration _conf;//配置类对象
    wordQuery _wqo;//查询类对象
    TcpServer _tcpsever;//TCP通信对象
    Threadpool _tp1;//执行文本纠错结果的缓存线程类
    void onConnection(const TcpConnectionPtr & conn);
    void onMessage(const TcpConnectionPtr & conn);
    void onClose(const TcpConnectionPtr & conn);
	void dotask(const TcpConnectionPtr & conn, const string & msg);//该方法由线程池的某一个线程执行
public:
    Wordqueryserver(Configuration & conf);//构造函数
    void start();//开始提供服务
};

}
#endif
