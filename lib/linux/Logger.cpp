#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include "../../inc/linux/Logger.h"

Logger Log;

void Logger::add_time()
{
    struct timeval time_val;
    struct tm * time_info;
    gettimeofday (&time_val, NULL);
    time_info = localtime (&time_val.tv_sec);
    this->to_devices("%02d-%02d %02d:%02d:%02d.%06ld ",time_info->tm_mon + 1, time_info->tm_mday,
                     time_info->tm_hour, time_info->tm_min, time_info->tm_sec, time_val.tv_usec);
}

void Logger::toFile(const char* name)
{
    int file_fd = open(name,   O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    FILE* filp = NULL;
    int index = this->num_log_device;

    if (index >= max_dev_num)
    {
        return;
    }

    if (file_fd > 2)
    {
        filp = fdopen(dup(file_fd), "w");

        if (filp == NULL)
        {
            close(file_fd);
            return;
        }
        setlinebuf(filp);
        this->log_devs[index] = (struct log_device *)malloc(sizeof(struct log_device));
        this->log_devs[index]->log_steam = filp;
        this->log_devs[index]->log_fd    = file_fd;
        this->log_devs[index]->log_type  = LOCAL_FILE;
        this->num_log_device++;
    }
    else
    {
        close(file_fd);
    }
}

int Logger::to_devices(const char *format, ...)
{
    va_list arg;
    int done, index;
    char default_log_name[256];

    if (this->num_log_device == 0)
    {
        memset(default_log_name, 0x0, 256);
        sprintf(default_log_name, "/dev/shm/%d.log", getpid());
        this->toFile(default_log_name);
        this->toFile("/dev/stdout");
    }

    for (index = 0; index < num_log_device; index++)
    {
        va_start (arg, format);

        if (log_devs[index]->log_steam)
            done = vfprintf(log_devs[index]->log_steam, format, arg);

        va_end (arg);
    }

    return done;
}

void Logger::Hexdump(const char* data, int len)
{
    int i = 0, value = 0;
    for (i = 0; i < len; i++)
    {
        value = *((unsigned char *) data + i);
        if ((i) % 2 == 0)
        {
            this->to_devices(" ");
        }
        if ((i) % 16 == 0)
        {
            this->to_devices("\n");
            this->to_devices("%p: ", data + i);
        }
        this->to_devices("%02x", value);
    }
    this->to_devices("\n");
}