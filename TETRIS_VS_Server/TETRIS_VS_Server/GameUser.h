#pragma once

class SystemFrame;
class PacketManager;
class LobbySystem;
class GameRoomSystem;
//class PlayGameSystem;

class GameUser
{
private:
	SOCKET m_socket;
	SOCKADDR_IN m_cliaddr = { 0 };

	USER_STATE m_state;
	int m_userNum = 0;

	PacketManager* m_packetManager = nullptr;
	
	SystemFrame* m_systemFrame = nullptr;

	HANDLE m_threadHandle = nullptr;
public:
	void Initialize();
	void Recv();
	void Send();
	static unsigned int WINAPI Communication(void*);

	int GetUserNum();
	bool IsConnect();

	GameUser(SOCKET, SOCKADDR_IN, int);
	~GameUser();
};