#include "StdAfx.h"
#include <iostream>
#pragma warning(disable : 4995)
#include "strsafe.h"
#include "Game.h"
#include "Piece.h"
#include "Board.h"
#include "King.h"

using namespace std;

extern HANDLE g_hQuit;

DWORD WINAPI gameThread( LPVOID lpParam );
void errorHandler(LPTSTR lpszFunction);

CGame::CGame(void)
{	
	DWORD dwThreadId = 0;
	HANDLE hThread = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            gameThread,             // thread function name
            NULL,                   // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadId);           // returns the thread identifier 


	// Check the return value for success.
    // If CreateThread fails, terminate execution. 
    // This will automatically clean up threads and memory. 

    if (hThread == NULL) 
    {
        errorHandler(TEXT("CreateThread"));
        ExitProcess(3);
    }

}

CGame::~CGame(void)
{
}

void errorHandler(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK); 

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

// NOTE: REMOVE MAGIC NUMBERS THROUGHOUT CODE

DWORD WINAPI gameThread( LPVOID lpParam ) 
{
	// get user input, do move, verify check status, display result on screen, repeat 
	bool bQuit = false;
	EColour eCurrentColour = eWhite;

	cout << endl << "CONSOLE CHESS BY OLIVER ERNSTER: Version 1.0" << endl << endl << endl;

	// initialise game
	CBoard* pBoard = CBoard::Instance();
	std::string inputMove;
	std::string sourcePos;
	std::string targetPos;
	
	sourcePos.reserve(2);
	targetPos.reserve(2);
		
	while (!bQuit)
	{
		// display board
		pBoard->display();
		
		// get move from user
		cout << endl << "Please enter your move in the form <letter><number>-><letter><number>" << endl;
		cout << "For example: b2->b3" << endl;
		if (eCurrentColour == eWhite)
		{
			cout << endl << "WHITE to move:" << endl;
		}
		else
		{
			cout << endl << "BLACK to move:" << endl;
		}

		inputMove.clear();
		cin >> inputMove;
		
		if (strcmp(inputMove.c_str(), "quit") == 0)
		{
			bQuit = true;
			break;
		}

		sourcePos.clear();
		targetPos.clear();

		sourcePos += *(inputMove.begin());
		sourcePos += *(inputMove.begin()+1);
		targetPos += *(inputMove.begin()+4);
		targetPos += *(inputMove.begin()+5);

		sourcePos[0] = toupper(sourcePos[0]);
		sourcePos[1] = toupper(sourcePos[1]);
		targetPos[0] = toupper(targetPos[0]);
		targetPos[1] = toupper(targetPos[1]);

		CPiece* pPiece = pBoard->getPiece(sourcePos);

		if (pPiece == NULL)
		{
			cout << "No piece found at source position" << endl;
			continue;
		}

		if (pPiece->getColour() == eCurrentColour)
		{
			// do move
			if (!pPiece->move(sourcePos, targetPos))
			{
				cout << "Move failed!  Invalid target position?" << endl;
			}

			if (eCurrentColour == eWhite)
			{
				eCurrentColour = eBlack;
			}
			else
			{
				eCurrentColour = eWhite;
			}
		}
		else
		{
			cout << endl << "INVALID PIECE COLOUR!" << endl;
			continue;
		}


		// does it place the nemesis player in check?
		CKing* pKing = dynamic_cast<CKing*> (pPiece->getKing(eCurrentColour));
		if (pKing != NULL)
		{
			if (pKing->inCheck())
			{
				if (eCurrentColour == eWhite)
				{
					cout << endl << "WHITE is in CHECK!" << endl << endl;
				}
				else
				{
					cout << endl << "BLACK is in CHECK!" << endl << endl;
				}
			}

			// does it place the nemesis player in check mate?
			// if it does, report winner and set bQuit = true;
			if (pKing->checkMate())
			{
				if (eCurrentColour == eWhite)
				{
					cout << endl << "WHITE has been CHECK MATED! BLACK WINS!" << endl << endl;
				}
				else
				{
					cout << endl << "BLACK has been CHECK MATED! WHITE WINS!" << endl << endl;
				}
			
				//bQuit = true;
			}
		}
	}

	pBoard = NULL;

	SetEvent(g_hQuit);

	return 0;
}