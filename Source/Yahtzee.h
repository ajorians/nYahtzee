#ifndef YAHTZEE_H
#define YAHTZEE_H

#include "YahtzeeDice.h"
#include "YahtzeeScore.h"

#define ROLL_LIMIT	3

//A data organizing class for Yahtzee
class Yahtzee
{
public:
	Yahtzee();
	void Reset();
	void ResetTurn();
	void Roll();
	void HoldDice(int nDiceIndex, bool bHold);
	void ScoreSpot(ScoreCategory eCategory);

	bool IsGameOver() const;

	int GetRollNumber() const;
	bool GetDiceHolded(int nDiceIndex) const;//Whether a dice is pull and not to be rolled
	int GetDiceValue(int nDiceIndex) const;//The number value that dice has
	int GetScore(ScoreCategory eCategory) const;//Score of spot and if not taken then what would be if taken
	bool GetScoreEntered(ScoreCategory eCategory) const;//Whether a spot was taken
	int GetTotalNumbers() const;
	int GetTotalBonus() const;
	int GetTotalUpper() const;
	int GetYahtzeeBonus() const;
	int GetTotalLower() const;
	int GetTotal() const;

protected:
	YahtzeeDice m_Dice;
	YahtzeeScore m_Score;
	int m_nRollNumber;
};

#endif