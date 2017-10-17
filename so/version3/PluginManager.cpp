#include "PluginManager.h"

using namespace std;

void PluginManager::ParsePluginDir()
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
    closedir(dir);
}

void PluginManager::ParseFunction()
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
        funso.Print = (PrintFunc)dlsym(handle, "Print");
        if(funso.Print == NULL)
        {
            cout<<"get print error: "<<dlerror()<<endl;
            return;
        }
        funso.Help = (HelpFunc)dlsym(handle, "Help");
        if(funso.Help == NULL)
        {
            cout<<"get help error: "<< dlerror()<<endl;
            return;
        }

        funso.GetID = (GetIDFunc)dlsym(handle, "GetID");
         if(funso.GetID == NULL)
        {
            cout<<"get getID error: "<< dlerror()<<endl;
            return;
        }
        m_funcSo[funso.GetID()] = funso;      
    }
}

void PluginManager::Help()
{
    map<int, FuncSo>::iterator it;
    for(it = m_funcSo.begin(); it != m_funcSo.end(); it++)
    {
        it->second.Help();
    }
}      

void PluginManager::CallFunc(int funcId)
{
    if(m_funcSo.find(funcId) == m_funcSo.end())
    {
        cout<<"func not found"<<endl;
        return;
    }
    m_funcSo[funcId].Print();
}
