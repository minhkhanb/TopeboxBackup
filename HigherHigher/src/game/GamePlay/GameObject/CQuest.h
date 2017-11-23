#if !_CQUEST_H
#define _CQUEST_H 1



class CQuest
{
private:
	CGame* _game;
	Graphic* _gp;
public:
	CQuest();
	~CQuest();
	bool _effect_end;
	float _sizeText;
	int _state;
	float _posX, _posY;
	int _newPercent, _percent;
	float _perScroll, _detaPer;

	char _name[128], _description[1024], _gcID[128], _gplayID[128];
	void LoadInfoQuest(int lvl);
	void SetCurrentValue(int lvl, int value = -1);
	void AddCurrentValue(int lvl, int value);
	void NewValueQuest();

	void InitQuest();
	void RenderQuest(float x, float y, int opacity);
	void UpdateQuest();
	void SetNextQuest();
	enum enumObjective
	{
		Objective_1,
		Objective_2,
		Objective_3,
		Objective_4,
		Objective_5,
		Objective_6,
		Objective_7,
		Objective_8,
		Objective_9,
		Objective_10,
		Objective_11,
		Objective_12,
		Objective_13,
		Objective_14,
		Objective_15,
		Objective_16,
		Objective_17,
		Objective_18,
		Objective_19,
		Objective_20,
		Objective_21,
		Objective_22,
		Objective_23,
		Objective_24,
		Objective_25,
	};

	//
	BYTE _status;
	enum enumQuestStatus
	{
		_QUEST_NONE,
		_QUEST_DOING,
		_QUEST_WAITE,
		_QUEST_COMPLETE,
	};
	int _level;
	int _currentQuest;
	int _currentValue, _maxValue;
	
	int _totalCoin;
	int _dayPlay;
	int _timeRope;
	int _multi;
	
	void CheckQuestComplete();
	void SetQuestComplete(int levl);
	int GetCurrentMulti();

	int _offDescX, _offNextX;
	int _stateEffect;

	float _zoomCheck;
	int _opaCheck;

	float _zoomLevel;
	float _zoomMulti;

	int _timeDelay;
	int _offNameY;

};


#endif
