#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


#define BUFFER 128/* String buffer size */


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
    unsigned short destport;/* target port */
    unsigned short servport;/* host port */
    char msg[BUFFER];/* bounceback message */
    char *IP;/* IP address of target */
    char *s1;/* string sent to target */

    int dorway;/* socket */

    int n;/* message byte size */
    int s1len;/* length of initial string */

    IP = argv[1];
    s1 = argv[2];
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

    unsigned int destsize = sizeof(dest);


    /* Check for common errors */
    if ((s1len = strlen(s1)) > BUFFER)
    {
        printf("Echo string too long.\n");
        exit(2);
    }

    if ((dorway = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        printf("Problem creating server socket\n");
        exit(3);
    }

    if (bind(dorway, (struct sockaddr *) &serv, sizeof(serv)) < 0)
    {
        printf("Problem binding\n");
        exit(4);
    }

    if (sendto(dorway, s1, s1len, 0, (struct sockaddr *)
               &target, sizeof(target)) != s1len)
    {
        printf("Could not send string.\n");
        exit(5);
    }


    /* start listening */
    int i = 0;
    while (1)
    {
        memset(msg, 0, BUFFER);
        n = recvfrom(dorway, msg, BUFFER, 0, (struct sockaddr *) &dest, &destsize);
        printf("msg%i: %s", i, msg);
        sendto(dorway, msg, n, 0, (struct sockaddr *) &dest, destsize);
        i++;
    }
    return 0;
}
