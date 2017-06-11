#include "YahtzeeGame.h"

#include "YahtzeeDice.h"//For NUMBER_OF_DICE
#include "YahtzeeScore.h"//For ScoreCategory
#include "Yahtzee.h"

#define WAIT_KEYRELEASE_SLEEP		25
#define SELECTOR_CHOICES_CHARWIDTH	5
#define CHOICE_ROW1_HEIGHT			3*CHAR_HEIGHT
#define CHOICE_ROW2_HEIGHT			4.5*CHAR_HEIGHT
#define CHOICE_ROW3_HEIGHT			6*CHAR_HEIGHT
#define CHOICE_ROW4_HEIGHT			7.5*CHAR_HEIGHT
#define CHOICE_ROW5_HEIGHT			9*CHAR_HEIGHT
#define CHOICE_ROW6_HEIGHT			10.5*CHAR_HEIGHT
#define CHOICE_ROW7_HEIGHT			12*CHAR_HEIGHT
#define CHOICE_ROW8_HEIGHT			13.5*CHAR_HEIGHT
#define CHOICE_ROW9_HEIGHT			15*CHAR_HEIGHT

//#define TRACE(x) Trace(x, m_screen)
#define TRACE(x)

void Trace(char* pstr, ScreenBuffer screen)
{
	drawBox_(0, 1.5  * CHAR_HEIGHT, SCREEN_WIDTH, CHAR_HEIGHT, WHITE, screen);
	drawStr(0, 1.5  * CHAR_HEIGHT,    pstr, 1, 1, RED, screen);
	display(screen);
}

SelectorSpot & SelectorSpot_increment(SelectorSpot& eSpot, SelectorSpot eBegin, SelectorSpot eEnd)
{
	return eSpot = (eSpot == eEnd) ? eBegin : SelectorSpot(eSpot+1);
}

SelectorSpot & SelectorSpot_decrement(SelectorSpot& eSpot, SelectorSpot eBegin, SelectorSpot eEnd)
{
	return eSpot = (eSpot == eBegin) ? eBegin : SelectorSpot(eSpot-1);
}

SelectorSpot & operator++ (SelectorSpot& eSpot)
{
	return SelectorSpot_increment(eSpot, RollSpot, MAXSPOT);
}

SelectorSpot & operator-- (SelectorSpot& eSpot)
{
	return SelectorSpot_decrement(eSpot, RollSpot, MAXSPOT);
}

int NumberOfDigits(int n)//Very quickly thrown together
{
	if( n == 0 )
		return 1;
	int nDigits = 0;
	while(n/10 > 0 || n%10 > 0)
	{
		nDigits++;
		n/=10;
	}
	return nDigits;
}

void itoa(int n, char buffer[], int nBufferSize)//Very quickly thrown together
{
	int i=2;
	nBufferSize = min(nBufferSize, NumberOfDigits(n)+1);
	for(; i<=nBufferSize; i++)
	{
		int nRemainder = n%10;
		buffer[nBufferSize-i] = '0' + nRemainder;
		n/=10;
	}
	buffer[nBufferSize-1] = '\0';
}

YahtzeeGame::YahtzeeGame(Yahtzee* pYahtzee, ScreenBuffer screen)
: m_pYahtzee(pYahtzee), m_screen(screen), m_eLastSelection(RollSpot), m_eSelection(RollSpot)
{
	clearScreen(WHITE, m_screen);
	DisplayBoard();
}

bool YahtzeeGame::Loop()
{
	while(true)
	{
		if( QuitKeyPressed() )
		{
			while(any_key_pressed())
				sleep(100);
			return false;
		}
		
		if( !HandleGameKeyPresses() )
			break;
	
	}
	
	return true;
}

bool YahtzeeGame::QuitKeyPressed()
{
   return isKeyPressed(KEY_NSPIRE_ESC) || isKeyPressed(KEY_NSPIRE_Q) || isKeyPressed(KEY_NSPIRE_HOME);
}

bool YahtzeeGame::HandleGameKeyPresses()
{
	if( isKeyPressed(KEY_NSPIRE_LEFT) || isKeyPressed(KEY_NSPIRE_4) )
	{
		while(any_key_pressed())
			sleep(WAIT_KEYRELEASE_SLEEP);
		
		TRACE("Left key pressed");
		
		m_eLastSelection = m_eSelection;
		
		if( m_eSelection == Dice1Spot )//Convenient
			m_eSelection = Dice5Spot;
		else if( m_eSelection >= Dice1Spot && m_eSelection <= Dice5Spot )//A Dice
			--m_eSelection;
		else if( m_eSelection == ThreeOfAKindSpot )
			m_eSelection = AcesSpot;
		else if( m_eSelection == FourOfAKindSpot )
			m_eSelection = TwosSpot;
		else if( m_eSelection == FullHouseSpot )
			m_eSelection = ThreesSpot;
		else if( m_eSelection == SmStraightSpot )
			m_eSelection = FoursSpot;
		else if( m_eSelection == LgStraightSpot )
			m_eSelection = FivesSpot;
		else if( m_eSelection == YahtzeeSpot )
			m_eSelection = SixesSpot;
		else if( m_eSelection == RollSpot && m_pYahtzee->IsGameOver() )//Quit spot when gameover
			m_eSelection = PlayAgainSpot;
		
		DisplayBoard();
	}
	else if( isKeyPressed(KEY_NSPIRE_RIGHT) || isKeyPressed(KEY_NSPIRE_6) )
	{
		while(any_key_pressed())
			sleep(WAIT_KEYRELEASE_SLEEP);
		
		TRACE("Right key pressed");
		
		m_eLastSelection = m_eSelection;
		
		if( m_eSelection == Dice5Spot )//Convenient
			m_eSelection == Dice1Spot;
		else if( m_eSelection <= Dice4Spot && m_eSelection >= ChanceSpot )
			++m_eSelection;
		else if( m_eSelection == AcesSpot )
			m_eSelection = ThreeOfAKindSpot;
		else if( m_eSelection == TwosSpot )
			m_eSelection = FourOfAKindSpot;
		else if( m_eSelection == ThreesSpot )
			m_eSelection = FullHouseSpot;
		else if( m_eSelection == FoursSpot )
			m_eSelection = SmStraightSpot;
		else if( m_eSelection == FivesSpot )
			m_eSelection = LgStraightSpot;
		else if( m_eSelection == SixesSpot )
			m_eSelection = YahtzeeSpot;
		else if( m_eSelection == PlayAgainSpot && m_pYahtzee->IsGameOver() )
			m_eSelection = RollSpot;//Quit spot when gameover
		
		DisplayBoard();
	}
	else if( isKeyPressed(KEY_NSPIRE_UP) || isKeyPressed(KEY_NSPIRE_8) )
	{
		while(any_key_pressed())
			sleep(WAIT_KEYRELEASE_SLEEP);
		
		TRACE("Up key pressed");
		
		m_eLastSelection = m_eSelection;
		
		if( m_eSelection == RollSpot )
		{
			if( !m_pYahtzee->IsGameOver() )
				m_eSelection = Dice1Spot;
		}
		else if( m_eSelection <= ChanceSpot && m_eSelection >= AcesSpot )//Choice spot
			--m_eSelection;
		else if( m_eSelection >= Dice1Spot && m_eSelection <= Dice5Spot )//A dice
			m_eSelection = ChanceSpot;
		
		DisplayBoard();
	}
	else if( isKeyPressed(KEY_NSPIRE_DOWN) || isKeyPressed(KEY_NSPIRE_2) )
	{
		while(any_key_pressed())
			sleep(WAIT_KEYRELEASE_SLEEP);
		
		TRACE("Down key pressed");
		
		m_eLastSelection = m_eSelection;
		
		if( m_eSelection == SixesSpot )//If you want it to go to the 3 of a kind; comment this part out
		{
			m_eSelection = Dice1Spot;
		}
		else if( m_eSelection <= ChanceSpot && m_eSelection >= RollSpot )//Roll or choice
		{
			if( !m_pYahtzee->IsGameOver() )
				++m_eSelection;
		}
		else if( m_eSelection >= Dice1Spot && m_eSelection <= Dice5Spot )//A dice
			m_eSelection = RollSpot;
		
		DisplayBoard();
	}
	else if( isKeyPressed(KEY_NSPIRE_ENTER) || isKeyPressed(KEY_NSPIRE_CTRL) )
	{
		while(any_key_pressed())
			sleep(WAIT_KEYRELEASE_SLEEP);
		
		TRACE("Enter key pressed");
		if( m_eSelection == RollSpot )//And quit spot
		{
			if( !m_pYahtzee->IsGameOver() )
			{
				TRACE("Roll");
				m_pYahtzee->Roll();
				DisplayBoard();
			}
			else
			{
				return false;
			}
		}
		else if( m_eSelection >= Dice1Spot && m_eSelection <= Dice5Spot )
		{
			if( m_pYahtzee->GetRollNumber() > 0 )
			{
				TRACE("Hold Dice");
				m_pYahtzee->HoldDice(m_eSelection - Dice1Spot, !m_pYahtzee->GetDiceHolded(m_eSelection - Dice1Spot));
				DisplayBoard();
			}
		}
		else if( m_eSelection >= AcesSpot && m_eSelection <= ChanceSpot )
		{
			ScoreCategory eCategory = (ScoreCategory)(m_eSelection-1);
			if( !m_pYahtzee->GetScoreEntered(eCategory) && m_pYahtzee->GetRollNumber() > 0 )
			{
				TRACE("Scored spot");
				m_pYahtzee->ScoreSpot(eCategory);//The -1 is to convert SelectionSpot to ScoreCategory
				m_eLastSelection = m_eSelection;
				m_eSelection = RollSpot;
				
				if( m_pYahtzee->IsGameOver() )
				{
					drawBox_(0, 1.5 * CHAR_HEIGHT-1, SCREEN_WIDTH, CHAR_HEIGHT+3, WHITE, m_screen);//Clear Roll /Play again&Quit area
				}
				
				DisplayBoard();
			}
		}
		else if( m_eSelection == PlayAgainSpot && m_pYahtzee->IsGameOver() )
		{
			TRACE("Playing again");
			
			m_pYahtzee->Reset();
			m_eLastSelection = m_eSelection = RollSpot;
			
			drawBox_(0, 1.5 * CHAR_HEIGHT-1, SCREEN_WIDTH, CHAR_HEIGHT+3, WHITE, m_screen);//Clear Roll /Play again&Quit area
			
			DisplayBoard();
		}
	}
	return true;
}

void YahtzeeGame::DisplayBoard()
{
	//Status
	drawBox_(0, 0, SCREEN_WIDTH, CHAR_HEIGHT, WHITE, m_screen);//Clear status
	{
		char buffer[2] = {0};
		drawStr(0, 0  * CHAR_HEIGHT,    "Roll: ", 1, 1, BLUE, m_screen);
		itoa(m_pYahtzee->GetRollNumber(), buffer, 2);
		drawStr(7 * CHAR_WIDTH, 0  * CHAR_HEIGHT,    buffer, 1, 1, BLUE, m_screen);
		drawStr(8 * CHAR_WIDTH, 0  * CHAR_HEIGHT,    "/", 1, 1, BLUE, m_screen);
		itoa(ROLL_LIMIT, buffer, 2);
		drawStr(9 * CHAR_WIDTH, 0  * CHAR_HEIGHT,    "3", 1, 1, BLUE, m_screen);
		drawStr(SCREEN_WIDTH/2, 0  * CHAR_HEIGHT,    "Score:", 1, 1, BLUE, m_screen);
	}
	{
		char buf[5] = {0};
		itoa(m_pYahtzee->GetTotal(), buf,5);
		drawStr(SCREEN_WIDTH/2 + 8 * CHAR_WIDTH, 0  * CHAR_HEIGHT,    buf, 1, 1, BLUE, m_screen);
	}
	
	//Roll Choice
	if( !m_pYahtzee->IsGameOver() )
	{
		drawStr(SCREEN_WIDTH/3, 1.5 * CHAR_HEIGHT, "--ROLL--", 1, 1, BLACK, m_screen);
	}
	else
	{
		drawStr(SCREEN_WIDTH/5, 1.5 * CHAR_HEIGHT, "--PLAY AGIN--", 1, 1, BLACK, m_screen);
		drawStr(3*SCREEN_WIDTH/5, 1.5 * CHAR_HEIGHT, "--QUIT--", 1, 1, BLACK, m_screen);
	}
	
	//Draw Choices
	drawStr(0, CHOICE_ROW1_HEIGHT, 	"Aces        :", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW2_HEIGHT, 	"2s          :", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW3_HEIGHT, 	"3s          :", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW4_HEIGHT, 	"4s          :", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW5_HEIGHT, 	"5s          :", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW6_HEIGHT, 	"6s          :", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW7_HEIGHT, 	"Total       :", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW8_HEIGHT, 	"Bonus (>=63):", 1, 1, BLACK, m_screen);
	drawStr(0, CHOICE_ROW9_HEIGHT, 	"Total Upper :", 1, 1, BLACK, m_screen);
	
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW1_HEIGHT, 	"3 of a kind:", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW2_HEIGHT, 	"4 of a kind:", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW3_HEIGHT, 	"Full house :", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW4_HEIGHT, 	"Sm Straight:", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW5_HEIGHT, 	"Lg Straight:", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW6_HEIGHT, 	"Yahtzee    :", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW7_HEIGHT, 	"Chance     :", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW8_HEIGHT, 	"Yahtzee bonus:", 1, 1, BLACK, m_screen);
	drawStr(SCREEN_WIDTH/2, CHOICE_ROW9_HEIGHT, 	"Total Lower:", 1, 1, BLACK, m_screen);
	
	//Draw Score Values
	drawBox_(CHAR_WIDTH*14, CHOICE_ROW1_HEIGHT, CHAR_WIDTH*5, CHOICE_ROW9_HEIGHT+CHAR_HEIGHT-CHOICE_ROW1_HEIGHT, WHITE, m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW1_HEIGHT, 	m_pYahtzee->GetScore(AcesCategory), m_pYahtzee->GetScoreEntered(AcesCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW2_HEIGHT, 	m_pYahtzee->GetScore(TwosCategory), m_pYahtzee->GetScoreEntered(TwosCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW3_HEIGHT, 	m_pYahtzee->GetScore(ThreesCategory), m_pYahtzee->GetScoreEntered(ThreesCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW4_HEIGHT, 	m_pYahtzee->GetScore(FoursCategory), m_pYahtzee->GetScoreEntered(FoursCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW5_HEIGHT, 	m_pYahtzee->GetScore(FivesCategory), m_pYahtzee->GetScoreEntered(FivesCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW6_HEIGHT, 	m_pYahtzee->GetScore(SixesCategory), m_pYahtzee->GetScoreEntered(SixesCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW7_HEIGHT, 	m_pYahtzee->GetTotalNumbers(), true, m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW8_HEIGHT, 	m_pYahtzee->GetTotalBonus(), true, m_screen);
	DrawScoreValue(CHAR_WIDTH*14, CHOICE_ROW9_HEIGHT, 	m_pYahtzee->GetTotalUpper(), true, m_screen);

	drawBox_(CHAR_WIDTH*34, CHOICE_ROW1_HEIGHT, CHAR_WIDTH*5, CHOICE_ROW9_HEIGHT+CHAR_HEIGHT-CHOICE_ROW1_HEIGHT, WHITE, m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW1_HEIGHT, 	m_pYahtzee->GetScore(ThreeOfAKindCategory), m_pYahtzee->GetScoreEntered(ThreeOfAKindCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW2_HEIGHT, 	m_pYahtzee->GetScore(FourOfAKindCategory), m_pYahtzee->GetScoreEntered(FourOfAKindCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW3_HEIGHT, 	m_pYahtzee->GetScore(FullHouseCategory), m_pYahtzee->GetScoreEntered(FullHouseCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW4_HEIGHT, 	m_pYahtzee->GetScore(SmStraightCategory), m_pYahtzee->GetScoreEntered(SmStraightCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW5_HEIGHT, 	m_pYahtzee->GetScore(LgStraightCategory), m_pYahtzee->GetScoreEntered(LgStraightCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW6_HEIGHT, 	m_pYahtzee->GetScore(YahtzeeCategory), m_pYahtzee->GetScoreEntered(YahtzeeCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW7_HEIGHT, 	m_pYahtzee->GetScore(ChanceCategory), m_pYahtzee->GetScoreEntered(ChanceCategory), m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW8_HEIGHT, 	m_pYahtzee->GetYahtzeeBonus(), true, m_screen);
	DrawScoreValue(CHAR_WIDTH*34, CHOICE_ROW9_HEIGHT, 	m_pYahtzee->GetTotalLower(), true, m_screen);
	
	//Dice
	int nDiceSideLength = SCREEN_WIDTH/(2*NUMBER_OF_DICE+1);//The +1 is because I want a gap and then the first dice
	int nDisplaceAmount = nDiceSideLength/2;
	for(int i=0; i<NUMBER_OF_DICE; i++)
	{
		int nX = nDiceSideLength*(2*i+1), nY = SCREEN_HEIGHT - nDiceSideLength-1 - nDisplaceAmount;//The -1 is be off the bottom
		drawBox_(nX-1, nY-1, nDiceSideLength+3, 1.5*nDiceSideLength+2, WHITE, m_screen);
		
		DrawDice(nX, nY + (m_pYahtzee->GetDiceHolded(i) ? nDisplaceAmount : 0), 
			nDiceSideLength, m_pYahtzee->GetDiceValue(i), m_screen);
	}
	
	if( !(m_eLastSelection >= Dice1Spot && m_eLastSelection <= Dice5Spot) )
		DrawSelector(m_eLastSelection, false);
	DrawSelector(m_eSelection, true);
	
	display(m_screen);
}

void YahtzeeGame::DrawSelector(SelectorSpot eSpot, bool bON)
{
	Color c = bON ? RED : WHITE;
	int nDiceSideLength = SCREEN_WIDTH/(2*NUMBER_OF_DICE+1);//The +1 is because I want a gap and then the first dice
	int nDisplaceAmount = nDiceSideLength/2;
	switch(eSpot)
	{
		case PlayAgainSpot:
			DrawBox(SCREEN_WIDTH/5-1, 1.5 * CHAR_HEIGHT-1, CHAR_WIDTH * 14+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		default:
		case RollSpot:
			if( m_pYahtzee->IsGameOver() )
			{
				DrawBox(3*SCREEN_WIDTH/5-1, 1.5 * CHAR_HEIGHT-1, CHAR_WIDTH * 8+2, CHAR_HEIGHT+2, c, m_screen);
			}
			else
			{
				DrawBox(SCREEN_WIDTH/3-1, 1.5 * CHAR_HEIGHT-1, CHAR_WIDTH * 8+2, CHAR_HEIGHT+2, c, m_screen);
			}
			break;
		case AcesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW1_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case TwosSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW2_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case ThreesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW3_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case FoursSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW4_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case FivesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW5_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case SixesSpot:
			DrawBox(CHAR_WIDTH*14-1, CHOICE_ROW6_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case ThreeOfAKindSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW1_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case FourOfAKindSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW2_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case FullHouseSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW3_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case SmStraightSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW4_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case LgStraightSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW5_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case YahtzeeSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW6_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case ChanceSpot:
			DrawBox(CHAR_WIDTH*34-1, CHOICE_ROW7_HEIGHT-1, CHAR_WIDTH * SELECTOR_CHOICES_CHARWIDTH+2, CHAR_HEIGHT+2, c, m_screen);
			break;
		case Dice1Spot:
			DrawBox(nDiceSideLength*(1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (m_pYahtzee->GetDiceHolded(0) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c, m_screen);
			break;
		case Dice2Spot:
			DrawBox(nDiceSideLength*(2+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (m_pYahtzee->GetDiceHolded(1) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c, m_screen);
			break;
		case Dice3Spot:
			DrawBox(nDiceSideLength*(4+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (m_pYahtzee->GetDiceHolded(2) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c, m_screen);
			break;
		case Dice4Spot:
			DrawBox(nDiceSideLength*(6+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (m_pYahtzee->GetDiceHolded(3) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c, m_screen);
			break;
		case Dice5Spot:
			DrawBox(nDiceSideLength*(8+1)-1, SCREEN_HEIGHT - nDiceSideLength-1 - (m_pYahtzee->GetDiceHolded(4) ? 0 : nDisplaceAmount) -1, //The extra -1 is to surround the box
			nDiceSideLength+2, nDiceSideLength+2, c, m_screen);
			break;
	}
}

void YahtzeeGame::DrawScoreValue(int nX, int nY, int nValue, bool bUsed, ScreenBuffer buffer)
{
	if( nValue < 0 || nValue > 1000 )
		return;
	
	if( !bUsed && m_pYahtzee->GetRollNumber() <= 0 )//If haven't rolled don't display spots haven't choosen
		return;
	
	drawBox_(nX, nY, CHAR_WIDTH*3, CHAR_HEIGHT, WHITE, buffer);
	
	Color c = bUsed ? BLACK : GREEN;
	char buf[4] = {0};
	itoa(nValue, buf, 4);
	drawStr(nX, nY, buf, 1, 1, c, buffer);
}

void YahtzeeGame::DrawDice(int nX, int nY, int nDiceSideLength, int nNumber, ScreenBuffer buffer)
{
	//Draw Box
	DrawBox(nX, nY, nDiceSideLength, nDiceSideLength, BLACK, m_screen);
	drawBox_(nX+1, nY+1, nDiceSideLength-2, nDiceSideLength-2, WHITE, buffer);
	
	//Draw Numbers
	if( nNumber >=1 && nNumber <= 6 )
	{
		int nCircleRadius = 2;
		switch(nNumber)
		{
			default:
			case 1:
				drawDisc_(nX + nDiceSideLength/2, nY + nDiceSideLength/2, nCircleRadius, BLACK, buffer);
				break;
			case 2:
				drawDisc_(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);
				break;
			case 3:
				drawDisc_(nX + nDiceSideLength/2, nY + nDiceSideLength/2, nCircleRadius, BLACK, buffer);
				drawDisc_(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);
				break;
			case 4:
				drawDisc_(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Top-Left
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Top-Right
				drawDisc_(nX + 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Bottom-Left
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Bottom-Right
				break;
			case 5:
				drawDisc_(nX + nDiceSideLength/2, nY + nDiceSideLength/2, nCircleRadius, BLACK, buffer);//Center
				drawDisc_(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Top-Left
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Top-Right
				drawDisc_(nX + 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Bottom-Left
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Bottom-Right
				break;
			case 6:
				drawDisc_(nX + 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Top-Left
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Top-Right
				drawDisc_(nX + 2*nCircleRadius, nY + nDiceSideLength/2, nCircleRadius, BLACK, buffer);//Middle-Left
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength/2, nCircleRadius, BLACK, buffer);//Middle-Right
				drawDisc_(nX + 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Bottom-Left
				drawDisc_(nX + nDiceSideLength - 2*nCircleRadius, nY + nDiceSideLength - 2*nCircleRadius, nCircleRadius, BLACK, buffer);//Bottom-Right
				break;
		}
	}
}

void YahtzeeGame::DrawBox(int nX, int nY, int nWidth, int nHeight, Color c, ScreenBuffer buffer)
{
	drawLine_(nX, 			nY, 			nX+nWidth, 	nY, 		c, buffer);//Top
	drawLine_(nX, 			nY, 			nX, 		nY+nHeight, c, buffer);//Left
	drawLine_(nX, 			nY+nHeight, 	nX+nWidth, 	nY+nHeight, c, buffer);//Bottom
	drawLine_(nX+nWidth, 	nY, 			nX+nWidth, 	nY+nHeight, c, buffer);//Right
}

