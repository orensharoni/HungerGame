#include "Player.h"
#include "GameBoard.h"

Player::~Player( void )
{
	m_gameBoard.putOnBoard(m_positionX, m_positionY, CLEAR_SIGN);
}

void Player::move()
{
	// TODO - implement an algorithm for player to move...
	while (is_failed(m_gameBoard.move(*this)))
	{
		int direction = rand() % 4;
		switch (direction)
		{
			case 0: // UP
				m_direction = Util::UP;
				break;
			case 1: // DOWN
				m_direction = Util::DOWN;
				break;
			case 2: // LEFT
				m_direction = Util::LEFT;
				break;
			case 3: // RIGHT
				m_direction = Util::RIGHT;
				break;
		}	
	}
}

eStatus Player::shoot()
{
	// TODO: we should check here if the player shoot an arrow in the last 3 steps of the game to prevent another shoot.
	if (m_arrowsCount > 0)
	{
		m_gameBoard.addArrow(*this);
		m_arrowsCount--;
		return STATUS_SUCCESS;
	}

	return STATUS_FAILED;
}

