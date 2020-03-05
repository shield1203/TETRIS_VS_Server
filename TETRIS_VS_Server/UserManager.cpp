#include "stdafx.h"
#include "UserManager.h"

UserManager* UserManager::Inst = nullptr;

UserManager* UserManager::getInstance()
{
	if (Inst == nullptr) {
		Inst = new UserManager();
	}

	return Inst;
}

UserManager::UserManager()
{

}

UserManager::~UserManager()
{
	for (auto i : m_userList)
	{
		closesocket(i->socket);
		
		if (i->thread != nullptr)
		{
			CloseHandle(i->thread);
		}

		SafeDelete(i);
	}

	m_userList.clear();
}