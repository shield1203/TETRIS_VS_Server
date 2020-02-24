#pragma once
struct GameRoom_Lobby
{
	int roomNum = 0;
	int userCount = 0;
};

// Lobby
enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM };

struct LobbyPacket
{
	bool b_enterRoom = true;
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	int n_roomNum = 0;
	int n_roomCount = 0;
};

struct LobbySendPackt
{
	LobbyPacket lobbyPacket;
	GameRoom_Lobby gameRoom_lobby[20];
};

// GameRoom
enum class USER_ROOM : unsigned int { ROOM_IDLE, ROOM_BACK_LOBBY, ROOM_GAME_START };

struct GameRoomPacket
{
	USER_ROOM userReq = USER_ROOM::ROOM_IDLE;
	int userNum = 0;
	bool bOn = false;
	bool bOwner = false;
	bool bReady = false;
	bool bGameStart = false;
};

// PlayGame
struct PlayGamePacket
{

};

class PacketManager
{
public:
	LobbyPacket* m_lobbyPacket = nullptr;
	LobbySendPackt* m_lobbySendPacket = nullptr;
	GameRoomPacket* m_gameRoomPacket = nullptr;
	PlayGamePacket* m_PlayGamePacket = nullptr;
public:
	void SetLobbyPacket(LobbyPacket*);
	void SetGameRoomPacket(GameRoomPacket*);

	PacketManager();
	~PacketManager();
};