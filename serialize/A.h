#ifndef A_H
#define A_H
#include <fstream>
#include "ILSerializable.h"

using namespace std;

class A : public ILSerializable
{
public:
    virtual bool Serialize(ofstream & out);
    virtual ILSerializable *Deserialize(ifstream & in);
    virtual bool GetType(int & type);
    
    A();
    explicit A(int j);
private:
    int i;
};    
#endif
