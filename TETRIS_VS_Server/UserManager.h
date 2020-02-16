#pragma once

struct GameUser
{
	bool bOn = false;

	SOCKET socket;
	SOCKADDR_IN cliaddr = { 0 };

	HANDLE thread = nullptr;

	USER_STATE state;
};

class UserManager
{
private:
	static UserManager* Inst;
	UserManager();
public:
	list<GameUser*> m_userList;
public:
	static UserManager* getInstance();

	~UserManager();
};

