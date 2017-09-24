#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class CB_LL
{
private:
    string b_str;
    float b;
public:
    CB_LL();
    void setString(string s);
    void setFloat(float _b);
    bool Serialize(const char *pFilePath);
    bool Deserialize(const char *pFilePath);
    bool Serialize(ofstream &out);
    bool Deserialize(ifstream &in);
};
