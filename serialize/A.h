#include <fstream>
#include "ILSerializable.h"

using namespace std;

class A : public ILSerializable
{
public:
    virtual bool Serialize(ofstream & out)
    virtual ILSerialzable *Deserialize(ifstream & in);
    virtual int GetType(int & type);
    
    A();
    explicit A(int j);
private:
    int i;
};    
