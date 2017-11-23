#if !_CKEYBOARD_H
#define _CKEYBOARD_H 1

#define KEYBOARD_STATE_NORMAL   0
#define KEYBOARD_STATE_INIT     1
#define KEYBOARD_STATE_USING    2

class CKeyboard
{

public:
	CKeyboard(){};
    int _state;
    void Init(int max_char, int x, int y, int w, int h);
    void Dismiss();
    bool _isCallFormReturn;
    // NOTE : only use buffer if _state == KEYBOARD_STATE_USING
    char *_buffer;
    int _max_char;
    int _current_length;
    
    //event : true will dissmiss, false will not dissmiss
    bool onReturn();
};
#endif