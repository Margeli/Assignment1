#ifndef __GUIINPUT_H__
#define __GUIINPUT_H__

#include "j1UI_Elem.h"

class GuiInput : public j1UI_Elem
{
public:
	GuiInput(Alignment alignment = NO_ALIGN);
	virtual ~GuiInput();
	bool Start();
	bool CleanUp();
	bool Update(float dt);

};

#endif // __GUIINPUT_H__