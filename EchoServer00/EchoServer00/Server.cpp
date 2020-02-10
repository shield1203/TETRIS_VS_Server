#include "common.h"

#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256

typedef struct msgStruct
{
    int x;
    int y;
}MSGSTRUCT;

SOCKET SetTCPServer(short pnum, int blog);//��� ���� ����
void AcceptLoop(SOCKET sock);//Accept Loop
void DoIt(void* param); //�ۼ��� ������ ������


int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);//���� �ʱ�ȭ           
    SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE);//��� ���� ����

    if (sock == -1)
    {
        perror("��� ���� ����");
        WSACleanup();
        return 0;
    }

    printf("���� ����\n");

    AcceptLoop(sock);//Accept Loop
    WSACleanup();//���� ����ȭ

    return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���� ����

    if (sock == -1) { return -1; }

    SOCKADDR_IN servaddr = { 0 };//���� �ּ�
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = GetDefaultMyIP();
    servaddr.sin_port = htons(PORT_NUM);

    int re = 0;

    re = bind(sock, (struct sockaddr*) & servaddr, sizeof(servaddr));//���� �ּҿ� ��Ʈ��ũ �������̽� ����

    if (re == -1) { return -1; }

    re = listen(sock, blog);//�� �α� ť ����

    if (re == -1) { return -1; }

    return sock;
}

void AcceptLoop(SOCKET sock)
{
    SOCKET dosock;
    SOCKADDR_IN cliaddr = { 0 };

    int len = sizeof(cliaddr);

    while (true)
    {
        dosock = accept(sock, (SOCKADDR*)&cliaddr, &len);//���� ����
        if (dosock == -1)
        {
            perror("Accept ����");
            break;
        }

        printf("%s:%d�� ���� ��û ����\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        _beginthread(DoIt, 0, (void*)dosock);
    }

    closesocket(sock);//���� �ݱ�
}

void DoIt(void* param)
{
    SOCKET dosock = (SOCKET)param;
    SOCKADDR_IN cliaddr = { 0 };

    int len = sizeof(cliaddr);

    getpeername(dosock, (SOCKADDR*)&cliaddr, &len);//��� ���� �ּ� �˾Ƴ���

    char msg[MAX_MSG_LEN] = "";

    MSGSTRUCT* s;

    while (recv(dosock, msg, sizeof(msg), 0) > 0)//����
    {
        msg[sizeof(MSGSTRUCT)] = '\0';

        s = (MSGSTRUCT*)msg;
        printf("recv:x��ǥ %d y��ǥ %d, \n", s->x, s->y);

        send(dosock, (char*)s, sizeof(MSGSTRUCT), 0);//�۽�
    }

    printf("%s:%d�� ��� ����\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
    closesocket(dosock);//���� �ݱ�
}