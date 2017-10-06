#include "CLSerializer.h"

bool CLSerializer::Serialize(const char * pFilePath, vector<ILSerializable*> &v)
{
	ofstream out;
	int vector_size = v.size();
	out.open(pFilePath, ios::out | ios::trunc | ios::binary);
	out.write((char*)&vector_size, sizeof(vector_size));
	for(int i = 0; i < vector_size; i++)
	{
		int type;
		v[i]->GetType(type);
		cout<<"Serialize type: "<<type<<endl;
		out.write((char*)&type, sizeof(int));
		v[i]->Serialize(out);
	}
	out.close();
}


bool CLSerializer::Deserialize(const char *pFilePath, vector<ILSerializable*> &v)
{
	ifstream in;
    in.open(pFilePath, ios::in | ios::binary);
    
    int vector_size;

    in.read((char*)&vector_size, sizeof(int));
    printf("vector_size:%d\n", vector_size);
    for(int i = 0; i< vector_size; i++)
    {
        int type;
        in.read((char*)(&type),sizeof(int));
        cout<<"Deserialize type: "<<type<<endl;
        ILSerializable *p = m_vSerialized[type]->Deserialize(in);
        v.push_back(p);
    }
    in.close();
}

bool CLSerializer::regist(ILSerializable * p)
{
	int type;
	p->GetType(type);
	m_vSerialized[type] = p;
}
