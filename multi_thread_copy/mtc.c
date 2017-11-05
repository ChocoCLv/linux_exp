#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

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
    char data[DATA_BLOCK_SIZE];
}data_buff;

void * thread_read(void * arg)
{
    int size_read;
    int block_read_seq = (int)arg;
    unsigned long tid = (unsigned long)pthread_self();
    data_buff *buf;
    
    while(1)
    {
        buf = (struct data_buff*)malloc(sizeof(data_buff));
        size_read = pread(fd_read, buf->data, DATA_BLOCK_SIZE, DATA_BLOCK_SIZE * block_read_seq);
        if(size_read == 0)
        {
            printf("read thread %lu : exit\n", tid);
            pthread_exit((void*)1);
            break;
        }
        else if(size_read == -1)
        {
            printf("read thread %lu : read error\n", tid);
            pthread_exit((void*)2);
            break;
        }
        else
        {
            buf->data_size = size_read;
            buf->offset = DATA_BLOCK_SIZE * block_read_seq;
            write(fd_pipe[1], (void*)&buf, sizeof(buf)); 
            block_read_seq += THREAD_READ_NUM;
        }
    }
}

void * thread_write()
{
    int write_times = 0;
    int n;
    unsigned long tid = pthread_self();
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
            pwrite(fd_write, buf->data, buf->data_size, buf->offset);
            write_times++;
            free(buf);
        }
    }
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
    filename_read = args[1];
    filename_write = args[2];

    //open file read
    fd_read = open(filename_read, O_RDONLY);
    if(fd_read == -1)
    {
        printf("open file read error: %s",strerror(errno));
        return -1;
    }
    //open file write
    fd_write = open(filename_write, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
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
        err = pthread_create(&tid_read[i], NULL, thread_read, (void*)i);
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
        err = pthread_join(tid_read[i], &tret);
        if(err != 0)
        {
            printf("can't create thread\n");
            return -1;
        }
    }
    close(fd_pipe[1]);
    close(fd_read);
    //wait write thread exit
    for(i = 0; i < THREAD_WRITE_NUM; i++)
    {
        err = pthread_join(tid_write[i], &tret);
        if(err != 0)
        {
            printf("can't create thread\n");
            return -1;
        }
    }
    close(fd_pipe[0]);
    close(fd_write);
    return 0;
}
