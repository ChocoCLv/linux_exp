#include <fstream>
#include <iostream>

using namespace std;

class CA_LL
{
private:
    int i;
public:
    CA_LL();
    void setI(int _i);
    bool Serialize(const char *pFilePath);
    bool Deserialize(const char *pFilePath);
    bool Serialize(ofstream &out);
    bool Deserialize(ifstream &in);
};
