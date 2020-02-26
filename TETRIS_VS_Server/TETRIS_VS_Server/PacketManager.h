#pragma once
class RoomManager;

typedef struct PacketData
{
	USER_STATE userState;
	unsigned short size = 0;
	char data[256] = "";
};

// Lobby
enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM };

struct LobbyData
{
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	bool bEnterRoom = true;
	int nRoomNum = 0;
};

// GameRoom
enum class USER_ROOM : unsigned int { ROOM_IDLE, ROOM_BACK_LOBBY, ROOM_GAME_START };

struct GameRoomData
{
	USER_ROOM userReq = USER_ROOM::ROOM_IDLE;
	int userNum = 0;
	int roomNum = 0;
	bool bOn = false;
	bool bOwner = false;
	bool bReady = false;
	bool bGameStart = false;
};

// PlayGame
struct PlayGameData
{

};

class PacketManager
{
public:
	RoomManager* m_roomManager = nullptr;

	PacketData* m_packetData = nullptr;
	LobbyData* m_lobbyData = nullptr;
	GameRoomData* m_gameRoomData = nullptr;
	PlayGameData* m_playGameData = nullptr;
public:
	// Recv
	void CopyPacket(PacketData*);
	void GetData(USER_STATE);
	void GetLobbyData();
	void GetGameRoomData();
	void GetPlayGameData();

	// Send
	void SetPacket(USER_STATE);
	void SetLobbyData();
	void SetGameRoomData();
	void SetPlayGameData();

	PacketManager();
	~PacketManager();
};