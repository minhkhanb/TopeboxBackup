#if !defined(_H_JNI_ENVIRONMENT)
#define _H_JNI_ENVIRONMENT
#include "../../../game/GameCore/CGame.h"
#include "../../../game/Lib/Sound/OpenSLES/SoundOpenSLES.h"
class Environment{
protected:
	float ScreenWidth, ScreenHeight;
	bool IsInit;
public:
	
	static Environment* GetInstance();
	void Init(int device_width, int device_height);
	void SizeChange(int device_width, int device_height);
	void Update();
	void Render();
	void TouchBegin(int id, int x, int y);
	void TouchDrag(int id, int x, int y);
	void TouchEnd(int id, int x, int y);
	void ChangeOrientation(int ori);
	void Suppend();
	void Resume();
	bool BackKeyPress();
private:
	Environment();
	static Environment* _this;
	SoundOpenSLES _sound_engine;
	CGame _game;
	bool _is_pause;
};
#endif//_H_JNI_ENVIRONMENT