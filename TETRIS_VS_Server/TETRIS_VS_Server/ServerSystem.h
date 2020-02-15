#pragma once

class UserManager;

class ServerSystem
{
private:
	SOCKET m_acceptSocket;

	UserManager* m_userManager = nullptr;
public:
	void Process();
	// 

	SOCKET SetTCPServer();
	static unsigned int WINAPI AcceptUser(void*);

	ServerSystem();
	~ServerSystem();
};