#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DATA_BLOCK_SIZE 4096
#define THREAD_READ_NUM 1
#define THREAD_WRITE_NUM 10

int fd_pipe[2];
int fd_write = -1;
int fd_read = -1;
char * filename_read = NULL;
char * filename_write = NULL;

typedef struct file_data_buff
{
    int offset;
    int data_size;
    char buf[DATA_BLOCK_SIZE];
}data_buff;

void * thread_read()
{
    data_buff *buf;
}

void * thread_write()
{

}

int main(int argc, char * args[])
{
    int i = 0;
    int err;
    void * tret;
    pthread_t tid_read[THREAD_READ_NUM];
    pthread_t tid_write[THREAD_WRITE_NUM];

    if(argc != 3)
    {
        printf("parameter error\n");
        return -1;
    }
    file_read = args[1];
    file_write = args[2];

    //open file read
    fd_read = open(file_read, O_RDONLY);
    if(fd_read == -1)
    {
        printf("open file read error: %s",strerror(errno));
        return -1;
    }
    //open file write
    fd_write = open(file_read, O_WDONLY | O_CREAT | O_TRUNC, S_IWUSR);
    if(fd_write == -1)
    {
        printf("open file write error: %s", strerror(errno));
        return -1;
    }

    //open pipe
    //0 for read
    //1 for write
    if(pipe(fd_pipe) < 0)
    {
        printf("pipe error\n");
        return -1;
    }

    //create file read thread
    for(i = 0; i < THREAD_READ_NUM; i++)
    {
        err = pthread_create(&tid_read[i], NULL, thread_read, NULL);
        if(err != 0)
        {
            printf("can't create thread\n");
            return -1;
        }
    }

    //create file write thread
    for(i = 0; i < THREAD_WRITE_NUM; i++)
    {
        err = pthread_create(&tid_write[i], NULL, thread_write, NULL);
        if(err != 0)
        {
            printf("can't create thread\n");
            return -1;
        }
    }
    //wait read thread exit
    for(i = 0; i < THREAD_READ_NUM; i++)
    {
        err = pthread_join(&tid_read[i], &tret);
        if(err != 0)
        {
            printf("can't create thread\n");
            return -1;
        }
    }

    //wait write thread exit
    for(i = 0; i < THREAD_WRITE_NUM; i++)
    {
        err = pthread_join(&tid_write[i], &tret);
        if(err != 0)
        {
            printf("can't create thread\n");
            return -1;
        }
    }

    return 0;
}
