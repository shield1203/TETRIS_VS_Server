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

void LobbySystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;


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