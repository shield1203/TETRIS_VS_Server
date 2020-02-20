#pragma once
class SystemFrame;
class GameUser;

enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM };

struct LobbyPacket
{
	bool b_enterRoom = true;
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	int n_roomNum = 0;
	int n_roomCount = 0;
};

class LobbySystem : public SystemFrame
{
private:
	LobbyPacket *m_lobbyPacket = nullptr;
public:
	LobbySystem();
	~LobbySystem();

	virtual void Update(GameUser*);
	virtual void Recv();
	virtual bool CheckPacket();
	virtual void Send();

	static unsigned int WINAPI Communication(void*);
};