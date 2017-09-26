#include "A.h"

A::A()
{
    i = 0;
}

explicit A::A(int j)
{
    i = j;
}

virtual bool A::Serialize(ofstream & out)
{

}

virtual bool A::GetType(int & type)
{
    type = 0;
    return 0;
}

virtual ILSerializable * A::Deserialize(ifstream & in)
{

}
