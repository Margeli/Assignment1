#ifndef __j1UIELEM_H__
#define __j1UIELEM_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"
#include "p2Point.h"

struct SDL_Texture;

enum FontType { DEFAULT = 0, ARIALN, FRIZQT, MORPHEUS, SKURRI };
enum UIType { CHECK = 1, IMAGE, BUTTON, INPUTBOX,LABEL,  WINDOW };
enum ButtonState { IDLE = 0, HOVER, PRESSED_R, UP_R, PRESSED_L, UP_L, DISABLED };
enum ButtonEvent { MOUSE_INSIDE = 1, MOUSE_OUTSIDE, LEFT_CLICK, LEFT_CLICK_UP, RIGHT_CLICK, RIGHT_CLICK_UP };
enum Alignment { NO_ALIGN, ALIGN_LEFT = 1, ALIGN_CENTERED, ALIGN_RIGHT };

class j1UI_Elem : j1Module
{
public:

	j1UI_Elem(UIType type, Alignment align, j1Module* listener = nullptr);

	virtual ~j1UI_Elem();

	virtual bool Awake(pugi::xml_node&) { return true; };

	virtual bool Start();

	virtual bool Update(float dt);

	virtual bool PostUpdate() { return true; };

	virtual bool CleanUp();

	void UpdateAlignment();

	virtual void DebugDraw();

	virtual void StateChanging(ButtonState state) {};

	SDL_Texture* LoadTexture(p2SString path);

	SDL_Texture* tex;
	UIType type;
	ButtonEvent event = ButtonEvent::MOUSE_OUTSIDE;
	ButtonEvent previous_event = ButtonEvent::MOUSE_OUTSIDE;
	ButtonState state = ButtonState::IDLE;
	Alignment align;
	SDL_Rect rect;
	iPoint position;
	iPoint displacement;
	j1Module* listener;
	bool draw = true;
	bool to_delete = false;


private:
	bool Blit() const { return true; }
	bool debug = false;

protected:
	bool moving = false;
	iPoint click_pos = { 0,0 };
};

#endif // __j1UIELEM_H__