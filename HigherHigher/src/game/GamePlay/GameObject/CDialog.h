
#ifndef CDialog_H
#define CDialog_H

#define DIALOG_BUTTON_GIFT			0

#define DIALOG_BUTTON_RATE_LATER	0
#define DIALOG_BUTTON_RATE			1

#define DIALOG_BUTTON_SHARE_NEXT	0
#define DIALOG_BUTTON_SHARE_YEAH	1

class CDialog
{
private:
	CGame *_game;
	Graphic *_gp;

public:
	CDialog()
	{

	};

	void SetInitGame(CGame *game);

	int _typeDialog;
	int _buttonChoose;
	enum enumTypeDialog
	{
		DIALOG_NONE,
		COMFIRM_SHOW_RATTING,
		COMFIRM_SHOW_GIFT,
		COMFIRM_SHOW_SHARE,
		COMFIRM_SHOW_NOTIFY,
		COMFIRM_USE_BIRD,
	};

	int _typeTrans;
	enum enumTypeTrans
	{
		ZOOM_IN,
		ZOOM_OUT,
		_MOVE_IN_LEFT_,
		_MOVE_OUT_LEFT_,
		_MOVE_IN_RIGHT_,
		_MOVE_OUT_RIGHT_,
		_WAITE_,
	};


	float _sizeTitle, _sizeText;

	int _state, _opacity, _opacityScreen, _opacityName;
	float _centerX, _centerY, _zoomX, _zoomY;
	float _offX, _offY;
	char _title[1024], _description[1024];

	void InitTrans(int typeTrans);	
	void AddDialogPopup(int typeDialog, int typeTrans);
	
	void AcctionButton(int index);
	void WaiteProcessing();
	void Render(float x, float y);
	void RenderDialogPopup(float x, float y, PineMatrix4 tranf);
	

	bool Update();
	void SetStateTrans(int type);
	bool UpdateTrans();
	Button _button[2];
	bool TouchButton(int touchX, int touchY, int touchState, int touchID);
	bool UpdateTouch();
	bool ButtonProcessing();

	void InitShare();
	void RenderShare(float x, float y, int opa);
	bool UpdateShare();
    bool IsOpening();
};

#endif 

