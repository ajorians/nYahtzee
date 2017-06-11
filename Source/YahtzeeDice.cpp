#include "YahtzeeDice.h"

extern "C"
{
#include <os.h>
}

Dice::Dice()
{
	Reset();
}

void Dice::Reset()
{
	m_nValue = -1;
}

int Dice::Roll()
{
	m_nValue = rand() % 6 + 1;
	return m_nValue;
}

int Dice::GetValue() const
{
	return m_nValue;
}

//////////////////////////////////
HoldableDice::HoldableDice()
: Dice(), m_bHold(false)
{
}

void HoldableDice::Reset()
{
	Dice::Reset();
	m_bHold = false;
}

void HoldableDice::Hold(bool bHold)
{
	m_bHold = bHold;
}

bool HoldableDice::GetHolded() const
{
	return m_bHold;
}

//////////////////////////////////

YahtzeeDice::YahtzeeDice()
{
	Reset();
}

void YahtzeeDice::Reset()
{
	for(int i=0; i<NUMBER_OF_DICE; i++)
		m_Dice[i].Reset();
}

void YahtzeeDice::Roll()
{
	for(int i=0; i<NUMBER_OF_DICE; i++)
		if( !GetDiceHolded(i) )
			m_Dice[i].Roll();
}

void YahtzeeDice::HoldDice(int nDiceIndex, bool bHold)
{
	if( nDiceIndex <0 || nDiceIndex >= NUMBER_OF_DICE )
		return;
	
	m_Dice[nDiceIndex].Hold(bHold);
}

bool YahtzeeDice::GetDiceHolded(int nDiceIndex) const
{
	if( nDiceIndex <0 || nDiceIndex >= NUMBER_OF_DICE )
		return false;
	
	return m_Dice[nDiceIndex].GetHolded();
}

int YahtzeeDice::GetDiceValue(int nDiceIndex) const
{
	if( nDiceIndex <0 || nDiceIndex >= NUMBER_OF_DICE )
		return -1;
	
	return m_Dice[nDiceIndex].GetValue();
}

int YahtzeeDice::GetNumberDiceWithValue(int nValue) const
{
	int nCount = 0;
	for(int i=0; i<NUMBER_OF_DICE; i++)
	{
		if( m_Dice[i].GetValue() == nValue )
			nCount++;
	}
	return nCount;
}

int YahtzeeDice::LargestOfAKind() const
{
	int nLargest = 0;
	for(int i=0; i<NUMBER_OF_DICE; i++)
	{
		int nCount = 1;
		for(int j=0; j<NUMBER_OF_DICE; j++)
		{
			if( i == j )
				continue;
			
			if( m_Dice[i].GetValue() == m_Dice[j].GetValue() )
				nCount++;
		}
		if( nCount > nLargest )
			nLargest = nCount;
	}
	return nLargest;
}

bool YahtzeeDice::IsFullHouse() const
{
	//Basically if it consists of 2 numbers and the Largest of a Kind is 3.
	int nNumber1 = m_Dice[0].GetValue(), nNumber2 = m_Dice[1].GetValue();
	if( nNumber1 == nNumber2 )//It could be possible dice 0 & 1 are the same number
		nNumber2 = m_Dice[2].GetValue();
	if( nNumber1 == nNumber2 )//This fixes that!
		nNumber2 = m_Dice[3].GetValue();
	for(int i=2; i<NUMBER_OF_DICE; i++)
	{
		if( m_Dice[i].GetValue() != nNumber1 && m_Dice[i].GetValue() != nNumber2 )
			return false;
	}
	
	return LargestOfAKind() == 3;//Exactly 3; the dice consist of 2 numbers and one of the numbers shows up 3 times.
}

int YahtzeeDice::HighestStraight() const
{
	int nHighestStraight = 0;
	for(int nStartingDice=1; nStartingDice <= 6; nStartingDice++)//Could be improved if speed is an issue
	{
		int nNumber = nStartingDice, nCount = 0;
		for(int i=0; i<NUMBER_OF_DICE; i++)
		{
			for(int j=0; j<NUMBER_OF_DICE; j++)
			{
				if( m_Dice[j].GetValue() == nNumber )
				{
					nNumber++;
					nCount++;
					break;
				}
			}
		}
		if( nCount > nHighestStraight )
			nHighestStraight = nCount;
	}
	return nHighestStraight;
}

int YahtzeeDice::SumOfAllDice() const
{
	int nSum = 0;
	for(int i=0; i<NUMBER_OF_DICE; i++)
		nSum += m_Dice[i].GetValue();
	return nSum;
}

