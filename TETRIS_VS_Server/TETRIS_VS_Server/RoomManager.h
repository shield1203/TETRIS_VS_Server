#pragma once
class GameUser;

enum ROOM_MANAGER { FULL_USER_COUNT = 2 };

struct GameRoom
{
	int roomNum = 0;
	list<GameUser*>gameUserList;
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

	void CreateGameRoom(GameUser*);
	bool EnterRoom(int, GameUser*);
	void ExitRoom(int, GameUser*);
	void CheckRoom(GameRoom*);

	~RoomManager();
};

