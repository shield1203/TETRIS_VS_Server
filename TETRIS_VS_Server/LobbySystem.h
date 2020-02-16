#pragma once
class SystemFrame;

enum USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_RROM, LOBBY_ENTER_ROOM };

struct LobbyPacket
{
	USER_LOBBY lobby;
	int n_roomNum;
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
	virtual void Send();

	static unsigned int WINAPI Communication(void*);
};