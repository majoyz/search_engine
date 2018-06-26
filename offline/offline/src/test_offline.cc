

//Author:liuiuge(1220595883@qq.com

#include "Pagelib.h"
#include "DirScanner.h"
#include "Pagelibpreprocessor.h"
#include <time.h>
#include <iostream>

int main()
{
    My_conf conf("./conf/myconf.cfg");
    // conf.debug();
#if 0
    set<string> s1 = conf.get_stopwordlist();
    for(auto & it:s1)
	std::cout<<it<<std::endl;
#endif
    Dir_scanner dirscanner(conf);
    dirscanner();
    //dirscanner.debug();
    Pagelib page1(conf,dirscanner);
    time_t t1 = time(NULL);
    page1.create();
    page1.store();
    time_t t2 = time(NULL);
    std::cout<<"raw lib create and store time: "<<t2-t1<<"min\n";
    PageLibPreProcessor plpp1(conf);
    plpp1.doProcess();

    return 0;
}
