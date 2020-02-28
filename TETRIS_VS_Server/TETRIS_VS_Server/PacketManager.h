#pragma once
class RoomManager;

struct PacketData
{
	USER_STATE userState = USER_STATE::USER_LOBBY;
	unsigned short size = 0;
	char data[256] = "";
};

// Lobby
enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM, LOBBY_BACK_MENU };

struct LobbyData
{
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	bool bEnterRoom = true;
	int roomNum = 0;
};

struct LobbyData_GameRoom
{
	int roomNum;
	int userCount = 0;
};

// GameRoom
enum class USER_ROOM : unsigned int { ROOM_IDLE, ROOM_BACK_LOBBY, ROOM_GAME_START, ROOM_BACK_MENU };

struct GameRoomData
{
	USER_ROOM userReq = USER_ROOM::ROOM_IDLE;
	int roomNum = 0;
	bool bOn = false;
	bool bOwner = false;
	bool bReady = false;
};

// PlayGame
struct PlayGameData
{

};

class PacketManager
{
public:
	int m_userNum = 0;

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

	PacketManager(int);
	~PacketManager();
};