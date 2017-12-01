#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "j1UI_Elem.h"

class GuiLabel;


// ---------------------------------------------------
class GuiButton : public j1UI_Elem
{
public:

	GuiButton(Alignment alignment = NO_ALIGN);

	// Destructor
	virtual ~GuiButton();
	bool Start();
	bool CleanUp();
	bool Update(float dt);

	p2SString text;

	
	void StateChanging(ButtonState state);
	void ChangeButtonTxt(p2SString txt);

	void Drag(iPoint displace);


private:
	SDL_Texture* up;
	SDL_Texture* down;
	SDL_Texture* press;

	void AddButtonText();
	void DragButtonElements(iPoint displace);
	GuiLabel* buttontext;


	
	
};

#endif // __GUIBUTTON_H__