#include "pch.h"
#include "../../GameCore/CGame.h"

const int HSGame::_bg_spriteid[BG_SPRITEID_NUM] ={
	SPRITEID_OBJECT_01,
	SPRITEID_OBJECT_02,
	SPRITEID_OBJECT_03,
};

const int HSGame::_char_spriteid[CHAR_SPRITEID_NUM] ={
	SPRITEID_OBJECT_CHARACTER_01,
	SPRITEID_OBJECT_CHARACTER_02,
	SPRITEID_OBJECT_CHARACTER_03,
	SPRITEID_OBJECT_CHARACTER_04,
	SPRITEID_OBJECT_CHARACTER_05,
	SPRITEID_OBJECT_CHARACTER_06,
	SPRITEID_OBJECT_CHARACTER_07,
	SPRITEID_OBJECT_CHARACTER_08,
	SPRITEID_OBJECT_CHARACTER_09,
	SPRITEID_OBJECT_CHARACTER_10,
};

float HSGame::GetDesignGeneralint(int row, int param)
{
	return _game_design.Level.SheetGeneral.floatValue.getValue(row, param);
}
float HSGame::GetDesignItemFloat(int row, int param)
{
	return _game_design.Level.Sheetitem.floatValue.getValue(row, param);
}
int HSGame::GetDesignAchiveInt(int row, int param)
{
	return _game_design.Achievement.SheetObjective.int32Value.getValue(row, param);
}
const char* HSGame::GetDesignAchiveString(int row, int param)
{
	return _game_design.Achievement.SheetObjective.stringValue.getValue(row, param);
}
int HSGame::GetDesignMultiScoreInt(int row, int param)
{
	return _game_design.Achievement.SheetMultiplier.int32Value.getValue(row, param);
}
const char* HSGame::GetDesignCharString(int row, int param)
{
	return _game_design.Level.Sheetcharacter.stringValue.getValue(row, param);
}
float HSGame::GetDesignSpeedFloat(int row, int param)
{
	if (row >= _game_design.Level.Sheetlevel.floatValue._totalData)
	{
		row = _game_design.Level.Sheetlevel.floatValue._totalData - 1;
	}
	return _game_design.Level.Sheetlevel.floatValue.getValue(row, param);
}