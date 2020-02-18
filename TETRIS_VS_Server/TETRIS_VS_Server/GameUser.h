#pragma once

struct GameRoom;
class SystemFrame;
class LobbySystem;
//class RoomSystem
//class PlayGameSystem;

class GameUser
{
private:
	bool bOn = false;

	SOCKET m_socket;
	SOCKADDR_IN m_cliaddr = { 0 };

	GameRoom* m_gameRoom = nullptr;

	HANDLE m_threadHandle = nullptr;

	USER_STATE m_state;

	SystemFrame* m_systemFrame = nullptr;
public:
	friend LobbySystem;
	//
	//
public:
	void Update();
	void StartThread();

	USER_STATE GetUserState();

	GameUser(SOCKET, SOCKADDR_IN);
	~GameUser();
};