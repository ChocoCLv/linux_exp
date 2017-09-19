#include "CA_LL.h"

CA_LL::CA_LL()
{
    i = 0;
}

void CA_LL::setI(int _i)
{
    i = _i;
}

bool CA_LL::Serialize(const char *pFilePath)
{
    ofstream out;
    unsigned int size;
    out.open(pFilePath, ios::out | ios::trunc | ios::binary);
    size = sizeof(i);
    out.write((char*)&size, sizeof(size));
    out.write((char*)&i, size);
    cout<<"serialize i:"<<i<<endl;
    out.close();    
}

bool CA_LL::Serialize(ofstream &out)
{
    unsigned int size;
    size = sizeof(i);
    out.write((char*)&size, sizeof(size));
    out.write((char*)&i, size);
    cout<<"serialize i:"<<i<<endl;
}
bool CA_LL::Deserialize(const char *pFilePath)
{
    ifstream in;
    unsigned int size;
    in.open(pFilePath, ios::in | ios::binary);
    in.read((char*)&size, sizeof(size));
    cout<<"size:"<<size<<endl;
    in.read((char*)&i, size);
    cout<<"deserialize i:"<<i<<endl;
    in.close();
}

bool CA_LL::Deserialize(ifstream &in)
{
    unsigned int size;
    in.read((char*)&size, sizeof(size));
    cout<<"size:"<<size;
    in.read((char*)&i, size);
    cout<<"deserialize i:"<<i<<endl;
}
