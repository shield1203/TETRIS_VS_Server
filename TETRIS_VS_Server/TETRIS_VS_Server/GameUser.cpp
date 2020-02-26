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

	m_state = USER_STATE::LOBBY;
	m_packetManager = new PacketManager();

	m_systemFrame = new LobbySystem();
}

GameUser::~GameUser()
{
	closesocket(m_socket);

	if (m_systemFrame != nullptr)
	{
		SafeDelete(m_systemFrame);
	}

	if (m_threadHandle != nullptr)
	{
		CloseHandle(m_threadHandle);
	}
	
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

}

void GameUser::Send()
{

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