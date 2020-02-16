#include"stdafx.h"
#include"ServerSystem.h"

int main()
{
	system("title KJH's TETRIS_VS_MatchingServer");

	ServerSystem* pServerSystem = new ServerSystem();

	pServerSystem->Process();

	SafeDelete(pServerSystem);
}