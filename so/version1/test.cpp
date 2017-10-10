#include <dlfcn.h>
#include <iostream>
using namespace std;
int main()
{
    void * handle = dlopen("./libtest.so", RTLD_LAZY);
    if(handle == NULL)
    {
        cout<<"open libtest so error!"<<endl;
        return -1;
    }

    typedef void (*Func)();
    Func f;
    f = (Func)dlsym(handle, "f");
    if(f == NULL)
    {
        cout<<dlerror()<<endl;
        return -1;
    }
    f();    
    dlclose(handle);
    return 0;
}


