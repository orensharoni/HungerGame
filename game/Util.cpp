#include "Util.h"
#include <windows.h>
#include <process.h>

/***************************************
  Credit for finding this code goes to:
          Yoav  Aharoni
      [yoav_ah@netvision.net.il]
***************************************/
void Util::gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition);
}

void Util::clrscr()
{
	system("cls");
}

void Util::drawXy( int x, int y, char ch /*= CLEAR_SIGN */ )
{
	gotoxy(x, y);
	cout << ch;
}
