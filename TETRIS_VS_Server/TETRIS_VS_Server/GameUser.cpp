#include "stdafx.h"
#include "GameUser.h"

#include "RoomManager.h"
#include "PacketManager.h"
#include "SystemFrame.h"
#include "LobbySystem.h"
#include "GameRoomSystem.h"
//#include "Pl"

GameUser::GameUser(SOCKET socket, SOCKADDR_IN cliaddr, int userNum)
{
	this->m_socket = socket;
	this->m_cliaddr = cliaddr;

	m_userNum = userNum;

	m_state = USER_STATE::USER_LOBBY;
	m_packetManager = new PacketManager(userNum);

	m_systemFrame = new LobbySystem();
}

GameUser::~GameUser()
{
	closesocket(m_socket);

	SafeDelete(m_systemFrame);

	CloseHandle(m_threadHandle);
	
	SafeDelete(m_packetManager);
}

void GameUser::Initialize()
{
	if (m_packetManager->m_packetData->userState != m_state)
	{
		m_state = m_packetManager->m_packetData->userState;

		SafeDelete(m_systemFrame);

		switch (m_state)
		{
		case USER_STATE::USER_LOBBY:
			m_systemFrame = new LobbySystem();
			break;
		case USER_STATE::USER_GAME_ROOM:
			m_systemFrame = new GameRoomSystem();
			break;
		case USER_STATE::USER_PLAY_GAME:
			//m_systemFrame = new PlayGameSystem();
			break;
		}
	}
}

void GameUser::Recv()
{
	char msg[MAX_MSG_LEN] = "";
	int connectCheck = recv(m_socket, msg, sizeof(msg), 0);

	if (connectCheck > 0)
	{
		msg[sizeof(PacketData)] = '\0';
		PacketData* packetData = (PacketData*)msg;

		m_packetManager->CopyPacket(packetData);
		m_packetManager->GetData(m_packetManager->m_packetData->userState);
	}
	else
	{
		m_packetManager->m_packetData->userState = USER_STATE::CLOSE_CONNECT;
	}
}

void GameUser::Send()
{
	m_packetManager->SetPacket(m_state);
	send(m_socket, (char*)m_packetManager->m_packetData, sizeof(PacketData), 0);
}

int GameUser::GetUserNum()
{
	return m_userNum;
}

bool GameUser::IsConnect()
{
	bool bConnect = (m_state != USER_STATE::CLOSE_CONNECT);

	return bConnect;
}

unsigned int WINAPI GameUser::Communication(void* gameUser)
{
	GameUser* pGameUser = (GameUser*)gameUser;

	while (pGameUser->m_packetManager->m_packetData->userState != USER_STATE::CLOSE_CONNECT)
	{
		pGameUser->Recv();

		if (pGameUser->m_packetManager->m_packetData->userState != USER_STATE::CLOSE_CONNECT)
		{
			pGameUser->Initialize();

			pGameUser->m_systemFrame->CheckPacket(pGameUser->m_packetManager);

			pGameUser->Send();
		}
	}

	pGameUser->m_mutex.lock();
	switch (pGameUser->m_state)
	{
	case USER_STATE::USER_GAME_ROOM:
	case USER_STATE::USER_PLAY_GAME:
		RoomManager::getInstance()->ExitRoom(pGameUser->m_packetManager->m_userNum);
		break;
	}

	printf("%s:[%d]와 통신 종료\n", inet_ntoa(pGameUser->m_cliaddr.sin_addr), ntohs(pGameUser->m_cliaddr.sin_port));
	pGameUser->m_state = USER_STATE::CLOSE_CONNECT;
	pGameUser->m_mutex.unlock();

	return 0;
}