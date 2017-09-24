#include "CB_LL.h"

CB_LL::CB_LL()
{
    b = 0;
    b_str = "";
}

void CB_LL::setString(string s)
{
    b_str = s;
}

void CB_LL::setFloat(float _b)
{
    b = _b;
}

bool CB_LL::Serialize(const char *pFilePath)
{
    //先写入b，在写入b_str
    ofstream out;
    out.open(pFilePath, ios::out | ios::trunc | ios::binary);
    Serialize(out);
    out.close();
}

bool CB_LL::Serialize(ofstream &out)
{
    unsigned int size;
    size = sizeof(b);
    out.write((char*)&size, sizeof(size));
    cout<<"serialize CB_LL:\n\tb:"<<b<<endl;
    out.write((char*)&b, size);
    size = b_str.size();
    out.write((char*)&size, sizeof(size));
    out.write(b_str.data(), size);
    cout<<"\tb_str:"<<b_str<<endl;
}
bool CB_LL::Deserialize(const char *pFilePath)
{
    ifstream in;
    in.open(pFilePath, ios::in | ios::binary);
    Deserialize(in);
    in.close();
}

bool CB_LL::Deserialize(ifstream &in)
{
    unsigned int size;
    in.read((char*)&size, sizeof(size));
    in.read((char*)&b, size);
    cout<<"deserialize CB_LL:\n\tb:"<<b<<endl;
    in.read((char*)&size, sizeof(size));
    char *buf = new char[size+1];
    in.read(buf, size);
    buf[size] = '\0';
    b_str.assign(buf);
    cout<<"\tb_str:"<<b_str<<endl;
}
