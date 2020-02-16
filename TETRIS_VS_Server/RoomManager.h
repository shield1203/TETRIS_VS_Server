#pragma once

struct GameRoom
{
	int m_roomNum;
	int m_userCount = 0;
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

	void CreateGameRoom(GameRoom*);
	bool EnterRoom(int, GameRoom*);

	~RoomManager();
};

