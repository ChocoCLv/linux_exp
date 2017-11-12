#include <unistd.h>
#include <map>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define READ_PROC_NUM 2
#define COUNT_PROC_NUM 2
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
    unsigned int times;
    char data[50]; //max word length 46
}count_result;

//total size is 4096
//ensure automic operation
typedef struct file_data_buff
{
    int data_size;
    char data[BUFF_SIZE];
}data_buff;

//save file path
//max file num is 100
//max file name length is 100
typedef struct dir_info{
    int file_num;
    char file[100][100];
}dir_file_list;

dir_file_list file_list;

//a word can contain uppercase letters, lowercase letters, '-', and '_'
int is_partof_word(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') 
        || c == '-' || c == '_');
}

//read process funtion
//one process read one file
void read_proc(int offset)
{
    //close unnecessary pipe port
    close(work_pipe[0]);
    close(result_pipe[0]);
    close(result_pipe[1]);

    int fd_read;
    int seq = offset;
    int size_read;
    unsigned long pid = (unsigned long)getpid();
    data_buff buf;

    while(seq < file_list.file_num)
    {
        fd_read = open(file_list.file[seq], O_RDONLY);
        if(fd_read == -1)
        {
            printf("open file read error : %s\n", file_list.file[seq]);
            seq += READ_PROC_NUM;
            continue;
        }

        printf("read process %lu read file : %s\n", pid, file_list.file[seq]);

        //read one file data to work_pipe
        while(1)
        {
            size_read = read(fd_read, buf.data, MAX_READ_SIZE);
            if(size_read == 0)
            {
                printf("read process : read file: %s over\n", file_list.file[seq]);
                break;
            }
            else if(size_read == -1)
            {
                printf("read process : read file: %s error\n", file_list.file[seq]);
                break;
            }
            else
            {
                buf.data_size = size_read;
                char nc;

                //ensure datablock does not divide a word
                while(1)
                {
                    size_read = read(fd_read, &nc, 1);
                    if(size_read == 0)
                        break;
                    if(is_partof_word(nc)){
                        buf.data[buf.data_size] = nc;
                        buf.data_size++;
                    }else{
                        break;
                    }
                }
                write(work_pipe[1], &buf, sizeof(buf)); 
            }
        }
        printf("read process %lu read over : %s\n", pid, file_list.file[seq]);
        close(fd_read);
        seq += READ_PROC_NUM;
    }
    printf("read process %lu process over\n", pid);
    close(work_pipe[1]);
}

void count_words(map<string, int>& proc_result, data_buff & buf)
{
    //count words
    int j;
    int flag;
    char word[50];
    int i = 0;
    map<string, int>::iterator iter;
    while(i < buf.data_size)
    {
        //read a word
        j = 0;
        flag = 0;
        memset(word, 0, sizeof(word));
        while(1)
        {   
            if(is_partof_word(buf.data[i]) && i < buf.data_size){
                flag = 1;
                word[j] = buf.data[i];
                j++;
                i++;
            }else{
                i++;
                break;
            }
        }

        if(flag == 1)
        {
            //save word in map
            string sword = word;
            //printf("%s\n", word);
            iter = proc_result.find(sword);
            //if(iter == proc_result.end()){
                //proc_result[sword] = 1;
            //}else{
            //default initiate to 0
                proc_result[sword]++;
            //}
        }
    }    
}

//count process funtion
//read data form work pipe and count words
void count_proc()
{
    close(work_pipe[1]);
    close(result_pipe[0]);
    int n;
    unsigned long pid = (unsigned long)getpid();
    data_buff buf;
    while(1)
    {
        map<string, int> proc_result;
        map<string, int>::iterator iter;
        n = read(work_pipe[0], (void*)&buf, sizeof(buf));

        if(n == -1){
            printf("work pipe read error\n");
            break;
        }else if(n == 0){
            printf("work pipe read over\n");
            break;
        }else{
            count_words(proc_result, buf);
        }

        printf("count process %lu count %d words\n", pid, (int)proc_result.size());
        //write result in pipe
        for(iter = proc_result.begin(); iter != proc_result.end(); iter++)
        {
            count_result result;
            result.times = (*iter).second;
            strcpy(result.data, (*iter).first.data());
            write(result_pipe[1], &result, sizeof(result));
        }
    }
    close(work_pipe[0]);
    close(result_pipe[1]);
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

//test
//output file list
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
    map<string, int>::iterator iter;

    if(argc != 3)
    {
        printf("parameter error");
        return -1;
    }

    read_file_list(args[1]);
    //print_file_list();

    
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
            exit(0);//not excute follow code
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
            exit(0);
        }
    }


    close(work_pipe[0]);
    close(work_pipe[1]);
    close(result_pipe[1]);

    //read child process count result from result_pipe
    //save final result to map
    count_result word;
    int n;
    while(1)
    {
        n = read(result_pipe[0], &word, sizeof(word));
        if(n == -1){
            break;
        }else if(n == 0){
            close(result_pipe[0]);
            break;
        }else{
            //save word in map
            string sword = word.data;
            //printf("%s\n", word);
            //iter = result.find(sword);
            //if(iter == result.end()){
                //result[sword] = word.times;
            //}else{
            //default initiate to 0
                result[sword] += word.times;
            //}
        }
    }


    //write final result in file
    char * result_filename = args[2];
    int fd_result = open(result_filename,  O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
    if(fd_result == -1)
    {
        printf("open file write error: %s", strerror(errno));
        return -1;
    }
    char buf[100];
    for(iter = result.begin(); iter != result.end(); iter++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s : %d\n", (*iter).first.data(), (*iter).second);
        write(fd_result, buf, strlen(buf));
    }
    close(fd_result);


    //wait read process exit
    for(i = 0; i < READ_PROC_NUM; i++)
    {
       if(waitpid(read_pid[i], NULL, 0) != read_pid[i]){
           printf("waitpid error\n");
       }else{
           printf("read process exit\n");   
       }
    }
    close(work_pipe[1]);

    //wait count process exit
    for(i = 0; i < COUNT_PROC_NUM; i++)
    {
        if(waitpid(count_pid[i], NULL, 0) != count_pid[i]){
            printf("waitpid error\n");
        }
    }
    close(work_pipe[0]);
    return 0;
}

