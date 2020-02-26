#pragma once

#define PORT_NUM 21524
#define BLOG_SIZE 5
#define MAX_MSG_LEN 512

enum class USER_STATE : unsigned int { USER_LOBBY, USER_GAME_ROOM, USER_PLAY_GAME, CLOSE_CONNECT };

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