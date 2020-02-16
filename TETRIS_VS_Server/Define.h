#pragma once

#define PORT_NUM 21524
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256

enum USER_STATE : unsigned int { LOBBY, IN_ROOM, PLAY_GAME, CLOSE_CONNECT };

/////////////
// LINKING //
/////////////
#pragma comment(lib,"ws2_32")

//////////////
// INCLUDES //
//////////////
#include <WinSock2.h>
#include <process.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>

using namespace std;