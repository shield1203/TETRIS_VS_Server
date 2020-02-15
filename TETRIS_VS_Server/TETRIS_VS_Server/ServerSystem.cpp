#include "stdafx.h"
#include "ServerSystem.h"

ServerSystem::ServerSystem()
{
}

ServerSystem::~ServerSystem()
{
}

SOCKET ServerSystem::SetTCPServer()
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (sock == -1)
	{
		printf("[Accept소켓] 생성 실패");
		exit(1);
	}

	SOCKADDR_IN servaddr = { 0 };//소켓 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = GetDefaultMyIP();
	servaddr.sin_port = htons(PORT_NUM);

	int m_check = 0;

	m_check = bind(sock, (struct sockaddr*) & servaddr, sizeof(servaddr));//소켓 주소와 네트워크 인터페이스 결합

    if (m_check == -1) 
	{ 
		printf("[Accept소켓] bind 실패");
		exit(1);
	}

	m_check = listen(sock, BLOG_SIZE); //백 로그 큐 설정

	if (m_check == -1)
	{
		printf("[Accept소켓] listen 실패");
		exit(1);
	}

	return sock;
}

void ServerSystem::Process()
{
	WSADATA m_wsadata;
	WSAStartup(MAKEWORD(2, 2), &m_wsadata);//윈속 초기화       

	SOCKET m_acceptSocket;
	m_acceptSocket = SetTCPServer();

	printf("[TETRIS_VS] 서버 시작");

	while (true)
	{

	}

	WSACleanup();
}

void ServerSystem::AcceptUser()
{

}