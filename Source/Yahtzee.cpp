#include "Yahtzee.h"

extern "C"
{
#include <os.h>
}

Yahtzee::Yahtzee()
{
	Reset();
}

void Yahtzee::Reset()
{
	m_Score.Reset();
	ResetTurn();
}

void Yahtzee::ResetTurn()
{
	m_nRollNumber = 0;
	m_Dice.Reset();
}

void Yahtzee::Roll()
{
	if( m_nRollNumber >= ROLL_LIMIT )
		return;
	
	m_nRollNumber++;
	m_Dice.Roll();
}

void Yahtzee::HoldDice(int nDiceIndex, bool bHold)
{
	if( m_nRollNumber <= 0 )
		return;
	
	m_Dice.HoldDice(nDiceIndex, bHold);
}

void Yahtzee::ScoreSpot(ScoreCategory eCategory)
{
	if( m_Score.GetValueSupplied(eCategory) || m_nRollNumber <= 0 )
		return;
	
	m_Score.HandleIfYahtzee(&m_Dice);
	
	m_Score.SetScore(eCategory, YahtzeeScore::ScoreForCategory(eCategory, &m_Dice));
	
	m_Dice.Reset();
	m_nRollNumber = 0;
}

bool Yahtzee::IsGameOver() const
{
	return !m_Score.AnyCategoriesNotSupplied();
}

int Yahtzee::GetRollNumber() const
{
	return m_nRollNumber;
}

bool Yahtzee::GetDiceHolded(int nDiceIndex) const
{
	return m_Dice.GetDiceHolded(nDiceIndex);
}

int Yahtzee::GetDiceValue(int nDiceIndex) const
{
	return m_Dice.GetDiceValue(nDiceIndex);
}

int Yahtzee::GetScore(ScoreCategory eCategory) const
{
	return GetScoreEntered(eCategory) ? m_Score.GetScore(eCategory) : YahtzeeScore::ScoreForCategory(eCategory, &m_Dice);
}

bool Yahtzee::GetScoreEntered(ScoreCategory eCategory) const
{
	return m_Score.GetValueSupplied(eCategory);
}

int Yahtzee::GetTotalNumbers() 	const 	{ return m_Score.GetTotalNumbers(); }
int Yahtzee::GetTotalBonus() 	const 	{ return m_Score.GetBonus(); 		}
int Yahtzee::GetTotalUpper() 	const 	{ return m_Score.GetTotalUpper(); 	}
int Yahtzee::GetYahtzeeBonus() 	const 	{ return m_Score.GetYahtzeeBonus(); }
int Yahtzee::GetTotalLower() 	const 	{ return m_Score.GetTotalLower(); 	}
int Yahtzee::GetTotal() 		const 	{ return m_Score.GetTotal(); 		}

