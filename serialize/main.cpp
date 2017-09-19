#include "SerializerForCA_LLs.h"
    
int
main(int argc, char *args[])
{
    if(argc != 2){
        return 1;
    } 
    //CA_LL ca1, ca2;
    //ca1.setI(11);
    //ca1.Serialize(args[1]);
    //ca2.Deserialize(args[1]);
    SerializerForCA_LLs s;
    std::vector<CA_LL> v1,v2;
    s.genVector(v1);
    s.Serialize(args[1], v1);
    s.Deserialize(args[1], v2);
    return 0;
}
