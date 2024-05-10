#pragma once

/* 공통적으로 사용하기위한 정의들을 모아놓는다.*/
/* 기타 다른 헤더파일들도 모아놓는다. */
#include <process.h>

//#define WINSIZEX 1280
//#define WINSIZEY 720
namespace Client
{
	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	
}

extern HWND		g_hWnd;

using namespace Client;



