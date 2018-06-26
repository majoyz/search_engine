#include "DirScanner.h"
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <iostream>

Dir_scanner::Dir_scanner(My_conf & conf)
    :_conf(conf)
{std::cout<<">Dir_scanner\n";}
vector<string>  & Dir_scanner::files()
{
    return _vecfiles;
}
void Dir_scanner::operator()()
{
    map<string,string> m1= _conf.get_config_map();
    string dirname = m1["DirScanner"];
    std::cout<<">scanning dir"<<dirname<<'\n';
    traverse(dirname);
}
void Dir_scanner::traverse(const string & dirname)
{
    struct dirent * mydirent;
    DIR * dir1 = opendir(dirname.c_str());
    if(NULL == dir1)
    {
        std::cout<<"dir open error\n";
        exit(EXIT_FAILURE);
    }
    chdir(dirname.c_str());
    while(NULL!=(mydirent = readdir(dir1)))
    {
        if(mydirent->d_type == 4)
        {
            if(mydirent->d_name[0] != '.'){
                std::cout<<"opening dir "<<mydirent->d_name<<"\n    ";
                traverse(mydirent->d_name);
            }
        }
        else
        {
            string filename;
            filename.append(getcwd(NULL,0))
                .append("/")
                .append(mydirent->d_name);
            _vecfiles.push_back(filename);
        }
    }
    chdir("..");
    closedir(dir1);
}
void Dir_scanner::debug()
{
    for(auto & it:_vecfiles)
        std::cout<<it<<std::endl;
}
#ifdef HELLO
class Dir_scanner
{
    vector<string> _vecFilesfiles;
    My_conf _conf;
public:
    Dir_scanner(My_conf & conf);
    vector<string> files();
    void traverse(const string & dirname);
        
};

#endif
