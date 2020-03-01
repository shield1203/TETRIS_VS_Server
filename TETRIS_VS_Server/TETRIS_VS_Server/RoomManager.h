#pragma once
class GameUser;
class PacketManager;

enum ROOM_MANAGER { FULL_USER_COUNT = 2 };

struct GameRoom
{
	int roomNum = 0;
	list<PacketManager*>gameUserList;
};

class RoomManager
{
private:
	static RoomManager* Inst;
	RoomManager();
public:
	list<GameRoom*>m_roomList;
public:
	static RoomManager* getInstance();

	void CreateGameRoom(PacketManager*);
	bool EnterRoom(int, PacketManager*);
	void ExitRoom(int);
	void CheckRoom(GameRoom*);
	void GameStartRoom(int);
	void SetGameResult(int);

	~RoomManager();
};

