#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "j1UI_Elem.h"

class GuiLabel;

class GuiButton : public j1UI_Elem
{
public:

	GuiButton(Alignment alignment = NO_ALIGN);
	virtual ~GuiButton();
	bool Start();
	bool CleanUp();
	bool PreUpdate();
	bool Update(float dt);

	p2SString text;
	void StateChanging(ButtonState state);
	void ChangeButtonTxt(p2SString txt);
	void Drag(iPoint displace);
	
	void SetButtonTex(p2SString path_idle, p2SString path_hover, p2SString path_pressed = nullptr);

	bool active = true;

private:
	SDL_Texture* up = nullptr;
	SDL_Texture* down = nullptr;
	SDL_Texture* press = nullptr;
	

	void AddButtonText();
	void DragButtonElements(iPoint displace);
	GuiLabel* buttontext = nullptr;

};

#endif // __GUIBUTTON_H__