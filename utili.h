#ifndef UTILI_H
#define UTILI_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <sys/stat.h>
#include <fcntl.h>

#define PORT  8888
#define SIZE 40

//与服务器端要保持一致
struct Msg
{
    char name[SIZE];
    char passwd[SIZE];
    char operate[SIZE];
    char time[SIZE];
    char passwd2[SIZE];
    char result[SIZE];
	char buf[SIZE];
	int money;
	int tt;
};

void Menu_Seletc(int socketfd );
void Main_Menu();
void Manager_Menu();



#endif 
