#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


#define BUFFER 64/* String buffer size */


int main(int argc, char *argv[])
{
    if ((argc != 5))/* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s [IP] [String] [Target Port] [Host Port]\n", argv[0]);
        exit(1);
    }
    /* Construct variables */
    struct sockaddr_in target;/* socket specs of target */
    struct sockaddr_in dest;/* socket specs of respondant */
    struct sockaddr_in serv;/* socket specs of host */
    unsigned long long i = 0;/* number prefix for console printout */
    unsigned short destport;/* target port */
    unsigned short servport;/* host port */
    char msg[BUFFER];/* message string */
    char *IP;/* IP address of target */

    int dorway;/* socket */

    int b;/* message byte size */
    int msglen;/* length of initial string */
    unsigned int destsize = sizeof(dest);/* address length */

    IP = argv[1];
    strncpy(msg, argv[2], BUFFER);
    servport = atoi(argv[4]);
    destport = atoi(argv[3]);

    /* Client setup */
    memset((char *)&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(IP);
    target.sin_port   = htons(destport);

    /* Server setup */
    memset((char *)&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(servport);


    /* Check for common errors */
    if ((msglen = strlen(msg)) > BUFFER)
    {
        printf("String too long.\n");
        exit(2);
    }

    if ((dorway = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("Problem creating socket\n");
        exit(3);
    }

    if (bind(dorway, (struct sockaddr *) &serv, sizeof(serv)) < 0)
    {
        printf("Problem binding\n");
        exit(4);
    }

    if (sendto(dorway, msg, msglen, 0, (struct sockaddr *)
               &target, sizeof(target)) != msglen)
    {
        printf("Could not send string.\n");
        exit(5);
    }


    /* start listening */
    while (1)
    {
        memset(msg, 0, BUFFER);
        b = recvfrom(dorway, msg, BUFFER, 0, (struct sockaddr *) &dest, &destsize);
        printf("msg%llu: %s\n", i, msg);
        sendto(dorway, msg, b, 0, (struct sockaddr *) &dest, destsize);
        i++;
    }
    return 0;
}
