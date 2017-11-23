#include "pch.h"
#include "GameDesign.h"
#include "../../GameCore/Os_Functions.h"
#include "../../Lib/DataInputStream/DataInputStream.h"

//DesignClassDataInt32
void DesignClassDataInt32::Load()
{
	DataInputStream * inputSteam = new DataInputStream();

	ResourceItem resItem = OS_GetResourceItem("GameDesign.bdsg");

	inputSteam->Open(resItem.Path, resItem.Offset + _beginByte, -1);

	_totalAtribute = inputSteam->ReadInt32();
	_totalData = inputSteam->ReadInt32();

	_data = new int[_totalData * _totalAtribute];

	for (int i = 0; i < _totalAtribute*_totalData; i++)
	{
		_data[i] = inputSteam->ReadInt32();
	}

	if (inputSteam)
	{
		inputSteam->Close();
		delete inputSteam;
		inputSteam = NULL;
	}
}

void DesignClassDataInt32::UnLoad()
{
	_totalAtribute = 0;
	_totalData = 0;
	if (_data)
	{
		delete[] _data;
		_data = NULL;
	}
}

int DesignClassDataInt32::getValue(int row, int atribute)
{
	return _data[row * _totalAtribute + atribute];
}

//DesignClassDataInt64
void DesignClassDataInt64::Load()
{
	DataInputStream * inputSteam = new DataInputStream();

	ResourceItem resItem = OS_GetResourceItem("GameDesign.bdsg");

	inputSteam->Open(resItem.Path, resItem.Offset + _beginByte, -1);

	_totalAtribute = inputSteam->ReadInt32();
	_totalData = inputSteam->ReadInt32();

	_data = new INT64[_totalData * _totalAtribute];

	for (int i = 0; i < _totalAtribute*_totalData; i++)
	{
		_data[i] = inputSteam->ReadInt64();
	}

	if (inputSteam)
	{
		inputSteam->Close();
		delete inputSteam;
		inputSteam = NULL;
	}
}

void DesignClassDataInt64::UnLoad()
{
	_totalAtribute = 0;
	_totalData = 0;
	if (_data)
	{
		delete[] _data;
		_data = NULL;
	}
}

INT64 DesignClassDataInt64::getValue(int row, int atribute)
{
	return _data[row * _totalAtribute + atribute];
}

//DesignClassDataFloat
void DesignClassDataFloat::Load()
{
	DataInputStream * inputSteam = new DataInputStream();

	ResourceItem resItem = OS_GetResourceItem("GameDesign.bdsg");

	inputSteam->Open(resItem.Path, resItem.Offset + _beginByte, -1);

	_totalAtribute = inputSteam->ReadInt32();
	_totalData = inputSteam->ReadInt32();

	_data = new float[_totalData * _totalAtribute];

	for (int i = 0; i < _totalAtribute*_totalData; i++)
	{
		_data[i] = inputSteam->RevertFloat32();
	}

	if (inputSteam)
	{
		inputSteam->Close();
		delete inputSteam;
		inputSteam = NULL;
	}
}

void DesignClassDataFloat::UnLoad()
{
	_totalAtribute = 0;
	_totalData = 0;
	if (_data)
	{
		delete[] _data;
		_data = NULL;
	}
}

float DesignClassDataFloat::getValue(int row, int atribute)
{
	return _data[row * _totalAtribute + atribute];
}

//DesignClassDataString
void DesignClassDataString::Load()
{
	DataInputStream * inputSteam = new DataInputStream();

	ResourceItem resItem = OS_GetResourceItem("GameDesign.bdsg");

	inputSteam->Open(resItem.Path, resItem.Offset + _beginByte, -1);

	_totalAtribute = inputSteam->ReadInt32();
	_totalData = inputSteam->ReadInt32();

	_data = new char*[_totalData * _totalAtribute];

	for (int i = 0; i < _totalAtribute*_totalData; i++)
	{
		int lengthString = inputSteam->ReadInt32();
		_data[i] = new char[lengthString + 1];
		for (int j = 0; j < lengthString; j++)
		{
			_data[i][j] = inputSteam->ReadInt8();
		}
		_data[i][lengthString] = '\0';
	}

	if (inputSteam)
	{
		inputSteam->Close();
		delete inputSteam;
		inputSteam = NULL;
	}
}

void DesignClassDataString::UnLoad()
{
	for (int i = 0; i < _totalData * _totalAtribute; i++)
	{
		if (_data[i])
		{
			delete[] _data[i];
			_data[i] = NULL;
		}
	}

	_totalAtribute = 0;
	_totalData = 0;

	if (_data)
	{
		delete[] _data;
		_data = NULL;
	}
}

char* DesignClassDataString::getValue(int row, int atribute)
{
	return _data[row * _totalAtribute + atribute];
}

//ClassDesign
