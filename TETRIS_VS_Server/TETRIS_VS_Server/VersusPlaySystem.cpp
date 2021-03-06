#include "stdafx.h"
#include "SystemFrame.h"
#include "VersusPlaySystem.h"

#include "GameUser.h"
#include "RoomManager.h"
#include "PacketManager.h"

VersusPlaySystem::VersusPlaySystem()
{
}

VersusPlaySystem::~VersusPlaySystem()
{
}

void VersusPlaySystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	CheckGameOver(pPacketManager);
}

void VersusPlaySystem::CheckGameOver(PacketManager* packetManager)
{
	m_mutex.lock();
	
	int count = 0;
	for (auto block : packetManager->m_blockList)
	{
		if (count > 3 && block->yPos == 2)
		{
			RoomManager::getInstance()->SetGameResult(packetManager->m_userNum);
			break;
		}
		count++;
	}

	m_mutex.unlock();
}