#include <dirent.h>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <dlfcn.h>
#include <iostream>
using namespace std;
typedef void (*PrintFunc)();
typedef void (*HelpFunc)();
typedef int (*GetIDFunc)();

typedef struct FunctionSo
{
    PrintFunc Print;
    HelpFunc Help;
    GetIDFunc GetID;
}FuncSo;

class PluginManager
{
private:
    map<int, FuncSo> m_funcSo;
    vector<string> v_soPath;

public:
    void ParsePluginDir();
    void ParseFunction();
    void Help();
    void CallFunc(int funcId);
};
