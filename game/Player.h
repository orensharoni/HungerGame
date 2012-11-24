#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Util.h"

const int  INIT_PLAYER_ARROWS = 4;
class GameBoard;

class Player
{
	char					m_name;
	int						m_positionX;
	int						m_positionY;
	int						m_power;
	GameBoard				&m_gameBoard;
	Util::eDirections		m_direction;
	int						m_arrowsCount;
	
	Player(const Player&);
public:
	Player(char name, int x, int y, int power, GameBoard &gameBoard)
		: m_name(name), m_positionX(x), m_positionY(y), m_power(power), m_gameBoard(gameBoard), 
		m_direction(Util::RIGHT), m_arrowsCount(INIT_PLAYER_ARROWS) {}
	~Player(void);
	
	void move();
	eStatus shoot();

	// Getters 
	char getName()const { return m_name; }
	int getPositionX()const { return m_positionX; }
	int getPositionY()const { return m_positionY; }
	int getPower()const { return m_power; }
	int getArrowsCount()const { return m_arrowsCount; }
	Util::eDirections getDirection()const { return m_direction; }

	// Setters 
	void setPower(int val) { m_power = val; }
	void setPositionX(int val) { m_positionX = val; }
	void setPositionY(int val) { m_positionY = val; }
	void setArrowsCount(int val) { m_arrowsCount = val; }

};

#endif // _PLAYER_H_

