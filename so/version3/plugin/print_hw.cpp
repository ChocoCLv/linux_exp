#include <iostream>

using namespace std;

const int FUNC_ID = 2;

extern "C" void Print()
{
    cout<<"hello world"<<endl;
}

extern "C" void Help()
{
    cout<<"funcid: "<<FUNC_ID<<"\tprint(\"hello world\")"<<endl;
}

extern "C" int GetID()
{
    return FUNC_ID;
}
