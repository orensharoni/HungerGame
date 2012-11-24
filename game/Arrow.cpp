#include "Arrow.h"
#include "GameBoard.h"


Arrow::~Arrow( void )
{
	// TODO - check if this place is us.. do not clear the position if it is a player..
	m_gameBoard.putOnBoard(m_positionX, m_positionY, CLEAR_SIGN);
}

eStatus Arrow::move()
{	
	return m_gameBoard.move(*this) ;
}


