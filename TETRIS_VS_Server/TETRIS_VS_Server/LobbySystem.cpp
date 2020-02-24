#include "stdafx.h"
#include "SystemFrame.h"
#include "LobbySystem.h"

#include "GameUser.h"
#include "RoomManager.h"
#include "PacketManager.h"

LobbySystem::LobbySystem()
{
}

LobbySystem::~LobbySystem()
{
}

void LobbySystem::Update(GameUser* gameUser)
{
	m_gameUser = gameUser;
	m_roomManager = RoomManager::getInstance();

	unsigned int threadID = 0;
	m_gameUser->m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, Communication, (void*)this, 0, (unsigned*)&threadID);
}

void LobbySystem::Recv()
{
	// 방 입장, 연결끊김, 방 만들기
	char msg[MAX_MSG_LEN] = "";
	int check = recv(m_gameUser->m_socket, msg, sizeof(msg), 0);

	if (check > 0)
	{
		msg[sizeof(LobbyPacket)] = '\0';
		LobbyPacket* lobbyPacket = (LobbyPacket*)msg;
		m_gameUser->m_packetManager->SetLobbyPacket(lobbyPacket);
	}
	else
	{
		m_gameUser->m_state = USER_STATE::CLOSE_CONNECT;
	}
}

bool LobbySystem::CheckPacket()
{
	if (m_gameUser->m_state == USER_STATE::CLOSE_CONNECT)
	{
		printf("%s:%d와 통신 종료\n", inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port));
		return false;
	}

	switch (m_gameUser->m_packetManager->m_lobbyPacket->userReq)
	{
	case USER_LOBBY::LOBBY_IDLE:
		break;
	case USER_LOBBY::LOBBY_CREATE_ROOM:
		m_gameUser->m_state = USER_STATE::IN_ROOM;

		m_roomManager->CreateGameRoom(m_gameUser);
		m_gameUser->m_packetManager->m_lobbyPacket->b_enterRoom = true;

		printf("%s:%d 가 게임룸 %d 생성\n", 
			inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum);
		break;
	case USER_LOBBY::LOBBY_ENTER_ROOM:
		if (m_roomManager->EnterRoom(m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum, m_gameUser))
		{
			m_gameUser->m_state = USER_STATE::IN_ROOM;

			m_gameUser->m_packetManager->m_lobbyPacket->b_enterRoom = true;

			printf("%s:%d 가 %d게임룸 입장\n", 
				inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum);
		}
		else
		{
			m_gameUser->m_packetManager->m_lobbyPacket->b_enterRoom = false;

			printf("%s:%d 가 %d게임룸 입장실패\n", 
				inet_ntoa(m_gameUser->m_cliaddr.sin_addr), ntohs(m_gameUser->m_cliaddr.sin_port), m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum);
		}
		break;
	}

	return true;
}

void LobbySystem::Send()
{
	SetPacket();
	send(m_gameUser->m_socket, (char*)m_gameUser->m_packetManager->m_lobbySendPacket, sizeof(LobbySendPackt), 0);
	// 방 수, 방 진입여부
	//m_gameUser->m_packetManager->m_lobbyPacket->n_roomCount = m_roomManager->m_roomList.size();
	//send(m_gameUser->m_socket, (char*)m_gameUser->m_packetManager->m_lobbyPacket, sizeof(LobbyPacket), 0);
	//
	//// 방 리스트
	//for (auto i : m_roomManager->m_roomList)
	//{
	//	GameRoom_Lobby* gameRoom_Lobby = new GameRoom_Lobby(i->roomNum, i->gameUserList.size());

	//	send(m_gameUser->m_socket, (char*)gameRoom_Lobby, sizeof(GameRoom_Lobby), 0);

	//	SafeDelete(gameRoom_Lobby);
	//}
	int k = 0; // ....????????
}

void LobbySystem::SetPacket()
{
	m_gameUser->m_packetManager->m_lobbySendPacket->lobbyPacket.b_enterRoom = m_gameUser->m_packetManager->m_lobbyPacket->b_enterRoom;
	m_gameUser->m_packetManager->m_lobbySendPacket->lobbyPacket.n_roomCount = m_roomManager->m_roomList.size();
	m_gameUser->m_packetManager->m_lobbySendPacket->lobbyPacket.n_roomNum = m_gameUser->m_packetManager->m_lobbyPacket->n_roomNum;
	m_gameUser->m_packetManager->m_lobbySendPacket->lobbyPacket.userReq = m_gameUser->m_packetManager->m_lobbyPacket->userReq;

	int num = 0;
	for (auto i : m_roomManager->m_roomList)
	{
		m_gameUser->m_packetManager->m_lobbySendPacket->gameRoom_lobby[num].roomNum = i->roomNum;
		m_gameUser->m_packetManager->m_lobbySendPacket->gameRoom_lobby[num].userCount = i->gameUserList.size();

		num++;
	}
}

unsigned int WINAPI LobbySystem::Communication(void* lobbySystem)
{
	LobbySystem* m_lobbySystem = (LobbySystem*)lobbySystem;

	while (m_lobbySystem->m_gameUser->m_state == USER_STATE::LOBBY)
	{
		m_lobbySystem->Recv();

		if (!m_lobbySystem->CheckPacket())
		{
			break;
		}

		m_lobbySystem->Send();
	}

	m_lobbySystem->m_gameUser->m_packetManager->m_lobbyPacket->userReq = USER_LOBBY::LOBBY_IDLE;

	m_lobbySystem->m_gameUser->bOn = false;

	return 0;
}