#include <sys/stat.h> /*Needed for open()*/
#include <fcntl.h>    /*Needed for open()*/
#include <unistd.h>   /*Needed for write()*/
#include <string.h>   /*Needed for strcpy()*/
#include <syslog.h>   /*Needed for system log function*/
#include <stdio.h>    /*Needed for printf()*/
#include <stdlib.h>   /*Needed for dynamic memory allocation*/

/* Definning buffer size*/
#ifndef BUFF_SIZE
#define BUFF_SIZE 1024U
#endif

int main(int argc, char *argv[])
{
    /* file descriptor, Open Flags, File permission*/
    int fd, openFlags, filePerms;
    /* Number of bytes written*/
    int numberOfBytes = 0;
    /* Openning log*/
    openlog(NULL, 0, LOG_USER);

    /* Check if User entered Commands number correctly*/
    if (argc != 3)
    {
        syslog(LOG_ERR, "Invalid Number of arguments :%d", argc);
        exit(1);
    }

    /* Intialze counter used for calculating total number of bytes in the second argument of main*/
    int buffer_size = 0;
    /* Looping over the second argument*/
    while (argv[2][buffer_size] != '\0')
        buffer_size++;
    /* Dynamically allocating memory in the heap for writting buffer*/
    char *buff = (char *)malloc(buffer_size * sizeof(char));
    /* Coppying data into writting buffer*/
    strcpy(buff, argv[2]);

    /* Open the file with the following flags
     * O_RDWR --> open the file for read and write
     * O_CREAT --> Create the file if doesn't exist
     * O_TRUNC --> Truncate existing file to zero length
     *
     * And with the following modes (permissions)
     * S_IRUSR user has read permission
     * S_IWUSR user has write permission
     * S_IXUSR user has excecute permission
     * S_IRGRP Group has read permission
     * S_IWGRP Group has write permission
     * S_IROTH Others has read permission
     * S_IWOTH Others has write permission
     */

    /* Open flags*/
    openFlags = (O_RDWR | O_CREAT | O_TRUNC);
    /* File permissions*/
    filePerms = (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    /* Openning the file and returning the file descripor*/
    fd = open(argv[1], openFlags, filePerms);
    /* Check the status of openning the file*/
    if (fd == -1) /* Openning file returned with an Error*/
    {
        syslog(LOG_ERR, "Closing file returned with an Error\n");
        exit(1);
    }
    /* Write the data into the file*/
    numberOfBytes = write(fd, buff, buffer_size);
    /* Informing syslog about writing to the file */
    syslog(LOG_DEBUG, "Writing %s to %s\n", argv[2], argv[1]);

    /* Closing the file*/
    if (close(fd) == -1) /* Closing the file returned with an error*/
    {
        syslog(LOG_ERR, "Closing file returned with an Error\n");
        exit(1);
    }

    /* Close opened log*/
    closelog();
    return 0; /* Programm run succefully*/
}
