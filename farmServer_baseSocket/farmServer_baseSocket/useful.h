#ifndef USEFUL_H
#define USEFUL_H

int errcont(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    return 1;
}

int errexit(const char *format, ...)
{
    va_list	args;

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}

int readn(int fd, void *buf, int num)
{
    int res;
    int n;
    char *ptr;
    n = num;
    ptr = (char *)buf;
    while(n > 0)
    {
        if((res = read(fd, ptr, n)) == -1)
        {
            if(errno == EINTR)
                res = 0;
            else
                return (-1);
        }
        else if(res == 0)
            return (-1);
        ptr += res;
        n -= res;
    }
    return 0;
}

int writen(int fd, void *buf, int num)
{
    int res;
    int n;
    char *ptr;
    n = num;
    ptr = (char *)buf;
    while(n > 0)
    {
        if((res = write(fd, ptr, n)) == -1)
        {
            if(errno == EINTR)
                res = 0;
            else
                return (-1);
        }
        else if(res == 0)
            return (-1);
        ptr += res;
        n -= res;
    }
    return 0;
}

#endif // USEFUL_H

