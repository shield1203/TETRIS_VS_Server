#pragma once

class SystemFrame
{
protected:
	GameUser* m_gameUser = nullptr;
public:
	virtual void Update(GameUser&) { };
	virtual void Recv(SOCKET) { };
	virtual void Send(SOCKET) { };
};