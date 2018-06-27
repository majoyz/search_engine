 ///
 /// @file    WordQueryServer.cpp
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-06-27 10:55:11
 ///
 
#include "WordQueryServer.h"
#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

Wordqueryserver::Wordqueryserver(Configuration & conf):
    _conf(conf),
    _wqo(_conf),
    _tcpsever(_conf.getConfigMap()["IP"].c_str(),
              atoi(_conf.getConfigMap()["Port"].c_str())),
    _tp1(4,10)
{
    _tcpsever.setConnectionCallback(std::bind(&Wordqueryserver::onConnection,this,std::placeholders::_1));
    _tcpsever.setMessageCallback(std::bind(&Wordqueryserver::onMessage,this,std::placeholders::_1));
    _tcpsever.setCloseCallback(std::bind(&Wordqueryserver::onClose,this,std::placeholders::_1));
}
void Wordqueryserver::start()
{
    	_tp1.start();
	_tcpsever.start();
}
void Wordqueryserver::onConnection(const TcpConnectionPtr & conn)
{
    printf("%s\n",conn->toString().c_str());
    //conn->send("welcome to server\n");
}
void Wordqueryserver::onMessage(const TcpConnectionPtr & conn)
{
    string s = conn->receive();
    string data;
    int pos = s.find('\n');
    if(pos >= 0)
        data = s.substr(0,pos);
    else
        data = s;
    _tp1.addTask(std::bind(&Wordqueryserver::dotask,this,conn,data));
	cout<<"ceshi----11111"<<endl;
}
void Wordqueryserver::onClose(const TcpConnectionPtr & conn)
{
    printf("%s close.\n",conn->toString().c_str());
}
void Wordqueryserver::dotask(const TcpConnectionPtr & conn, const string & data)
{
	string result = _wqo.doQeury(data);
	cout<<"ceshi----22222"<<endl;
	cout<<result<<endl;
	conn->sendInLoop(result);	
}

}
