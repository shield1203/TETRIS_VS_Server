#pragma once

class ServerSystem
{
public:
	SOCKET SetTCPServer();
	
	void Process();
	void AcceptSocket();

	ServerSystem();
	~ServerSystem();
};