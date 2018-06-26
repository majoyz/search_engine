 ///
 /// @file    test.cpp
 /// @author  majoyz(zmj-miss@live.com)
 /// @date    2018-05-22 11:21:28
 ///
 
#include "pageLibPreprocessor.h"
#include "pageLib.h"
using namespace std;

int main(){
	cout << "what?" << endl;
	Configuration conf("../conf/my.conf");
#if 1
	DirScanner ds(conf);
	ds();
	PageLib pl(conf,ds);

	pl.create();
	pl.store();
	cout << "old pagelib store finished" << endl;
#endif
	cout << "before PageLibPreprocessor()" << endl;
	PageLibPreprocessor pageLibPro(conf);
	cout << "before doProcess" << endl;
	pageLibPro.doProcess();

	cout << "new pagelib finish" << endl;//搞定去重

	return 0;
}
