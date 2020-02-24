#include "stdafx.h"
#include "PacketManager.h"
#include "RoomManager.h"

PacketManager::PacketManager()
{
	m_lobbyPacket = new LobbyPacket;
	m_lobbySendPacket = new LobbySendPackt;
	m_gameRoomPacket = new GameRoomPacket;
	m_PlayGamePacket = new PlayGamePacket;
}

PacketManager::~PacketManager()
{
	SafeDelete(m_lobbyPacket);
	SafeDelete(m_gameRoomPacket);
	SafeDelete(m_PlayGamePacket);
}

void PacketManager::SetLobbyPacket(LobbyPacket* lobbyPacket)
{
	m_lobbyPacket->b_enterRoom = lobbyPacket->b_enterRoom;
	m_lobbyPacket->n_roomCount = lobbyPacket->n_roomCount;
	m_lobbyPacket->n_roomNum = lobbyPacket->n_roomNum;
	m_lobbyPacket->userReq = lobbyPacket->userReq;
}

void PacketManager::SetGameRoomPacket(GameRoomPacket* gameRoomPacket)
{
	m_gameRoomPacket->bGameStart = gameRoomPacket->bGameStart;
	m_gameRoomPacket->bOn = gameRoomPacket->bOn;
	m_gameRoomPacket->bOwner = gameRoomPacket->bOwner;
	m_gameRoomPacket->bReady = gameRoomPacket->bReady;
	m_gameRoomPacket->userNum = gameRoomPacket->userNum;
	m_gameRoomPacket->userReq = gameRoomPacket->userReq;
}