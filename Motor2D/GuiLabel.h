#ifndef __GUILABEL_H__
#define __GUILABEL_H__

#include "j1UI_Elem.h"
#include "SDL_TTF\include\SDL_ttf.h"

class GuiLabel : public j1UI_Elem
{
public:

	GuiLabel(Alignment alignment = NO_ALIGN);
	virtual ~GuiLabel();
	bool Start();
	bool CleanUp();
	bool Update(float dt);
	
	void CreateText(p2SString txt, SDL_Color color, FontType font);
	void ChangeText(p2SString newtext);
	void Drag(iPoint displace);

private:
	p2SString text;
	SDL_Color text_color;
	_TTF_Font* text_font;
		
};

#endif // __GUILABEL_H__