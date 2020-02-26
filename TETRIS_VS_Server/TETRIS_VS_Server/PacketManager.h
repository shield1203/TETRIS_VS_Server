#pragma once
typedef struct PacketData
{
	USER_STATE userState;
	unsigned short size = 0;
	char data[256] = "";
};

// Lobby
enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM };

struct LobbyPacket
{
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	bool bEnterRoom = true;
	int nRoomNum = 0;
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
	PacketData* m_packetData = nullptr;
	LobbyPacket* m_lobbyPacket = nullptr;
	GameRoomPacket* m_gameRoomPacket = nullptr;
	PlayGamePacket* m_PlayGamePacket = nullptr;
public:
	void SetLobbyPacket(LobbyPacket*);
	void SetGameRoomPacket(GameRoomPacket*);

	PacketManager();
	~PacketManager();
};