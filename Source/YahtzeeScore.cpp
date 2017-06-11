#include "YahtzeeScore.h"

extern "C"
{
#include <os.h>
}

#include "YahtzeeDice.h"

ScoreField::ScoreField()
{
	Reset();
}

void ScoreField::Reset()
{
	m_bValueSupplied = false;
	m_nValue = 0;
}

void ScoreField::SetScore(int nScore)
{
	m_bValueSupplied = true;
	m_nValue = nScore;
}

int ScoreField::GetScore() const
{
	if( !m_bValueSupplied )
		return 0;
	return m_nValue;
}

bool ScoreField::GetValueSupplied() const
{
	return m_bValueSupplied;
}

YahtzeeScore::YahtzeeScore()
{
	Reset();
}

void YahtzeeScore::Reset()
{
	for(int i=0; i<YAHTZEE_SCORE_FIELDS; i++)
		m_Scores[i].Reset();
	m_nYahtzeeBonus = 0;
}

void YahtzeeScore::SetScore(ScoreCategory eCategory, int nScore)
{
	int nIndex = (int)eCategory;
	if( nIndex < 0 || nIndex > YAHTZEE_SCORE_FIELDS )
		return;
	
	return m_Scores[nIndex].SetScore(nScore);
}

int YahtzeeScore::GetScore(ScoreCategory eCategory) const
{
	int nIndex = (int)eCategory;
	if( nIndex < 0 || nIndex > YAHTZEE_SCORE_FIELDS )
		return -1;
	
	return m_Scores[nIndex].GetScore();
}

bool YahtzeeScore::GetValueSupplied(ScoreCategory eCategory) const
{
	int nIndex = (int)eCategory;
	if( nIndex < 0 || nIndex > YAHTZEE_SCORE_FIELDS )
		return -1;
	
	return m_Scores[nIndex].GetValueSupplied();
}

void YahtzeeScore::HandleIfYahtzee(const YahtzeeDice* pDice)
{
	//So if you entered a non-zero for the yahtzee field then you get a bonus for each yahtzee
	if( IsYahtzee(pDice) && GetValueSupplied(YahtzeeCategory) && GetScore(YahtzeeCategory) > 0 )
	{
		m_nYahtzeeBonus += 100;
	}
}

bool YahtzeeScore::AnyCategoriesNotSupplied() const
{
	for(int i=0; i<YAHTZEE_SCORE_FIELDS; i++)
		if( !m_Scores[i].GetValueSupplied() )
			return true;
	return false;
}

int YahtzeeScore::GetTotalNumbers() const
{
	return GetScore(AcesCategory) + GetScore(TwosCategory) + GetScore(ThreesCategory) + 
		GetScore(FoursCategory) + GetScore(FivesCategory) + GetScore(SixesCategory);
}

int YahtzeeScore::GetBonus() const
{
	return GetTotalNumbers() >= 63 ? 35 : 0;
}

int YahtzeeScore::GetTotalUpper() const
{
	return GetTotalNumbers() + GetBonus();
}

int YahtzeeScore::GetYahtzeeBonus() const
{
	return m_nYahtzeeBonus;
}

int YahtzeeScore::GetTotalLower() const
{
	return GetScore(ThreeOfAKindCategory) + GetScore(FourOfAKindCategory) + GetScore(FullHouseCategory) + 
		GetScore(SmStraightCategory) + GetScore(LgStraightCategory) + GetScore(YahtzeeCategory) + GetScore(ChanceCategory) + GetYahtzeeBonus();
}

int YahtzeeScore::GetTotal() const
{
	return GetTotalUpper() + GetTotalLower();
}

int YahtzeeScore::ScoreForCategory(ScoreCategory eCategory, const YahtzeeDice* pDice)
{
	switch(eCategory)
	{
		default:
		case AcesCategory:
			return pDice->GetNumberDiceWithValue(1);
			break;
		case TwosCategory:
			return 2*pDice->GetNumberDiceWithValue(2);
			break;
		case ThreesCategory:
			return 3*pDice->GetNumberDiceWithValue(3);
			break;
		case FoursCategory:
			return 4*pDice->GetNumberDiceWithValue(4);
			break;
		case FivesCategory:
			return 5*pDice->GetNumberDiceWithValue(5);
			break;
		case SixesCategory:
			return 6*pDice->GetNumberDiceWithValue(6);
			break;
		case ThreeOfAKindCategory:
			return pDice->LargestOfAKind() >= 3 ? pDice->SumOfAllDice() : 0;
			break;
		case FourOfAKindCategory:
			return pDice->LargestOfAKind() >= 4 ? pDice->SumOfAllDice() : 0;
			break;
		case FullHouseCategory:
			return pDice->IsFullHouse() ? 25 : 0;
			break;
		case SmStraightCategory:
			return pDice->HighestStraight() >= 4 ? 30 : 0;
			break;
		case LgStraightCategory:
			return pDice->HighestStraight() >= 5 ? 40 : 0;
			break;
		case YahtzeeCategory:
			return IsYahtzee(pDice) ? 50 : 0;
			break;
		case ChanceCategory:
			return pDice->SumOfAllDice();
			break;
	}
	
	return 0;
}

bool YahtzeeScore::IsYahtzee(const YahtzeeDice* pDice)
{
	return pDice->LargestOfAKind() >= 5;
}

