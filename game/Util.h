#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

const char CLEAR_SIGN = ' ';

enum eStatus
{
	STATUS_SUCCESS	= 0,	
	STATUS_FAILED	= -1
};

inline bool is_failed(eStatus status)
{
	return status != STATUS_SUCCESS;
}

class Util
{
	Util(void){};
	Util(const Util&){};
public:
	
	enum eDirections {
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

	static void gotoxy(int x, int y); // prototype
	
	static void clrscr(); // prototype

	static void drawXy(int x, int y, char ch = CLEAR_SIGN);
};


#endif //_GAME_UTIL_H_