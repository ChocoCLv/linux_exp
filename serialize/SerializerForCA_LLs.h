#include <iostream>
#include <fstream>
#include <vector>
#include "CA_LL.h"

using namespace std;

class SerializerForCA_LLs
{

public:
    bool Serialize(const char *pFilePath, std::vector<CA_LL> &v);
    bool Deserialize(const char *pFilePath, std::vector<CA_LL> &v);
    void genVector(std::vector<CA_LL> &v);
};
