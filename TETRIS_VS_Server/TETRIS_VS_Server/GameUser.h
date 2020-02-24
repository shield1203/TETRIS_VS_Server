#pragma once

class SystemFrame;
class PacketManager;
class LobbySystem;
class GameRoomSystem;
//class PlayGameSystem;

class GameUser
{
private:
	bool bOn = false;
	bool bSocketConnect = true;

	SOCKET m_socket;
	SOCKADDR_IN m_cliaddr = { 0 };

	int m_userNum = 0;
	PacketManager* m_packetManager = nullptr;

	HANDLE m_threadHandle = nullptr;

	USER_STATE m_state;

	SystemFrame* m_systemFrame = nullptr;
public:
	friend LobbySystem;
	friend GameRoomSystem;
	//
public:
	void Update();
	void StartThread();

	int GetUserNum();
	void SetUserNum(int);
	void SetRoomNum(int);
	USER_STATE GetUserState();
	bool IsConnect();

	GameUser(SOCKET, SOCKADDR_IN);
	~GameUser();
};