#include "Serializer.h"
#include <vector>
using namespace std;

int 
main(int argc, char* args[])
{
    if(argc != 2)
        return -1;

    Serialized s1, s2;
    CA_LL a;
    CB_LL b;
    
    a.setI(20);
    b.setFloat(3.14);
    b.setString("hello");

    s1.nType = 1;
    s1.pObj = &a;
    
    s2.nType = 2;
    s2.pObj = &b;

    vector<Serialized> v;
    v.push_back(s1);
    v.push_back(s2);

    Serializer s;
    s.Serialize(args[1], v);
    s.Deserialize(args[1], v);
    return 0;
}
