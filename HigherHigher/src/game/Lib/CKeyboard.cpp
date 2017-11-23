#include "StdAfx.h"
#include "CKeyboard.h"
#include "../GameCore/CGame.h"

extern "C" void TextInputStart(char* textBuf, unsigned int bufSize, int x, int y, int w, int h);
extern "C" void TextInputClose();

void CKeyboard::Init(int max_char, int x, int y, int w, int h)
{
    _isCallFormReturn = false;
    _state = KEYBOARD_STATE_INIT;
    _max_char = max_char;
    _buffer = new char[_max_char+1];
    _current_length = 0;
#ifdef MAC_OS
    TextInputStart(_buffer, _max_char, x, y, w, h);
    CGame::_this->Sleep(200);
#endif
}

void CKeyboard::Dismiss()
{
    if (_state >= KEYBOARD_STATE_USING)
    {
#ifdef MAC_OS
        TextInputClose();
#endif
    }
}

bool CKeyboard::onReturn()
{
#if ADD_DEVICE_KEYBOARD
    return GAME()->CheckRuleCanSubmitQuestion();
#endif
	return false;
}