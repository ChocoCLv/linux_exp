#include <fstream>
#include <string.h>
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

    char text[10][10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    unsigned int target_bytes = atoi(args[2]);
    char * filename = args[1];
    unsigned int num = target_bytes/50 + 1;
    ofstream out;
    

    out.open(filename, ios::trunc | ios::trunc | ios::binary);
    char c = ' ';
    for(unsigned int i = 0; i < num; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            out.write(text[j], strlen(text[j]));
            out.write(&c, 1);
        }
    }
    out.close();
    return 0;
}
