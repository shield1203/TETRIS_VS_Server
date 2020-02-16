#pragma once

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

	HANDLE m_threadHandle = nullptr;

	USER_STATE m_state;

	SystemFrame* m_systemFrame = nullptr;
public:
	friend LobbySystem;
public:
	void Update();
	void StartThread();

	GameUser(SOCKET, SOCKADDR_IN);
	~GameUser();
};