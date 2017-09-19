#include "SerializerForCA_LLs.h"

void SerializerForCA_LLs::genVector(std::vector<CA_LL> &v)
{
    int n = 0;
    for(; n < 10; n++)
    {
        CA_LL ca;
        ca.setI(n);
        v.push_back(ca);
    }
}

bool SerializerForCA_LLs::Serialize(const char *pFilePath, std::vector<CA_LL> &v)
{
    ofstream out;
    out.open(pFilePath, std::ios::out | std::ios::app| std::ios::binary);
    unsigned int vector_size = v.size();
    out.write((char*)&vector_size, sizeof(unsigned int));
    int n = 0;
    for(; n < vector_size; n++)
    {
        v[n].Serialize(out);
    }
    return true;
}

bool SerializerForCA_LLs::Deserialize(const char *pFilePath, std::vector<CA_LL> &v)
{
    ifstream in;
    in.open(pFilePath, std::ios::in | std::ios::binary);
    unsigned int vector_size = 0;
    in.read((char*)&vector_size, sizeof(unsigned int));
    cout<<"vector size:"<<vector_size<<endl;
    int n = 0;
    for(; n < vector_size; n++)
    {
        CA_LL ca;
        ca.Deserialize(in);
        v.push_back(ca);
    }
    return true;
}
