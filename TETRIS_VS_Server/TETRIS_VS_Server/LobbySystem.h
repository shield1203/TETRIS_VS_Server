#pragma once
class SystemFrame;

enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM };

struct LobbyPacket
{
	bool b_enterRoom;
	USER_LOBBY userReq;
	int n_roomNum;
	int n_roomCount;
};

class LobbySystem : public SystemFrame
{
private:
	LobbyPacket *m_lobbyPacket = nullptr;
public:
	LobbySystem();
	~LobbySystem();

	virtual void Update(GameUser&);
	virtual void Recv();
	virtual bool CheckPacket();
	virtual void Send();

	static unsigned int WINAPI Communication(void*);
};