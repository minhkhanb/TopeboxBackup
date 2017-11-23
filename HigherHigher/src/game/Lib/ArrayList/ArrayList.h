#ifndef _ARRAYLIST_H
#define _ARRAYLIST_H

#include "../../Define/Defines.h"

class StringListItem
{
public:
	StringListItem* 	_next;
	int				_len;
	char*			_val;

public:

	StringListItem(const char* val);
	~StringListItem();
};

class StringList
{
public:
	StringListItem* 	_head;
	StringListItem* 	_tail;
	StringListItem* 	_current;
	int					_num_items;
	
public:
		
	StringList();
	~StringList();
	
	void AddNewString(const char* str);
	void ClearAll();
	void RemoveItem(int index);
	
	StringListItem* GetItem(int index);
	StringListItem* GetCurrentItem();
	void NextItem();
	void ResetSearchPointer();
};

class IntListItem
{
public:
	IntListItem* 	_next;
	int				_val;

public:

	IntListItem();
	~IntListItem();
	IntListItem(int val);
};

class IntList
{
public:
	IntListItem* 		_head;
	IntListItem* 		_tail;
	IntListItem* 		_current;
	int					_num_items;
	
public:
		
	IntList();
	~IntList();
	
	void AddNewInt(int val);
	void ClearAll();
	void RemoveItem(int index);
	
	int GetItem(int index);
	int GetCurrentItem();
	void NextItem();
	void ResetSearchPointer();
};

class FloatListItem
{
public:
	FloatListItem* 	_next;
	float			_val;
	
public:
	
	FloatListItem();
	~FloatListItem();
	FloatListItem(float val);
};

class FloatList
{
public:
	FloatListItem* 		_head;
	FloatListItem* 		_tail;
	FloatListItem* 		_current;
	int					_num_items;


public:
	
	FloatList();
	~FloatList();
	
	void AddNewFloat(float val);
	void ClearAll();
	void RemoveItem(int index);
	
	float GetItem(int index);
	float GetCurrentItem();
	void NextItem();
	void ResetSearchPointer();
};


class FloatDictionaryItem
{
public:
	FloatDictionaryItem* 	_next;
	float			_val;
	int			_key;

public:

	FloatDictionaryItem();
	~FloatDictionaryItem();
	FloatDictionaryItem(float val,int key);
};

class FloatDictionary
{
public:
	FloatDictionaryItem* 		_head;
	FloatDictionaryItem* 		_tail;
	FloatDictionaryItem* 		_current;
	int					_num_items;


public:

	FloatDictionary();
	~FloatDictionary();

	void AddNewItem(float val,int key);
	void ClearAll();
	void RemoveItem(int key);

	float GetItem(int key);
	void NextItem();
	void ResetSearchPointer();
};


#endif