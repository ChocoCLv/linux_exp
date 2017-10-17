#include "PluginManager.h"

using namespace std;

void PluginManager::parsePluginDir()
{
    struct dirent * ptr;
    DIR * dir;
    dir = opendir("./plugin");
    while((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name, ".") == 0)
            continue;
        if(strcmp(ptr->d_name,"..") == 0)
            continue;
        if(strstr(ptr->d_name, ".so") == NULL)
            continue;
        string so_path, so_name;
        so_name.assign(ptr->d_name);
        so_path = "./plugin/" + so_name;
        v_soPath.push_back(so_path);
        cout<<"get so path: "<<so_path<<endl;
    }
}

void PluginManager::parseFunction()
{
    void * handle;
    for(int i = 0; i < v_soPath.size(); i++)
    {
        cout<<"parse so: "<<v_soPath[i]<<endl;
        FuncSo funso;
        handle = dlopen(v_soPath[i].data(), RTLD_LAZY);
        if(handle == NULL)
        {
            cout<<"get handle error: "<< dlerror()<<endl;
            return;
        }
        funso.print = (Print)dlsym(handle, "Print");
        if(funso.print == NULL)
        {
            cout<<"get print error: "<<dlerror()<<endl;
            return;
        }
        funso.help = (Help)dlsym(handle, "Help");
        if(funso.help == NULL)
        {
            cout<<"get help error: "<< dlerror()<<endl;
            return;
        }

        funso.getID = (GetId)dlsym(handle, "GetID");
         if(funso.getID == NULL)
        {
            cout<<"get getID error: "<< dlerror()<<endl;
            return;
        }
        m_funcSo[funso.getID()] = funso;      
    }
}

void PluginManager::help()
{
    map<int, FuncSo>::iterator it;
    for(it = m_funcSo.begin(); it != m_funcSo.end(); it++)
    {
        it->second.help();
    }
}      

void PluginManager::callFunc(int func_id)
{
    if(m_funcSo.find(func_id) == m_funcSo.end())
    {
        cout<<"func not found"<<endl;
        return;
    }
    m_funcSo[func_id].print();
}
