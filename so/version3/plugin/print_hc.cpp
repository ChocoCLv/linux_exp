#include <iostream>
using namespace std;

const int FUNC_ID =1;

extern "C" void Print()
{
    cout<<"hello China"<<endl;
}

extern "C" void Help()
{
    cout<<"funcid: "<<FUNC_ID<<"\tprint(\"hello China\")"<<endl;
}

extern "C" int GetID()
{
    return FUNC_ID;
}
