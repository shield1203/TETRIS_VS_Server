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
		printf("[Accept����] ���� ����");
		exit(1);
	}

	SOCKADDR_IN servaddr = { 0 };//���� �ּ�
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = GetDefaultMyIP();
	servaddr.sin_port = htons(PORT_NUM);

	int m_check = 0;

	m_check = bind(sock, (struct sockaddr*) & servaddr, sizeof(servaddr));//���� �ּҿ� ��Ʈ��ũ �������̽� ����

    if (m_check == -1) 
	{ 
		printf("[Accept����] bind ����");
		exit(1);
	}

	m_check = listen(sock, BLOG_SIZE); //�� �α� ť ����

	if (m_check == -1)
	{
		printf("[Accept����] listen ����");
		exit(1);
	}

	return sock;
}

void ServerSystem::Process()
{
	WSADATA m_wsadata;
	WSAStartup(MAKEWORD(2, 2), &m_wsadata);//���� �ʱ�ȭ       

	SOCKET m_acceptSocket;
	m_acceptSocket = SetTCPServer();

	printf("[TETRIS_VS] ���� ����");

	while (true)
	{

	}

	WSACleanup();
}

void ServerSystem::AcceptUser()
{

}