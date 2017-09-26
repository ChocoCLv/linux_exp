#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class ILSerializable
{
public:
    virtual bool Serialize(ofstream & out) = 0;
    virtual ILSerializable* Deserialie(ifstream & in) = 0;
    virtual bool GetType(int & type) = 0;

public:
    ILSerializable()
    {
    }

    virtual ~ILSerializable()
    {
    }

};
