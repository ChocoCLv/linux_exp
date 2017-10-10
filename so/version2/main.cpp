#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <string>
using namespace std;
typedef void (*Print)();
int main()
{
    DIR * dir;
    struct dirent *ptr;
    Print print;
    void * handle;
    dir = opendir("./plugin");
    while((ptr = readdir(dir)) != NULL)
    {
        string so_path, so_name;
        so_name.assign(ptr->d_name);
        
        if(so_name.find(".so\0", 0) == string::npos)
            continue;

        so_path = "./plugin/" + so_name;
        handle = dlopen(so_path.data(), RTLD_LAZY);
        if(handle == NULL)
        {
            cout<<dlerror()<<endl;
            continue;
        }
        print = (Print)dlsym(handle, "print");
        if(print == NULL)
        {
            cout<<dlerror()<<endl;
            continue;
        }
        print();
        dlclose(handle);
    }
    closedir(dir);
    return 0;
}
