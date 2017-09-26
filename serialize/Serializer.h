#include <fstream>
#include <vector>
#include "CA_LL.h"
#include "CB_LL.h"

struct Serialized
{
    int nType;
    void * pObj;
};

using namespace std;

class Serializer
{
public:
    bool Serialize(const char* pFilePath, vector<Serialized>& v);
    bool Deserialize(const char* pFilePath, vector<Serialized>& v);
};

