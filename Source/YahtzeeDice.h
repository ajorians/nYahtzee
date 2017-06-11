#ifndef YAHTZEEDICE_H
#define YAHTZEEDICE_H

#define NUMBER_OF_DICE	5

class Dice
{
public:
	Dice();
	void Reset();
	int Roll();
	int GetValue() const;

protected:
	int m_nValue;
};

class HoldableDice: public Dice
{
public:
	HoldableDice();
	void Reset();
	void Hold(bool bHold);
	bool GetHolded() const;

protected:
	bool m_bHold;
};

class YahtzeeDice
{
public:
	YahtzeeDice();
	void Reset();
	void Roll();
	void HoldDice(int nDiceIndex, bool bHold);
	bool GetDiceHolded(int nDiceIndex) const;
	int GetDiceValue(int nDiceIndex) const;
	int GetNumberDiceWithValue(int nValue) const;
	int LargestOfAKind() const;
	bool IsFullHouse() const;
	int HighestStraight() const;
	int SumOfAllDice() const;

protected:
	HoldableDice m_Dice[NUMBER_OF_DICE];
};

#endif