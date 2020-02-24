#include "stdafx.h"
#include "ServerSystem.h"

#include "GameUser.h"
#include "RoomManager.h"
#include "SystemFrame.h"

ServerSystem::ServerSystem()
{
}

ServerSystem::~ServerSystem()
{
	for (auto i : m_userList)
	{
		SafeDelete(i);
	}
	m_userList.clear();

	RoomManager* m_roomManager = RoomManager::getInstance();
	SafeDelete(m_roomManager);
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
		for (auto i = m_userList.begin(); i != m_userList.end();)
		{
			(*i)->Update();

			if ((*i)->IsConnect())
			{
				i++;
			}
			else
			{
				SafeDelete(*i);
				i = m_userList.erase(i++);
			}
		}
	}

	WSACleanup();
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

unsigned int WINAPI ServerSystem::AcceptUser(void* param)
{
	ServerSystem* m_serverSystem = (ServerSystem*)param;

	while (true)
	{
		SOCKET userSoket;
		SOCKADDR_IN cliaddr;

		int len = sizeof(cliaddr);

		userSoket = accept((SOCKET)m_serverSystem->m_acceptSocket, (SOCKADDR*)&cliaddr, &len);//연결 수락
		if (userSoket == -1)
		{
			closesocket(userSoket);
			perror("Accept 실패\n");
			break;
		}

		printf("%s:%d의 연결 요청 수락\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

		GameUser* pAddUser = new GameUser(userSoket, cliaddr);

		int n_userNum = 0;
		while (true)
		{
			bool b_num = true;
			for (auto i : m_serverSystem->m_userList)
			{
				if (i->GetUserNum() == n_userNum)
				{
					b_num = false;
					break;
				}
			}

			if (b_num)
			{
				break;
			}

			n_userNum++;
		}

		pAddUser->SetUserNum(n_userNum);
		m_serverSystem->m_userList.push_back(pAddUser);
	}

	closesocket(m_serverSystem->m_acceptSocket);//소켓 닫기

	return 0;
}