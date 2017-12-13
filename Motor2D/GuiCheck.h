#ifndef __GUICHECK_H__
#define __GUICHECK_H__

#include "j1UI_Elem.h"

class GuiLabel;

class GuiCheck : public j1UI_Elem
{
public:

	GuiCheck(Alignment alignment = NO_ALIGN);
	virtual ~GuiCheck();
	bool Start();
	bool CleanUp();
	bool Update(float dt);

	p2SString text;
	bool pressed = false;
	void StateChanging(ButtonState state);

private:
	void AddCheckText();
	SDL_Texture* check = nullptr;
	GuiLabel* checktxt = nullptr;
};

#endif // __GUICHECK_H__