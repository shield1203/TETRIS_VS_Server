#pragma once

#define PORT_NUM 21524
#define BLOG_SIZE 10
#define MAX_MSG_LEN 2937
#define MAX_DATA_LEN 2929 // 테트리스에서 최대 블록 개수일 때 데이터크기

enum class USER_STATE : unsigned int { USER_LOBBY, USER_GAME_ROOM, USER_PLAY_GAME, USER_RESULT, CLOSE_CONNECT };

enum COLOR : unsigned short { DARK, DARK_BLUE, DARK_GREEN, DARK_SKY, DARK_RED, DARK_PURPLE, DARK_YELLOW, DARK_WHITE,
	GRAY, BLUE, GREEN, SKY, RED, PURPLE, YELLOW, WHITE };

/////////////
// LINKING //
/////////////
#pragma comment(lib,"ws2_32")

//////////////
// INCLUDES //
//////////////
#include <WinSock2.h>
#include <process.h>
#include <mutex>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>

using namespace std;