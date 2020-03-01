#pragma once
class SystemFrame;
class PacketManager;
class LobbySystem;
class GameRoomSystem;
class VersusPlaySystem;
class GameResultSystem;

class GameUser
{
private:
	SOCKET m_socket;
	SOCKADDR_IN m_cliaddr = { 0 };

	USER_STATE m_state;
	int m_userNum = 0;
	
	SystemFrame* m_systemFrame = nullptr;
public:
	mutex m_mutex;
	HANDLE m_threadHandle = nullptr;
	PacketManager* m_packetManager = nullptr;
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