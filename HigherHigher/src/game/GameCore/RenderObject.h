// Pine-Entertainment
// Author: Duy.Nguyen
// Last update: 10/2014
// Class OBJECT
// represent a base class for render object.

#if !defined(_H_GAME_RENDER_OBJECT)
#define _H_GAME_RENDER_OBJECT
#include "Object.h"
#include "../Lib/OpenGLES/Graphic.h"
namespace Game
{
	class RenderObject : public Object
    {

	public:
		RenderObject();
		~RenderObject();
		bool Visible;
		PinePoint Pos;
		virtual void Update() = 0;
		virtual void Render(Graphic* g) = 0;

	};
}
#endif