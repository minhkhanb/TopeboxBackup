#include "pch.h"
#include <string.h>
#include "ArrayList.h"
#include"stdio.h"
#pragma region StringList

StringListItem::StringListItem(const char* val)
{
	_next = NULL;
	
	_len = strlen(val);
	_val = new char[_len + 1];
	
	strcpy(_val, val);
}

StringListItem::~StringListItem()
{
	SAFE_DEL_ARRAY(_val);
}

StringList::StringList()
{
	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;
}

StringList::~StringList()
{
	ClearAll();
}

void StringList::AddNewString(const char* str)
{
	if (_tail == NULL)
	{
		_head = new StringListItem(str);
		_tail = _head;
	}
	else
	{
		_tail->_next = new StringListItem(str);
		_tail = _tail->_next;
	}
	
	_num_items++;
}

void StringList::ClearAll()
{
	_current = _head;
	
	StringListItem* p = NULL;
	while (_current != NULL)
	{
		p = _current->_next;
		SAFE_DEL(_current);
		_current = p;
	}
	
	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;
}

void StringList::RemoveItem(int index)
{
	if (index < 0 || index >= _num_items)
	{
		return;
	}
	
	if (index == 0)
	{
		StringListItem* p = _head;
		
		_head = _head->_next;
		if (_current == p)
		{
			_current = _head;
		}
		
		SAFE_DEL(p);
		_num_items--;
	}
	else
	{
		ResetSearchPointer();
		
		for (int i = 0; i < index - 1; i++)
		{
			NextItem();
		}
		
		if (_current != NULL)
		{
			StringListItem* p = _current->_next;
			if (p != NULL)
			{
				_current->_next = p->_next;
				SAFE_DEL(p);
				_num_items--;
			}
		}
	}
	if(_num_items==0)
		_tail=_head=NULL;
}

StringListItem* StringList::GetItem(int index)
{
	if (index < 0 || index >= _num_items)
	{
		return NULL;
	}
	
	ResetSearchPointer();
	
	for (int i = 0; i < index; i++)
	{
		NextItem();
	}
	
	return _current;
}

StringListItem* StringList::GetCurrentItem()
{
	return _current;
}

void StringList::NextItem()
{
	if (_current != NULL)
	{
		_current = _current->_next;
	}
}

void StringList::ResetSearchPointer()
{
	_current = _head;
}

#pragma endregion StringList
#pragma region IntList

IntListItem::IntListItem()
{
	_next = NULL;
	_val = 0;
}

IntListItem::IntListItem(int val)
{
	_next = NULL;
	_val = val;
}

IntListItem::~IntListItem()
{
	_next = NULL;
}

IntList::IntList()
{
	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;
}

IntList::~IntList()
{
	ClearAll();
}

void IntList::AddNewInt(int val)
{
	if (_tail == NULL)
	{
		_head = new IntListItem(val);
		_tail = _head;
	}
	else
	{
		_tail->_next = new IntListItem(val);
		_tail = _tail->_next;
	}
	
	_num_items++;
}

void IntList::ClearAll()
{
	_current = _head;
	
	IntListItem* p = NULL;
	while (_current != NULL)
	{
		p = _current->_next;
		SAFE_DEL(_current);
		_current = p;
	}
	
	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;
}

void IntList::RemoveItem(int index)
{
	if (index < 0 || index >= _num_items)
	{
		return;
	}
	
	if (index == 0)
	{
		IntListItem* p = _head;
		
		_head = _head->_next;
		if (_current == p)
		{
			_current = _head;
		}
		
		SAFE_DEL(p);
		_num_items--;
	}
	else
	{
		ResetSearchPointer();
		
		for (int i = 0; i < index - 1; i++)
		{
			NextItem();
		}
		
		if (_current != NULL)
		{
			IntListItem* p = _current->_next;
			if (p != NULL)
			{
				_current->_next = p->_next;
				SAFE_DEL(p);
				_num_items--;
			}
		}
	}
	if(_num_items==0)
		_tail=_head=NULL;
}

int IntList::GetItem(int index)
{
	if (index < 0 || index >= _num_items)
	{
		return NULL;
	}
	
	ResetSearchPointer();
	
	for (int i = 0; i < index; i++)
	{
		NextItem();
	}
	
	return _current->_val;
}

int IntList::GetCurrentItem()
{
	return _current->_val;
}

void IntList::NextItem()
{
	if (_current != NULL)
	{
		_current = _current->_next;
	}
}

void IntList::ResetSearchPointer()
{
	_current = _head;
}
#pragma endregion IntList

#pragma region FloatList

FloatListItem::FloatListItem()
{
	_next = NULL;
	_val = 0;
}

FloatListItem::FloatListItem(float val)
{
	_next = NULL;
	_val = val;
}

FloatListItem::~FloatListItem()
{
	_next = NULL;
}

FloatList::FloatList()
{
	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;

}

FloatList::~FloatList()
{
	ClearAll();
}

void FloatList::AddNewFloat(float val)
{
	//printf("---AddNew: %f \n",val);
	if (_tail == NULL)
	{
		_head = new FloatListItem(val);
		_tail = _head;
	
	}
	else
	{
		_tail->_next = new FloatListItem(val);
		_tail = _tail->_next;
	}
	

	_num_items++;
}

void FloatList::ClearAll()
{
	_current = _head;

	FloatListItem* p = NULL;
	while (_current != NULL)
	{
		p = _current->_next;
		SAFE_DEL(_current);
		_current = p;
	}

	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;
}


void FloatList::RemoveItem(int index)
{
	
	if (index < 0 || index >= _num_items)
	{
		return;
	}

	if (index == 0)
	{
		FloatListItem* p = _head;

		_head = _head->_next;
		if (_current == p)
		{
			_current = _head;
		}
		/*
		//Find other Min Max 
		if(p==_MaxVal)
		{
			_MaxVal=FindMax();
		}
		
		if(p==_MinVal)
		{
			_MinVal=FindMin();
		}
		*/
		SAFE_DEL(p);
		
		_num_items--;
	}
	else
	{
		ResetSearchPointer();

		for (float i = 0; i < index - 1; i++)
		{
			NextItem();
		}

		if (_current != NULL)
		{
			FloatListItem* p = _current->_next;
			if (p != NULL)
			{
				_current->_next = p->_next;
				/*
				//Find other Min Max 
				if(p==_MaxVal)
				{
					_MaxVal=FindMax();
				}

				if(p==_MinVal)
				{
					_MinVal=FindMin();
				}
				*/
				SAFE_DEL(p);
				//printf("Remove: %d",index);
				_num_items--;
			}
		}
	}

	if(_num_items==0)
		_tail=_head=NULL;
	
}

float FloatList::GetItem(int index)
{
	if (index < 0 || index >= _num_items)
	{
		return NULL;
	}

	ResetSearchPointer();

	for (int i = 0; i < index; i++)
	{
		NextItem();
	}

	return _current->_val;
}

float FloatList::GetCurrentItem()
{
	return _current->_val;
}

void FloatList::NextItem()
{
	if (_current != NULL)
	{
		_current = _current->_next;
	}
}

void FloatList::ResetSearchPointer()
{
	_current = _head;
}

#pragma endregion FloatList


#pragma region FloatDictionary

FloatDictionaryItem::FloatDictionaryItem()
{
	_next = NULL;
	_val = 0;
}

FloatDictionaryItem::FloatDictionaryItem(float val,int key)
{
	_next = NULL;
	_key=key;
	_val = val;
}

FloatDictionaryItem::~FloatDictionaryItem()
{
	_next = NULL;
}

FloatDictionary::FloatDictionary()
{
	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;

}

FloatDictionary::~FloatDictionary()
{
	ClearAll();
}

void FloatDictionary::AddNewItem(float val,int key)
{
	//printf("---AddNew: %f \n",val);
	if (_tail == NULL)
	{
		_head = new FloatDictionaryItem(val,key);
		_tail = _head;
	
	}
	else
	{
		_tail->_next = new FloatDictionaryItem(val,key);
		_tail = _tail->_next;
	}
	

	_num_items++;
}

void FloatDictionary::ClearAll()
{
	_current = _head;

	FloatDictionaryItem* p = NULL;
	while (_current != NULL)
	{
		p = _current->_next;
		SAFE_DEL(_current);
		_current = p;
	}

	_head = NULL;
	_tail = NULL;
	_current = NULL;
	_num_items = 0;
}


void FloatDictionary::RemoveItem(int key)
{
// 	ResetSearchPointer();
// 
// 	while(_current!=_tail)
// 	{
// 		if(_current->_next->_key==key)
// 		{
// 			FloatListItem* p = _current->_next;
// 
// 			_head = _head->_next;
// 			if (_current == p)
// 			{
// 				_current = _head;
// 			}
// 			
// 			SAFE_DEL(p);
// 			
// 			_num_items--;
// 		}
// 		
// 	}
	
}

float FloatDictionary::GetItem(int key)
{
	ResetSearchPointer();

	while(_current!=NULL)
	{
		if(_current->_key==key)
			return _current->_val;
		NextItem();
	}
	return -1;
}



void FloatDictionary::NextItem()
{
	if (_current != NULL)
	{
		_current = _current->_next;
	}
}

void FloatDictionary::ResetSearchPointer()
{
	_current = _head;
}

#pragma endregion FloatDictionary