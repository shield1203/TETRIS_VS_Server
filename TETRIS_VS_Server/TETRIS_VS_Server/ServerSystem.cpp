#include "stdafx.h"
#include "ServerSystem.h"
#include "UserManager.h"

ServerSystem::ServerSystem()
{
	m_userManager = UserManager::getInstance();
}

ServerSystem::~ServerSystem()
{
	SafeDelete(m_userManager);
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

	m_acceptSocket = SetTCPServer();

	printf("[TETRIS_VS] 서버 시작\n\n");

	unsigned int threadID = 0;
	HANDLE threadAccept = (HANDLE)_beginthreadex(NULL, 0, AcceptUser, (void*)this, 0, (unsigned*)&threadID);

	while (true)
	{

	}

	WSACleanup();
}

unsigned int WINAPI ServerSystem::AcceptUser(void* param)
{
	ServerSystem* m_serverSystem = (ServerSystem*)param;

	while (true)
	{
		GameUser* gameUser = new GameUser;

		int len = sizeof(gameUser->cliaddr);

		gameUser->socket = accept((SOCKET)m_serverSystem->m_acceptSocket, (SOCKADDR*)&gameUser->cliaddr, &len);//연결 수락
		if (gameUser->socket == -1)
		{
			SafeDelete(gameUser);
			perror("Accept 실패\n");
			break;
		}

		printf("%s:%d의 연결 요청 수락\n", inet_ntoa(gameUser->cliaddr.sin_addr), ntohs(gameUser->cliaddr.sin_port));

		gameUser->state = LOBBY;
		m_serverSystem->m_userManager->m_userList.push_back(gameUser);
	}

	closesocket(m_serverSystem->m_acceptSocket);//소켓 닫기

	return 0;
}

