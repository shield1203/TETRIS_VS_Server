#include "common.h"

#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256

typedef struct msgStruct
{
    int x;
    int y;
}MSGSTRUCT;

SOCKET SetTCPServer(short pnum, int blog);//대기 소켓 설정
void AcceptLoop(SOCKET sock);//Accept Loop
void DoIt(void* param); //송수신 스레드 진입점


int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);//윈속 초기화           
    SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE);//대기 소켓 설정

    if (sock == -1)
    {
        perror("대기 소켓 오류");
        WSACleanup();
        return 0;
    }

    printf("서버 시작\n");

    AcceptLoop(sock);//Accept Loop
    WSACleanup();//윈속 해제화

    return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
    SOCKET sock;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소켓 생성

    if (sock == -1) { return -1; }

    SOCKADDR_IN servaddr = { 0 };//소켓 주소
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = GetDefaultMyIP();
    servaddr.sin_port = htons(PORT_NUM);

    int re = 0;

    re = bind(sock, (struct sockaddr*) & servaddr, sizeof(servaddr));//소켓 주소와 네트워크 인터페이스 결합

    if (re == -1) { return -1; }

    re = listen(sock, blog);//백 로그 큐 설정

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
        dosock = accept(sock, (SOCKADDR*)&cliaddr, &len);//연결 수락
        if (dosock == -1)
        {
            perror("Accept 실패");
            break;
        }

        printf("%s:%d의 연결 요청 수락\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        _beginthread(DoIt, 0, (void*)dosock);
    }

    closesocket(sock);//소켓 닫기
}

void DoIt(void* param)
{
    SOCKET dosock = (SOCKET)param;
    SOCKADDR_IN cliaddr = { 0 };

    int len = sizeof(cliaddr);

    getpeername(dosock, (SOCKADDR*)&cliaddr, &len);//상대 소켓 주소 알아내기

    char msg[MAX_MSG_LEN] = "";

    MSGSTRUCT* s;

    while (recv(dosock, msg, sizeof(msg), 0) > 0)//수신
    {
        msg[sizeof(MSGSTRUCT)] = '\0';

        s = (MSGSTRUCT*)msg;
        printf("recv:x좌표 %d y좌표 %d, \n", s->x, s->y);

        send(dosock, (char*)s, sizeof(MSGSTRUCT), 0);//송신
    }

    printf("%s:%d와 통신 종료\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
    closesocket(dosock);//소켓 닫기
}