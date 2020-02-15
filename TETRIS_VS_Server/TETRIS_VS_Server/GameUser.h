#pragma once

enum USER_STATE { LOBBY, IN_ROOM, PLAY_GAME };

class GameUser
{
private:
	SOCKET m_socket;
	SOCKADDR_IN m_userAddr = { 0 };

	USER_STATE m_state;
	bool bReady = false;
public:
	void Init(SOCKET);

	GameUser();
	~GameUser();
};

