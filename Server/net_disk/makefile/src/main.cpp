/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun 16 Dec 2018 07:32:49 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
#include "tcpnet.h"
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        setsid();
        chdir("/");
        umask(0777);
        TcpNet net;
        net.open(7799);
    }
    return 0;
}
