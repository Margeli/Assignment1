#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1UI_Elem.h"
#include "GuiImage.h"
#include "GuiLabel.h"
#include "GuiWindow.h"
#include "GuiInput.h"
#include "GuiCheck.h"
#include "GuiButton.h"

#define CURSOR_WIDTH 2

class j1UI_Elem;

class j1Gui : public j1Module
{
public:

	j1Gui();

	virtual ~j1Gui();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

	SDL_Texture* GetAtlas() const;

	GuiImage* AddImage(Alignment align, char* path , SDL_Rect texture = { 0,0,0,0 }, iPoint displacement = { 0,0 },j1Module* listener = nullptr);
	GuiButton* AddButton(Alignment align, p2SString text, iPoint displacement = { 0,0 }, j1Module* listener = nullptr);
	GuiCheck* AddCheck(Alignment align, p2SString text, iPoint displacement = { 0,0 }, j1Module* listener = nullptr);
	GuiLabel* AddText(Alignment align, p2SString text, iPoint displacement = { 0,0 }, FontType= ZEALAND, SDL_Color color = { 255,255,255,255 }, j1Module* listener = nullptr);
	GuiWindow* AddWindow(Alignment align, uint num_buttons, p2SString title, iPoint displacement = { 0,0 }, j1Module* listener = nullptr);

private:
	j1UI_Elem* AddElement(UIType type, Alignment aligment = Alignment::NO_ALIGN);
	void UpdateElemEvent() const;
	bool ManageElemEvent();
	void DestroyElement(j1UI_Elem* elem);

	p2List<j1UI_Elem*> elements;
	SDL_Texture* atlas;
	p2SString atlas_file_name;

	bool debug= false;
};

#endif // __j1GUI_H__