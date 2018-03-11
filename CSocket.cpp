//
// Created by lianyi on 18-2-28.
//

#include "CSocket.h"
#include<arpa/inet.h>
int CSocket::open_clientfd(const char *ip, int port)
{
    int clientfd;
//        struct hostent *hp;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1; /* check errno for cause of error */

    /* Fill in the server's IP address and port */

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
//        bcopy((char *)hp->h_addr_list[0],
//              (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    inet_pton(AF_INET,ip,&serveraddr.sin_addr);
    /* Establish a connection with the server */
    if (connect(clientfd,(struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0)
        return -1;
    return clientfd;


}
int CSocket::Open_clientfd(const char *ip, int port)
{
    int rc;

    if ((rc = open_clientfd(ip, port)) < 0) {
        if (rc == -1)
            Log::unix_error("Open_clientfd Unix error");
    }
    return rc;
}