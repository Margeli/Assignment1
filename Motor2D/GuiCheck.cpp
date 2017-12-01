#include "GuiCheck.h"
#include "j1Gui.h"
#include "j1UI_Elem.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"

GuiCheck::GuiCheck(Alignment alignment) : j1UI_Elem(UIType::CHECK, Alignment::NO_ALIGN) 
{
	align = alignment;
}

GuiCheck::~GuiCheck()
{}

bool GuiCheck::Start() 
{
	tex = LoadTexture("");// gui / wow ui / BUTTONS / UI - CheckBox - Down.png");
	check = LoadTexture("");//gui/wow ui/BUTTONS/UI-CheckBox-Check.png");
	rect = { 0,0,32,32 };
	AddCheckText();

	return true;
}

bool GuiCheck::CleanUp() 
{
	//App->tex->UnLoad(tex);
	return true;
}

bool GuiCheck::Update(float dt) 
{
	UpdateAlignment();
	App->render->Blit(tex, position.x + displacement.x, position.y + displacement.y, &rect);
	if (pressed) { App->render->Blit(check, position.x + displacement.x, position.y + displacement.y, &rect); }
	return true;
}

void GuiCheck::AddCheckText() const 
{
	App->gui->AddText(align, text, { displacement.x+ 30 , displacement.y + 5 }, MORPHEUS, { 255, 255,0,255 });
}

void GuiCheck::StateChanging(ButtonState status) 
{
	switch (status) 
	{
	case IDLE:
		state = status;
		break;
	case PRESSED_L:
		pressed = !pressed;
		state = status;
		break;
	}
}
