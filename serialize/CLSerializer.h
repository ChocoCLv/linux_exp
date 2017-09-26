#include "ILSerializable.h"
#include <vector>
#include <map>

using namespace std;

class CLSerializer
{
public:
    bool Serialize(const char * pFilePath, vector<ILSerializable> &v);
    bool Deserialize(const char * pFilePath, vector<ILSerializable> &v);
    
    bool regist(ILSerializable * p);

private:
    map<int, ILSerializable *> m_vSerialized;
};    
 
