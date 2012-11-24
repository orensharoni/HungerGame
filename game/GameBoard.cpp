#include "GameBoard.h"
#include "GameManager.h"

GameBoard::GameBoard(char *pszInputFileName)
	:m_inputFileName(pszInputFileName), m_arrowsCount(0)
{
	m_summaryX = -1;
	m_summaryY = -1;

	for (int i = 0; i < MAX_ARROWS; i++)
	{
		m_arrows[i] = NULL;
	}
}


GameBoard::~GameBoard( void )
{
	for (int i = 0; i < m_arrowsCount; i++)
	{
		delete m_arrows[i];
	}
}

eStatus GameBoard::prepare()
{
	eStatus status = STATUS_SUCCESS;
	ifstream 	inputfile;
	int 		iLine;
	char line[MAX_X + 1];

	// load the input file.
	inputfile.open(m_inputFileName);
	for ( iLine = 0; iLine < MAX_Y; iLine++)
	{
		
		inputfile.getline(line, MAX_X + 1);
		
		// parse the line and prepare it to the memory board
		parseLine(iLine, line);
		
	}
	
	// check if the file ended to early
	if (iLine < MAX_Y)
	{
		return STATUS_FAILED;
	}

	// if there is no summary position found set to default (1,1)
	setSummery(1, 1);

	// create the missing players
	while (m_gameManager->getAlivePlayers() < MAX_PLAYERS_COUNT)
	{
		int x, y;
		getRandomPosition(x, y);
		m_gameManager->addPlayer(x, y);
	}

	return status;
}

void GameBoard::parseLine( int iLine, char line[] )
{
	for (int iCol = 0; iCol < MAX_X; iCol++)
	{
		switch (line[iCol])
		{
			case 'W':
				m_board[iLine][iCol] = WALL_SIGN;
				break;
			case 'P':
				m_board[iLine][iCol] = m_gameManager->addPlayer(iCol, iLine);
				break;
			case 'O':
				if (is_failed(setSummery(iCol, iLine)))
				{
					m_board[iLine][iCol] = CLEAR_SIGN;
				}
				break;
			default:
				if (m_board[iLine][iCol] != WALL_SIGN && m_board[iLine][iCol] != SUMMARY_SIGN)
				{
					m_board[iLine][iCol] = CLEAR_SIGN;
				}
				break;
		}
	}
	m_board[iLine][MAX_X] = '\0';
}

void GameBoard::printBoard()const
{

	Util::gotoxy(0, 0);
	for (int i = 0; i < MAX_Y; i++)
	{
		cout << m_board[i];
		if (i != MAX_Y)
		{
			cout << endl;
		}
	}
	
	printSummary();
}

eStatus GameBoard::move( Player &player)
{
	eStatus status = STATUS_SUCCESS;
	char	collisionSign;

	int positionX = player.getPositionX();
	int positionY = player.getPositionY();
	
	status = move(positionX, positionY, player.getName(), player.getDirection(), collisionSign);

	if (status != STATUS_FAILED)
	{
		player.setPositionX(positionX);
		player.setPositionY(positionY);
		m_gameManager->handleCollision(player, collisionSign);
	}
	
	return status;
}

eStatus GameBoard::move( Arrow &arrow)
{
	eStatus status = STATUS_SUCCESS;
	char	collisionSign;

	int positionX = arrow.getPositionX();
	int positionY = arrow.getPositionY();

	status = move(positionX, positionY, arrow.getName(), arrow.getDirection(), collisionSign);

	if (status == STATUS_SUCCESS)
	{
		arrow.setPositionX(positionX);
		arrow.setPositionY(positionY);
		m_gameManager->handleCollision(arrow, collisionSign);
	}

	return status;
}

eStatus GameBoard::move( int &x, int &y, char sign, Util::eDirections direction, char &collisionSign )
{
	eStatus status = STATUS_SUCCESS;
	int	dirX, dirY;
	int nextPositionX = x;
	int nextPositionY = y;

	switch (direction)
	{
		case Util::UP: 
			dirY = -1;
			dirX = 0;
			break;
		case Util::DOWN: 
			dirY = 1;
			dirX = 0;
			break;
		case Util::LEFT: 
			dirY = 0;
			dirX = -1;
			break;
		case Util::RIGHT: 
			dirY = 0;
			dirX = 1;
			break;
	}

	// now do the move
	nextPositionX += dirX;
	if(nextPositionX >= MAX_X) {
		nextPositionX = MIN_X;
	}
	else if(nextPositionX < MIN_X) {
		nextPositionX = MAX_X - 1;
	}

	nextPositionY += dirY;
	if(nextPositionY >= MAX_Y) {
		nextPositionY = MIN_Y;
	}
	else if(nextPositionY < MIN_Y) {
		nextPositionY = MAX_Y - 1;
	}

	// TODO: should be function that check that the next position is legal for this player
	switch (m_board[nextPositionY][nextPositionX])
	{
		case WALL_SIGN:
			return STATUS_FAILED;
			break;
		default:
			collisionSign = m_board[nextPositionY][nextPositionX];
			break;
	}

	// Clear the current position only if it is the same sign. 
	// e.g player that shoots an arrow should not be cleared..
	if (m_board[y][x] == sign)
	{
		putOnBoard(x, y, CLEAR_SIGN);
	}
	putOnBoard(nextPositionX, nextPositionY, sign);

	x = nextPositionX;
	y = nextPositionY;

	return status;
}

void GameBoard::getRandomPosition(int &x, int &y)
{
	bool bIsLegalPosition = false;
	
	while (!bIsLegalPosition)
	{
		x = rand() % MAX_X + 1;
		y = rand() % MAX_Y + 1;

		bIsLegalPosition = isLegalPosition(x, y);
	}
}

bool GameBoard::isLegalPosition( const int x, const int y )const
{
	// TODO - isLegalPosition to be implemented.

	// TODO  decide if leave it like this or check whether the character is 'O'
	if((x >= m_summaryX - 1 ) && (x <= m_summaryX + 10) && (y >= m_summaryY - 1) && (y <= m_summaryY + 5))
	{
		return false;
	}

	if (m_board[y][x] == WALL_SIGN)
	{
		return false;
	}

	for (int iY = y - 2; iY < y + 2; iY++)
	{
		if (iY > MAX_Y || iY < MIN_Y)
			continue;

		if (m_board[iY][x] != CLEAR_SIGN && m_board[iY][x] != WALL_SIGN)
			return false;
	}

	for (int iX = x - 2; iX < x + 2; iX++)
	{
		if (iX > MAX_X || iX < MIN_X)
			continue;

		if (m_board[y][iX] != CLEAR_SIGN && m_board[y][iX] != WALL_SIGN)
			return false;
	}

	return true;
}

eStatus GameBoard::setSummery( int x, int y )
{
	if (m_summaryX == -1 && m_summaryY == -1)
	{
		if (x > MAX_X - 11 || x < MIN_X + 1)
		{
			return STATUS_FAILED;
		}
		if (y > MAX_Y - 6 || y < MIN_Y + 1)
		{
			return STATUS_FAILED;
		}

		m_summaryX = x;
		m_summaryY = y;
		char ch;
		for (int iY = m_summaryY - 1; iY < m_summaryY + 6; iY++)
		{
			for (int iX = m_summaryX - 1; iX < m_summaryX + 11; iX++)
			{
				if((iY == m_summaryY - 1 || iY == m_summaryY + 5) ||
					(iX == m_summaryX - 1 || iX == m_summaryX + 10))
				{
					ch = WALL_SIGN;
				}
				else
				{
					ch = SUMMARY_SIGN;
				}

				m_board[iY][iX] = ch;
			}
		}

		return STATUS_SUCCESS;
	}

	return STATUS_FAILED;
}

void GameBoard::printSummary()const
{
	Player *player;
	int playerCount = MAX_PLAYERS_COUNT;

	Util::gotoxy(m_summaryX, m_summaryY);
	cout << "          ";

	for(int i = 0; i < playerCount; i++ )
	{
		Util::gotoxy(m_summaryX, m_summaryY + i+1);
		player = m_gameManager->getPlayer(i);

		if (player)
		{
			cout << " " << player->getName() << " " << player->getPower() << " " 
				<< player->getArrowsCount() << " " << endl;
		}
		
	}

	for(int i = m_gameManager->getAlivePlayers() + 1; i <5; i++ )
	{
		Util::gotoxy(m_summaryX, m_summaryY + i);
		cout << "          ";
		//cout << setfill(CLEAR_SIGN) << setw(10) << endl; TODO - fix or delete
	}
}

//TODO - decide if we want it in different class
void GameBoard::addSurprise()
{
	int x, y;
	int token = rand() % 1000;

	if (token % 5 == 0) // Food
	{
		getLegalPosition(x, y);
		putOnBoard(x, y, FOOD_SIGN);
	}

	if (token % 10 == 0) // Arrows
	{
		getLegalPosition(x, y);
		putOnBoard(x, y, ARROWS_SIGN);
	}

	if (token % 20 == 0) // BOOM
	{
		getLegalPosition(x, y);
		putOnBoard(x, y, BOOM_SIGN);
	}
}

void GameBoard::getLegalPosition( int &x, int &y )
{
	do 
	{
		getRandomPosition(x, y);
	} while (!isLegalPosition(x,y));
}

void GameBoard::putOnBoard( const int x, const int y, const char sign )
{
	Util::drawXy(x, y, sign);
	m_board[y][x] = sign;
}

void GameBoard::addArrow( const Player &player )
{
	bool bDone = false;
	Arrow *arrow = new Arrow  (*this, player.getPositionX(), player.getPositionY(), player.getDirection());

	for (int i = 0; i < m_arrowsCount; i++)
	{
		if(!m_arrows[i])
		{
			m_arrows[i] = arrow;
			bDone = true;
			break;
		}
	}
	 
	if (!bDone && m_arrowsCount < MAX_ARROWS)
	{
		m_arrows[m_arrowsCount] = arrow;
		m_arrowsCount++;
	}

	arrow->move();
}


void GameBoard::removeArrow( Arrow * arrow )
{	
	for (int i = 0; i < m_arrowsCount; i++)
	{
		if (m_arrows[i] == arrow)
		{
			delete arrow;
			m_arrows[i] = NULL;
			break;
		}	
	}
}

void GameBoard::moveArrows()
{	
	for( int i = 0 ; i < m_arrowsCount; i++)
	{
		if (!m_arrows[i])
		{
			continue;
		}

		if (is_failed(m_arrows[i]->move()))
		{
			removeArrow(m_arrows[i]);
		}
	}
}
