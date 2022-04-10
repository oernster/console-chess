// ConsoleChess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"

HANDLE g_hQuit = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		g_hQuit = CreateEvent(NULL, FALSE, FALSE, L"Quit event");

		CGame game;

		WaitForSingleObject(g_hQuit, INFINITE);
	}
	// insert exceptions here
	catch(...)
	{

	}
	return 0;
}

