#pragma once

#define PORT_NUM 21524
#define BLOG_SIZE 5

enum USER_STATE { LOBBY, IN_ROOM, PLAY_GAME };

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
