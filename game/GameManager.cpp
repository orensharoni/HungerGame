#include "GameManager.h"
#include "Util.h"
#include <windows.h>	// needed for sleep function
#include <conio.h>

const int ESC_KEY				= 27;
const int SYS_MSG_LOCATION_X	= 25;
const int SYS_MSG_LOCATION_Y	= 12;

eStatus GameManager::prepare()
{
	eStatus	status = STATUS_SUCCESS;
	
	// Load the game board from file
	status = m_gameBoard.prepare();
	if (is_failed(status))
	{
		return status;
	}

	// print the start positions.
	m_gameBoard.printBoard();

	return status;
}

void GameManager::start()
{
	while (!isExit() && m_alivePlayersCount > 1)
	{
		m_gameCounter++; // assume the game counter is lower than int max value.

		Sleep(SLEEP_TIME);

		if (isExit())
		{
			continue;
		}

		m_gameBoard.moveArrows();

		// move the game players
		if (m_gameCounter % 2 == 0)
		{
			for (int iPlayer = 0; iPlayer < m_alivePlayersCount; iPlayer++)
			{
				m_players[iPlayer]->move();
				if (m_gameCounter % 15 == 0)
				{
					//TODO - implement a shooting algorithm - probably should be in the player class on the shoot method. 
					m_players[iPlayer]->shoot();
				}
			}

			// TODO - handle more then 2 players
			if (m_alivePlayersCount > 1)
			{
				handleCollision(*(m_players[0]), *(m_players[1]));
			}
		}

		if (m_gameCounter % 4 == 0)
		{
			m_gameBoard.addSurprise();
		}
		
	}

	if (m_alivePlayersCount == 1)
	{
		printWinner();
	}
}

char GameManager::addPlayer( int positionX, int positionY )
{
	char playerName;
	if (m_alivePlayersCount >= MAX_PLAYERS_COUNT)
	{
		return CLEAR_SIGN;
	}

	playerName =  'A' + m_alivePlayersCount;
	m_players[m_alivePlayersCount] = new Player(playerName, positionX, positionY, INIT_PLAYER_POWER, m_gameBoard);
	
	m_alivePlayersCount++;
	return playerName;
}

bool GameManager::isExit()
{
	if(_kbhit() && _getch() == ESC_KEY)
	{
		int i = 0;
		Util::clrscr();
		Util::gotoxy(SYS_MSG_LOCATION_X, SYS_MSG_LOCATION_Y + i++);
		cout << "******************************************" << endl;
		Util::gotoxy(SYS_MSG_LOCATION_X, SYS_MSG_LOCATION_Y + i++);
		cout << "*   Why do you want to leave us here?!?  *" << endl;
		Util::gotoxy(SYS_MSG_LOCATION_X, SYS_MSG_LOCATION_Y + i++);
		cout << "*   press Y for exit.                    *" << endl;
		Util::gotoxy(SYS_MSG_LOCATION_X, SYS_MSG_LOCATION_Y + i++);
		cout << "******************************************" << endl;

		char ch = _getch();
		if (ch == 'Y' || ch == 'y')
		{
			m_stop = true;
			return true;
		}
		else
		{
			m_gameBoard.printBoard();
		}
	}

	return false;
}

Player * GameManager::getPlayer( int ind )
{
	return m_players[ind];
}


void GameManager::removePlayer( Player *pPlayer )
{
	for (int i = 0; i < m_alivePlayersCount; i++)
	{
		if(m_players[i] == pPlayer)
		{
			m_gameBoard.putOnBoard(pPlayer->getPositionX(), pPlayer->getPositionY(), CLEAR_SIGN);
			delete pPlayer;
			m_players[i] = NULL;
		}
	}

	m_alivePlayersCount--;
}

void GameManager::handleCollision( Player &player, const char &collisionSign )
{
	// TODO - handle to players that jump against each other - this is a collision.
	switch (collisionSign)
	{
		case FOOD_SIGN:
			player.setPower(player.getPower() + 200);
			break;
		case ARROWS_SIGN:
			player.setArrowsCount(player.getArrowsCount() + 3);
			break;
		case BOOM_SIGN:
			player.setPower(player.getPower() - 750);
			break;
	}

	if (player.getPower() <= 0)
	{
		removePlayer(&player);
	}

	m_gameBoard.printSummary();	
}

void GameManager::handleCollision( Arrow &arrow, const char &collisionSign )
{
	for (int i = 0; i < MAX_PLAYERS_COUNT; i++)
	{
		if (!m_players[i])
		{
			continue;
		}

		if (collisionSign == m_players[i]->getName())
		{
			m_players[i]->setPower(m_players[i]->getPower() - 500);
			m_gameBoard.removeArrow(&arrow);
		}
	}
}

void GameManager::handleCollision( Player &player1, Player &player2 )
{
	bool isCollosion = false;
	
	if ((player1.getPositionX() == player2.getPositionX()) && 
		(player1.getPositionY() == player2.getPositionY()))
	{
		isCollosion = true;
	}

	if (isCollosion)
	{
		if (player1.getPower() > player2.getPower())
		{
			player1.setPower(player1.getPower() - 10);
			player2.setPower(player2.getPower() - 200);
		}
		else if (player2.getPower() > player1.getPower())
		{
			player1.setPower(player1.getPower() - 200);
			player2.setPower(player2.getPower() - 10);
		}
		else // player1.getPlayer() == player2.getPower()
		{
			player1.setPower(player1.getPower() - 50);
			player2.setPower(player2.getPower() - 50);
		}
	}
	
}

void GameManager::printWinner()const
{
	Player *player;
	int		i;
	for (i = 0; i < MAX_PLAYERS_COUNT; i++)
	{
		if (m_players[i])
		{
			player = m_players[i];
			break;
		}
	}

	i = 0;
	Util::clrscr();
	if (player)
	{
		Util::gotoxy(SYS_MSG_LOCATION_X, SYS_MSG_LOCATION_Y + i++);
		cout << "************************" << endl;
		Util::gotoxy(SYS_MSG_LOCATION_X, SYS_MSG_LOCATION_Y + i++);
		cout << "*   The Winner is: " << player->getName() << "   *" << endl;
		Util::gotoxy(SYS_MSG_LOCATION_X, SYS_MSG_LOCATION_Y + i++);
		cout << "************************" << endl;
	}
}
