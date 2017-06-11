extern "C"
{
#include <os.h>
#include <nGEO.h>
#include <nGUI.h>

#include <nRGBlib.h>
}
#include "Yahtzee.h"
#include "YahtzeeGame.h"

bool StartKeyPressed();
bool QuitKeyPressed();

int main()
{
	ScreenBuffer screen = GetNewScreenBuffer();
	
	while(true)
	{
		// Draw a wellcoming message
		clearScreen(BLACK, screen);
		drawStr(0, 0  * CHAR_HEIGHT,    "             +------------+\n"
										"             |  nYahtzee  |\n"
										"             +------------+\n"
										"\n"
										"---------------------------------------\n"
										"Controls:\n"
										"   - Move around    : Arrow keys\n"
										"   - Hold Dice/Release Dice \n"
										"       or choose score  : Ctrl or Enter\n"
										"\n"
										"Game play:\n"
										"Roll various dice up to three times\n"
										"and choose how to score them for the\n"
										"best score!\n\n"
										"Ctrl/Enter to Start !\n"
										"                      Or Esc to quit...\n", 1, 1, CYAN, screen);
		display(screen);

		// Wait and init rand
		while (!StartKeyPressed() && !QuitKeyPressed())
			rand();
		
		if( QuitKeyPressed() )
		{
			while(any_key_pressed())
				sleep(25);
			break;
		}
		else if( StartKeyPressed() )
		{
			while(any_key_pressed())
				sleep(25);
		}
		
		Yahtzee yahtzee;
		YahtzeeGame game(&yahtzee, screen);
		
		//Game loop
		if( !game.Loop() )
			break;
	}

	free(screen);
	
	// And quit
	return 0;
}

bool StartKeyPressed()
{
   return isKeyPressed(KEY_NSPIRE_CTRL) || isKeyPressed(KEY_NSPIRE_ENTER);
}

bool QuitKeyPressed()
{
   return isKeyPressed(KEY_NSPIRE_ESC) || isKeyPressed(KEY_NSPIRE_Q) || isKeyPressed(KEY_NSPIRE_HOME);
}

