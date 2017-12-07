#ifndef __GUIIMAGE_H__
#define __GUIIMAGE_H__

#include "j1UI_Elem.h"

class GuiImage : public j1UI_Elem
{
public:

	GuiImage(Alignment alignment = NO_ALIGN);
	virtual ~GuiImage();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void Drag(iPoint displace);
};

#endif // __GUIIMAGE_H__