#include "A.h"

A::A()
{
    i = 0;
}

A::A(int j)
{
    i = j;
}

bool A::Serialize(ofstream & out)
{
  	unsigned int size;
    size = sizeof(i);
    out.write((char*)&size, sizeof(size));
    cout<<"serialize size:"<<size<<endl;
    out.write((char*)&i, size);
    cout<<"serialize A.i:"<<i<<endl;
}

bool A::GetType(int & type)
{
    type = 1;
    return 0;
}

ILSerializable * A::Deserialize(ifstream & in)
{
	A *a = new A();
	unsigned int size;
    in.read((char*)&size, sizeof(size));
    cout<<"deserialize size:"<<size<<endl;
    in.read((char*)&(a->i), size);
    cout<<"deserialize A.i:"<<a->i<<endl;
    return a;
}
