#include "GameManager.h"
#include "Util.h"

#include <iostream>

using namespace std;

void main(int argc, char **argv)
{
	eStatus status;
	if (argc < 1)
	{
		cout << "Provide game board input file path from command line." << endl;
		return;
	}
	
	GameManager	gameManager(argv[1]);
	status = gameManager.prepare();
	if (is_failed(status))
	{
		cout << "failed to prepare the game with the game board: " << argv[0] << endl;
	}

	gameManager.start();
}