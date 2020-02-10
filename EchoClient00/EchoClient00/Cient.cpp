#include "common.h"

#define PORT_NUM 10200
#define MAX_MSG_LEN 256
#define SERVER_IP "203.232.193.170"

typedef struct msgStruct
{
	int x;
	int y;
}MSGSTRUCT;

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//���� �ʱ�ȭ           

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���� ����
	
	if (sock == -1) 
	{ 
		return -1; 
	}

	SOCKADDR_IN servaddr = { 0 };//���� �ּ�
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	re = connect(sock, (struct sockaddr*) & servaddr, sizeof(servaddr));//���� ��û

	if (re == -1) 
	{ 
		return -1; 
	}

	char msg[MAX_MSG_LEN] = "";
	int getsize = 0;
	MSGSTRUCT s;
	MSGSTRUCT* s1 = &s;

	while (true)
	{
		scanf("%d%d", &s1->x, &s1->y);
		printf("�۽�:%d %d\n", s1->x, s1->y);

		send(sock, (char*)s1, sizeof(MSGSTRUCT), 0);//�۽�

		if (strcmp(msg, "exit") == 0) 
		{ 
			break; 
		}

		getsize = recv(sock, msg, sizeof(msg), 0);//����

		msg[getsize] = '\0';

		s1 = (MSGSTRUCT*)msg;
		printf("����:%d %d\n", s1->x, s1->y);
	}

	closesocket(sock);//���� �ݱ�
	WSACleanup();//���� ����ȭ

	return 0;
}