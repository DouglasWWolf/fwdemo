#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cthread.h"
#include "config_file.h"
#include "netsock.h"


using namespace std;

void test_config();

NetSock sock, sock2;

int main()
{
    if (!sock.create_server(12345))
    {
        printf(">>>>> CREATE FAILED <<<<<<\n");
        exit(1);
    }

    if (!sock2.create_server(12345))
    {
        printf(">>>>> CREATE FAILED on sock2 <<<<<<\n");
        exit(1);
    }

    sock.listen_and_accept();
    string pa = sock.get_peer_address();
    printf("The client IP is %s\n", pa.c_str());


    test_config();
    return 0;
}

