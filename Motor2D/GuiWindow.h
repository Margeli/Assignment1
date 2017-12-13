#ifndef __GUIWINDOW_H__
#define __GUIWINDOW_H__

#include "j1UI_Elem.h"
#include "p2List.h"
#include "p2SString.h"

#define MAX_BUTTON_NUM 10

class GuiButton;
class GuiLabel;
class j1UI_Elem;

class GuiWindow : public j1UI_Elem
{
public:

	GuiWindow(Alignment alignment = NO_ALIGN);

	virtual ~GuiWindow();
	bool Start();
	bool CleanUp();
	bool Update(float dt);

	p2SString title;
	uint num_buttons;

	void AddWindowElement(j1UI_Elem* elem);
	void StateChanging(ButtonState state);
	void PushButtonName(p2SString txt); //Sets the name of the buttons in order

private:
	p2List<j1UI_Elem*> win_elems;
	p2List<GuiButton*> win_buttons;
	p2List<p2SString> win_buttons_txt;
	GuiLabel* win_text = nullptr;

	void Drag();
	void StartDrag();
	void EndDrag();
	void PutWindowButtons();
	void DragWindowElements(iPoint displace);
};

#endif // __GUIWINDOW_H__