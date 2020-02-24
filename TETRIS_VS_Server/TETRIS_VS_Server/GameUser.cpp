#include "stdafx.h"
#include "GameUser.h"

#include "RoomManager.h"
#include "PacketManager.h"
#include "SystemFrame.h"
#include "LobbySystem.h"
#include "GameRoomSystem.h"
//#include "Pl"

GameUser::GameUser(SOCKET socket, SOCKADDR_IN cliaddr)
{
	this->m_socket = socket;
	this->m_cliaddr = cliaddr;

	m_state = USER_STATE::LOBBY;
	m_packetManager = new PacketManager();
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

void GameUser::Update()
{
	if (!bOn)
	{
		bOn = true;

		if (m_systemFrame != nullptr)
		{
			SafeDelete(m_systemFrame);
		}

		switch (m_state)
		{
		case USER_STATE::CLOSE_CONNECT:
			bSocketConnect = false;
			return;
			break;
		case  USER_STATE::LOBBY:
			m_systemFrame = new LobbySystem();
			break;
		case USER_STATE::IN_ROOM:
			m_systemFrame = new GameRoomSystem();
			break;
		case USER_STATE::PLAY_GAME:
			//m_systemFrame = new PlayGameSystem();
			break;
		}

		StartThread();
	}
}

void GameUser::StartThread()
{
	if (m_threadHandle != nullptr)
	{
		CloseHandle(m_threadHandle);
	}

	m_systemFrame->Update(this);
}

int GameUser::GetUserNum()
{
	return m_userNum;
}

void GameUser::SetUserNum(int userNum)
{
	m_userNum = userNum;
}

void GameUser::SetRoomNum(int roomNum)
{
	m_packetManager->m_lobbyPacket->n_roomNum = roomNum;
}

USER_STATE GameUser::GetUserState()
{
	return m_state;
}

bool GameUser::IsConnect()
{
	return bSocketConnect;
}