// Pine-Entertainment
// Author: Duy.Nguyen
// Last update: 10/2014
// Class Animation
// represent an animation.

#if !defined(_H_GAME_ANIMATION)
#define _H_GAME_ANIMATION
#include "RenderObject.h"
#include "../Lib/Sprite/CSprite.h"
namespace Game{
	class Animation: public RenderObject{
	protected:
		CSprite* _curr_sprite;
		int _curr_anim;
		
		int _curr_aframe;
		int _curr_aframe_time;
		
		bool _is_stop;
		int _num_loop;

		int _num_aframe;
		int _min_aframe;
		int* _frame_time;

		int _total_frame_time;
	public:
		Animation();
		~Animation();
		
		void Init(CSprite* sprt, int anim = 0, int num_loop = -1);
		void ChangeSpriteAnim(int anim, int num_loop = -1);
		void Release();
		void Stop();
		void Resume();
		void SeekAFrame(int from, int aframe_offset);
		void SeekToTime(int from, int time_offset);
		bool IsStoped();
		int GetCurrentAFrame();

		//Implement RenderObject
		virtual void Update();
		virtual void Render(Graphic* g);
	};
}
#endif