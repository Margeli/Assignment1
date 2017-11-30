#ifndef __GUIINPUT_H__
#define __GUIINPUT_H__

#include "j1UI_Elem.h"


// ---------------------------------------------------
class GuiInput : public j1UI_Elem
{
public:

	GuiInput(Alignment alignment = NO_ALIGN);

	// Destructor
	virtual ~GuiInput();
	bool Start();
	bool CleanUp();
	bool Update(float dt);




private:

};

#endif // __GUIINPUT_H__