#include <dirent.h>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <dlfcn.h>
#include <iostream>
using namespace std;
typedef void (*Print)();
typedef void (*Help)();
typedef int (*GetId)();

typedef struct FunctionSo
{
    Print print;
    Help help;
    GetId getID;
}FuncSo;

class PluginManager
{
private:
    map<int, FuncSo> m_funcSo;
    vector<string> v_soPath;

public:
    void parsePluginDir();
    void parseFunction();
    void help();
    void callFunc(int func_id);
};
