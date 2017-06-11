#ifndef YAHTZEESCORE_H
#define YAHTZEESCORE_H

#define YAHTZEE_SCORE_FIELDS	13

class YahtzeeDice;

enum ScoreCategory
{
	AcesCategory = 0,
	TwosCategory,
	ThreesCategory,
	FoursCategory,
	FivesCategory,
	SixesCategory,
	ThreeOfAKindCategory,
	FourOfAKindCategory,
	FullHouseCategory,
	SmStraightCategory,
	LgStraightCategory,
	YahtzeeCategory,
	ChanceCategory
};

class ScoreField
{
public:
	ScoreField();
	void Reset();
	void SetScore(int nScore);
	int GetScore() const;
	bool GetValueSupplied() const;

protected:
	bool m_bValueSupplied;//Whether a value was supplied
	int m_nValue;//The value
};

class YahtzeeScore
{
public:
	YahtzeeScore();
	void Reset();
	void SetScore(ScoreCategory eCategory, int nScore);
	int GetScore(ScoreCategory eCategory) const;
	bool GetValueSupplied(ScoreCategory eCategory) const;
	void HandleIfYahtzee(const YahtzeeDice* pDice);

	bool AnyCategoriesNotSupplied() const;

	int GetTotalNumbers() const;
	int GetBonus() const;
	int GetTotalUpper() const;
	int GetYahtzeeBonus() const;
	int GetTotalLower() const;
	int GetTotal() const;

public:
	static int ScoreForCategory(ScoreCategory eCategory, const YahtzeeDice* pDice);
	static bool IsYahtzee(const YahtzeeDice* pDice);

protected:
	ScoreField m_Scores[YAHTZEE_SCORE_FIELDS];
	int m_nYahtzeeBonus;
};

#endif