#ifndef _GAME_BORAD_H_
#define _GAME_BORAD_H_

#include "Util.h"
#include "Player.h"
#include "Arrow.h"

class GameManager;

const int MAX_X	= 79;
const int MAX_Y	= 24;
const int MIN_X	 = 0;
const int MIN_Y	 = 0;
const char WALL_SIGN	= '#';
const char FOOD_SIGN = '@'; //0.2
const char ARROWS_SIGN = '&'; //0.1
const char BOOM_SIGN = '!'; // 0.05
const char SUMMARY_SIGN	= 'O';
const int MAX_ARROWS = 200;

class GameBoard
{
	char				*m_inputFileName;	// fully qualified path to the game input file.
	GameManager			*m_gameManager;
	char				m_board[MAX_Y][MAX_X + 1];
	int					m_summaryX;
	int					m_summaryY;
	Arrow				*m_arrows[MAX_ARROWS];
	int					m_arrowsCount;

	eStatus move(int &x, int &y, char sign, Util::eDirections direction, char &collisionSign);

	GameBoard(const GameBoard&);
public:
	GameBoard(char *pszInputFileName);
	
	~GameBoard(void);

	eStatus prepare();
	void parseLine( int iLine, char line[] );
	void printBoard()const;
	void printSummary()const;
	
	eStatus setSummery( int x, int y );

	eStatus move(Player &player);
	eStatus move(Arrow &arrow);
	void moveArrows();

	void getRandomPosition(int &x, int &y);
	void getLegalPosition( int &x, int &y );
	bool isLegalPosition( const int x, const int y )const;
	
	void putOnBoard(const int x, const int y, const char sign);

	void addSurprise();
	void addArrow( const Player &player);
	void removeArrow( Arrow * arrow );

	//setters
	void setGameManager( GameManager* val ){m_gameManager = val; }
};

#endif // _GAME_BORAD_H_