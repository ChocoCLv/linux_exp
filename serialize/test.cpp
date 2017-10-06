#include "CLSerializer.h"
#include "A.h"
#include "ILSerializable.h"
#include <vector>
using namespace std;

int 
main(int argc, char* args[])
{
    if(argc != 2)
        return -1;

    A a1(1);
    A a2(2);

    std::vector<ILSerializable*> v;
    v.push_back(&a1);
    v.push_back(&a2);

    CLSerializer s;
    s.Serialize(args[1], v);
    A a3, a4;
    s.regist(&a3);
    s.Deserialize(args[1], v);
    
    return 0;
}
