#include <unistd.h>
#include <map>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define READ_PROC_NUM 1
#define COUNT_PROC_NUM 1
#define BUFF_SIZE 4092
#define MAX_READ_SIZE 4046

using namespace std;

//from read process to count process
//file data
int work_pipe[2];

//from count process to parent process
//words num
int result_pipe[2];

typedef struct count_result_buff{
    unsigned int size;
    unsigned int times;
    char word[50]; //max word length 46
}count_result;

typedef struct file_data_buff
{
    int data_size;
    char data[BUFF_SIZE];
}data_buff;

typedef struct dir_info{
    int file_num;
    char file[100][100];//dir depth is 3
}dir_file_list;

dir_file_list file_list;

int is_partof_word(char c)
{
    return (nc >= 'a' || nc <= 'z' || nc >= 'A' ||
                 nc <= 'Z' || nc == '-' || nc == '_')
}

void read_proc(int offset)
{
    int fd_read;
    int seq = offset;
    int size_read;
    data_buff buf;
    while(seq < file_list.filenum)
    {
        fd_read = open(file_list.file[seq], O_RDONLY);
        if(fd_read == -1)
        {
            printf("open file read error");
            seq += READ_PROC_NUM;
            continue;
        }
        while(1)
        {
            size_read = read(fd_read, buf.data, MAX_READ_SIZE);
            if(size_read == 0)
            {
                printf("read process %lu : exit\n", tid);
                break;
            }
            else if(size_read == -1)
            {
                printf("read process %lu : read error\n", tid);
                break;
            }
            else
            {
                buf.data_size = size_read;
                char nc = '-';
                while(is_partof_word(nc))
                {
                    size_read = read(fd_read, &nc, 1)
                    if(size_read == 0)
                        break;
                    if(is_partof_word(nc))
                    {
                        
                    }
                }
                write(work_pipe[1], (void*)&buf, sizeof(buf)); 
            }
        }
    }
}

void count_proc()
{
    int n;
    data_buff *buf;
    while(1)
    {
        n = read(fd_pipe[0], (void*)&buf, sizeof(buf));
        if(n == -1)
        {
            printf("write thread %lu pipe read error\n", tid);
            pthread_exit((void*)1);
            break;
        }
        else if(n == 0)
        {
            printf("write thread %lu : write times: %d\n", tid,write_times);
            pthread_exit((void*)2);
            break;
        }
        else
        {
            
        }
    }
}

void read_file_list(char * base_path)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if((dir = opendir(base_path)) == NULL)
    {
        printf("open dir error\n");
        exit(1);
    }

    while((ptr = readdir(dir)) != NULL)
    {
        //get absolute path
        memset(base, 0, sizeof(base));
        strcpy(base,base_path);
        strcat(base,"/");
        strcat(base,ptr->d_name);
        //printf("%s\n", base);
        
        if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        else if(ptr->d_type == 8){
            //file
            if(file_list.file_num >= 100)
                return;
            if(strlen(base) >= 100)
                continue;
            strcpy(file_list.file[file_list.file_num], base);    
            file_list.file_num++;
        }else if(ptr->d_type == 4){
            //dir
            read_file_list(base);
        }
    }
    closedir(dir);
    return;
}

void print_file_list()
{
    int i = 0;
    for(i = 0; i < file_list.file_num; i++)
    {
        printf("%s\n", file_list.file[i]);
    }
}

int main(int argc, char * args[])
{
    pid_t read_pid[READ_PROC_NUM];
    pid_t count_pid[COUNT_PROC_NUM];
    int i;
    map<string, int> result;

    if(argc !=2)
    {
        printf("parameter error");
        return -1;
    }

    read_file_list(args[1]);
    print_file_list();

    
    if(pipe(work_pipe) < 0){
        printf("pipe error\n");
        return -1;
    }

    if(pipe(result_pipe) < 0){
        printf("pipe error\n");
        return -1;
    }

    for(i = 0; i < READ_PROC_NUM; i++)
    {
        if((read_pid[i] = fork()) == -1){
            printf("fork error\n");
            return -1;
        }else if(read_pid[i] == 0){
            //read child process
            //read text to pipe        
            read_proc(i); 
            return 0;//not excute follow code
        }
    }

    for(i = 0; i < COUNT_PROC_NUM; i++)
    {
        if((count_pid[i] = fork()) == -1){
            printf("fork error\n");
        }else if(count_pid[i] == 0){
            //count child process
            //read text from pipe and count words
            count_proc();
            return 0;
        }
    }

    //save result to map


    //write final result in file
    


    //wait read process exit
    for(i = 0; i < READ_PROC_NUM; i++)
    {
       if(waitpid(read_pid[i], NULL, 0) != read_pid[i]){
           printf("waitpid error\n");
       }
    }

    //wait count process exit
    for(i = 0; i < COUNT_PROC_NUM; i++)
    {
        if(waitpid(count_pid[i], NULL, 0) != count_pid[i]){
            printf("waitpid error\n");
        }
    }
    return 0;
}

