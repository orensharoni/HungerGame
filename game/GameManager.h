#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "Util.h"
#include "Player.h"
#include "GameBoard.h"

const int MAX_PLAYERS_COUNT = 2;
const int INIT_PLAYER_POWER  = 1000;

const int SLEEP_TIME = (int)(1000 * 0.05);

class GameManager
{
	bool		m_stop; // the stop flag to stop the game.
	int			m_alivePlayersCount;
	Player		*m_players[MAX_PLAYERS_COUNT];
	GameBoard	m_gameBoard;
	int			m_gameCounter;
	
	bool isExit(); 

	GameManager(const GameManager&);
public:
	GameManager(char *inputFileName)
		:m_stop(false), m_alivePlayersCount(0), m_gameBoard(inputFileName), 
		m_gameCounter(0)
	{
		m_gameBoard.setGameManager(this);
	}

	~GameManager(void){}

	eStatus prepare();
	void start();
	char addPlayer(int positionX, int positionY);
	Player* getPlayer(int ind);
	void updatePlayerStatus( Player &player, eStatus status );
	void removePlayer(Player *pPlayer);
	void handleCollision( Player &player, const char &collisionSign );
	void handleCollision( Arrow &arrow, const char &collisionSign );
	void handleCollision( Player &player1, Player &player2 );
	void printWinner()const;

	// getters
	int getAlivePlayers()const { return m_alivePlayersCount; }
	
	// setters
	void setAlivePlayers(int val) { m_alivePlayersCount = val;}
};

#endif