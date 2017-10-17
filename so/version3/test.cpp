#include "PluginManager.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char * args[])
{
    int func_id;
    if(argc != 2){
        cout<<"need command"<<endl;
        return -1;
    }
    PluginManager pl;
    pl.ParsePluginDir();
    pl.ParseFunction();
    if(strcmp(args[1], "help") == 0)
    {
        cout<<"debug: print help msg"<<endl;
        pl.Help();
    }
    else
    {
        func_id = atoi(args[1]);
        pl.CallFunc(func_id);
    }
    return 0;
}
