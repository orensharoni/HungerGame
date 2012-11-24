#ifndef _ARROW_H_
#define _ARROW_H_

#include "Util.h"
class GameBoard;

class Arrow
{
	char					m_sign;
	int						m_positionX;
	int						m_positionY;
	Util::eDirections	m_direction;
	int						m_damage;
	GameBoard				&m_gameBoard;


	Arrow(const Arrow&);
public:
	Arrow(GameBoard &gameBoard, const int &positionX, const int &positionY, Util::eDirections direction, int damage = 200)
	  :m_sign('*'), m_positionX(positionX), m_positionY(positionY), m_direction(direction), m_damage(damage), m_gameBoard(gameBoard)
	  {}
	~Arrow(void);

	eStatus move();
	
	// getters
	char getName()const { return m_sign; }
	Util::eDirections getDirection()const { return m_direction; }
	int getPositionX()const { return m_positionX; }
	int getPositionY()const { return m_positionY; }

	// setters
	void setPositionX( int positionX ) { m_positionX = positionX; }
	void setPositionY( int positionY ) { m_positionY = positionY; }
};

#endif // _ARROW_H_