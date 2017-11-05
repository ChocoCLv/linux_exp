#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* args[])
{
    if(argc != 3)
    {
        printf("parameter error\n");
        return -1;
    }
    unsigned int target_bytes = atoi(args[2]);
    char * filename = args[1];
    unsigned int num = target_bytes/sizeof(int);
    ofstream out;
    

    out.open(filename, ios::trunc | ios::trunc | ios::binary);

    for(unsigned int i = 0; i < num; i++)
    {
        int data = i % 100;
        out.write((char*)&data, sizeof(data));
    }
    out.close();
    return 0;
}
