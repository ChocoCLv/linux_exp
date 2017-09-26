#include "Serializer.h"


bool Serializer::Serialize(const char* pFilePath, vector<Serialized>& v)
{
    ofstream out;
    out.open(pFilePath, ios::out | ios::trunc | ios::binary);
    int i = 0;
    int vector_size = v.size();
    out.write((char*)&vector_size, sizeof(vector_size));
    for(; i < v.size(); i++)
    {
        int type = v[i].nType;
        out.write((char*)(&type), sizeof(int));
        if(1 == v[i].nType)
        {
            CA_LL *a = (CA_LL*)v[i].pObj;
            a->Serialize(out);
        }
        else if(2 == v[i].nType)
        {
            CB_LL *b = (CB_LL*)v[i].pObj;
            b->Serialize(out);
        }
    }
    out.close();
}

bool Serializer::Deserialize(const char* pFilePath, vector<Serialized>& v)
{
    ifstream in;
    in.open(pFilePath, ios::in | ios::binary);
    int i = 0;
    int vector_size;

    in.read((char*)&vector_size, sizeof(int));
    printf("vector_size:%d", vector_size);
    for(; i< vector_size; i++)
    {
        int type;
        Serialized s;
        CA_LL a;
        CB_LL b;
        in.read((char*)(&type),sizeof(int));
        s.nType = type;
        if(1 == s.nType)
        {
            a.Deserialize(in);
            s.pObj = &a;
        }
        else if(2 == s.nType)
        {
            b.Deserialize(in);
            s.pObj = &b;
        }
        v.push_back(s);
    }
    in.close();
}
