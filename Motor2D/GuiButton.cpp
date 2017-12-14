#include "GuiButton.h"
#include "j1Gui.h"
#include "j1UI_Elem.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"

GuiButton::GuiButton(Alignment alignment) : j1UI_Elem(UIType::BUTTON, Alignment::NO_ALIGN)
{
	align = alignment;
}

GuiButton::~GuiButton()
{}

bool GuiButton::Start() 
{
	
	rect = { 0,0, 339, 63 };
	AddButtonText();
	tex = up;

	return true;
}

bool GuiButton::CleanUp() 
{
	//App->tex->UnLoad(tex);

	to_delete = true;
	if(tex!=nullptr)
	App->tex->UnLoad(tex);
	App->tex->UnLoad(up);
	App->tex->UnLoad(down);
	App->tex->UnLoad(press);
	buttontext->CleanUp();


	return true;
}
bool GuiButton::PreUpdate() {

	if (to_delete)
	{
		buttontext->to_delete= true;
	}
	return true;
}

bool GuiButton::Update(float dt) 
{
	if (active == false) {
		
		tex = press;//texture pressed and disable its the same
	}
	UpdateAlignment();
	App->render->Blit(tex, position.x + displacement.x, position.y + displacement.y, &rect);
	return true;
}

void GuiButton::AddButtonText() 
{
	int width, heigh;
	App->font->CalcSize(text.GetString(), width, heigh);

	switch (align) 
	{
	case NO_ALIGN:
		buttontext = App->gui->AddText(NO_ALIGN, text, { displacement.x , displacement.y + 2 }, MORPHEUS, { 255, 255,0,255 });
		break;
	case ALIGN_CENTERED:
		buttontext = App->gui->AddText(align, text, { displacement.x , displacement.y + 2 }, MORPHEUS, { 255, 255,0,255 });
		break;
	case ALIGN_RIGHT:
		buttontext = App->gui->AddText(align, text, { displacement.x + width/ 2 - rect.w /2 , displacement.y + 2 }, MORPHEUS, { 255, 255,0,255 });
		break;
	case ALIGN_LEFT:
		buttontext = App->gui->AddText(align, text, { displacement.x + (rect.w -width)/2 , displacement.y + 2 }, MORPHEUS, { 255, 255,0,255 });
		break;
	}
}

void GuiButton::StateChanging(ButtonState status) 
{
	switch(status)
	{
	case IDLE:
		tex = up;
		state = status;
		break;
	case HOVER:
		tex = down;
		state = status;
		break;
	case PRESSED_L:
		if (press!= NULL)
		tex = press;
		state = status;
		break;
	case UP_L:
		tex = up;
		break;
	}
}

void GuiButton::ChangeButtonTxt(p2SString txt) 
{
	buttontext->ChangeText(txt);
}

void GuiButton::Drag(iPoint displace) 
{
	displacement.x += displace.x;
	displacement.y += displace.y;
	DragButtonElements(displace);
}

void GuiButton::DragButtonElements(iPoint displace)
{
	buttontext->Drag(displace);
}

void GuiButton::SetButtonTex(p2SString path_idle, p2SString path_hover, p2SString path_pressed) {

	up = LoadTexture(path_idle);
	down = LoadTexture(path_hover);
	press = LoadTexture(path_pressed);
	tex = up;
	
}