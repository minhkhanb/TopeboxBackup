// Pine-Entertainment
// Author: Duy.Nguyen
// Last update: 10/2014
// Class BUTTON
// Represent an animation button.

#if !defined(_H_HUD_BUTTON)
#define _H_HUD_BUTTON
#include "../../GameCore/RenderObject.h"
#include "../DataType/CType.h"
#include "../../GameCore/Animation.h"
namespace HUD{
	enum class ButtonState{
		Normal = 0,
		Active = 1,
		Hover = 2,
		Lock = 4
	};
	class ButtonAnimation: public Game::RenderObject{
	protected:
		CSprite* _curr_sprite;
		Game::Animation _anim;
		int _anim_lock;
		int _anim_normal;
		int _anim_hover;
		int _anim_active;
		float _padding;
	public:
		ButtonState State;

		ButtonAnimation();
		~ButtonAnimation();
		void Init(CSprite* sprt, int anim_lock, int anim_normal, int anim_hover, int anim_active);
		void Release();
		
		//implement RenderObject
		virtual void Update();
		virtual void Render(Graphic* g);
		
		//
		PineRectangle GetBound();
		void SetState(ButtonState state);
		
	};
}
#endif