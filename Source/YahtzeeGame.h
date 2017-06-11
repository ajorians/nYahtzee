#ifndef YAHTZEEGAME_H
#define YAHTZEEGAME_H

extern "C"
{
#include <os.h>
#include <nGEO.h>
#include <nGUI.h>

#include <nRGBlib.h>
}

class Yahtzee;

enum SelectorSpot
{
	PlayAgainSpot = -1,
	RollSpot = 0,//And Quit spot
	AcesSpot,
	TwosSpot,
	ThreesSpot,
	FoursSpot,
	FivesSpot,
	SixesSpot,
	ThreeOfAKindSpot,
	FourOfAKindSpot,
	FullHouseSpot,
	SmStraightSpot,
	LgStraightSpot,
	YahtzeeSpot,
	ChanceSpot,
	Dice1Spot,
	Dice2Spot,
	Dice3Spot,
	Dice4Spot,
	Dice5Spot,
	MAXSPOT
};

SelectorSpot & SelectorSpot_increment(SelectorSpot& eSpot, SelectorSpot eBegin, SelectorSpot eEnd);
SelectorSpot & SelectorSpot_decrement(SelectorSpot& eSpot, SelectorSpot eBegin, SelectorSpot eEnd);
SelectorSpot & operator++ (SelectorSpot& eSpot);
SelectorSpot & operator-- (SelectorSpot& eSpot);

class YahtzeeGame
{
public:
	YahtzeeGame(Yahtzee* pYahtzee, ScreenBuffer screen);
	bool Loop();//False means exit game

protected:
	bool QuitKeyPressed();
	bool HandleGameKeyPresses();//True means continue; False means quit
	void DisplayBoard();
	void DrawSelector(SelectorSpot eSpot, bool bON);
	void DrawScoreValue(int nX, int nY, int nValue, bool bUsed, ScreenBuffer buffer);
	void DrawDice(int nX, int nY, int nDiceSideLength, int nNumber, ScreenBuffer buffer);
	void DrawBox(int nX, int nY, int nWidth, int nHeight, Color c, ScreenBuffer buffer);

protected:
	Yahtzee* m_pYahtzee;//Does not own
	ScreenBuffer m_screen;//Does not own
	SelectorSpot m_eLastSelection;//Where the selection was
	SelectorSpot m_eSelection;//Where the selection is

};

#endif