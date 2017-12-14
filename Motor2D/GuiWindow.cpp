#include "GuiWindow.h"
#include "j1Gui.h"
#include "j1UI_Elem.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Textures.h"

GuiWindow::GuiWindow(Alignment alignment) : j1UI_Elem(UIType::INPUTBOX, Alignment::NO_ALIGN) 
{
	align = alignment;
}

GuiWindow::~GuiWindow()
{}

bool GuiWindow::Start() 
{
	if (tex != nullptr) {
		tex = App->gui->GetAtlas();
		rect = { 30, 542, 422, 454 };
	}
	PutWindowButtons();

	event = LEFT_CLICK; // to avoid dragging when the window is created
	previous_event = LEFT_CLICK;
	
	return true;
}

bool GuiWindow::CleanUp() 
{
	to_delete = true;
	App->tex->UnLoad(tex);

	if (win_text) { win_text->CleanUp(); }

	for (p2List_item<GuiButton*>* button = win_buttons.start; button; button = button->next) {
		button->data->CleanUp();
	}
	win_buttons.clear();
	for (p2List_item<j1UI_Elem*>* elem = win_elems.start; elem; elem = elem->next) {
		elem->data->CleanUp();
	}
	win_elems.clear();
	for (p2List_item<p2SString>* string = win_buttons_txt.start; string; string = string->next) {
		string->data.Clear();
	}
	win_buttons_txt.clear();
	//App->tex->UnLoad(tex);
	return true;
}

bool GuiWindow::Update(float dt) 
{
	if (moving && can_move) { Drag(); }
	UpdateAlignment();
	App->render->Blit(tex, position.x + displacement.x, position.y + displacement.y, &rect);
	return true;
}

void GuiWindow::StateChanging(ButtonState status) 
{
	switch (status) 
	{
	case IDLE:
		state = status;
		break;
	case HOVER:		
		state = status;
		break;
	case PRESSED_L:
		if (can_move)
			StartDrag();
		state = status;
		break;
	case UP_L:
		if (can_move)
			EndDrag();
		state = status;
		break;
	}
}

void GuiWindow::Drag() 
{
	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	displacement.x += ( mouse_position.x- click_pos.x);
	displacement.y += (mouse_position.y - click_pos.y);
	DragWindowElements({ mouse_position.x - click_pos.x , mouse_position.y - click_pos.y });
	click_pos = mouse_position;
}

void GuiWindow::StartDrag() 
{
	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	click_pos = mouse_position;

	moving = true;
}

void GuiWindow::EndDrag() 
{
	click_pos = {0,0};
	moving = false;
}

void GuiWindow::PutWindowButtons() 
{
	if (num_buttons > MAX_BUTTON_NUM) 
	{
		LOG("Exceded max num buttons");
		num_buttons = MAX_BUTTON_NUM;
	}
	iPoint localPos = { position.x + displacement.x, position.y + displacement.y };
	for (int i = 0; i < num_buttons; i++) 
	{
		p2SString butt_text;
		if (win_buttons_txt.count() <= i) 
		{
			butt_text = nullptr;
		}
		else 
		{
			butt_text = win_buttons_txt.At(i)->data;
		}
		GuiButton* butt;
		
		butt = App->gui->AddButton(align,butt_text , { localPos.x , localPos.y + 40*(i+1)}, listener);

		win_buttons.add(butt);
	}
}
void GuiWindow::DragWindowElements(iPoint displace) {
	
	for (p2List_item<GuiButton*>* button = win_buttons.start; button; button = button->next) {
		button->data->Drag(displace);	
	}
	for (p2List_item<j1UI_Elem*>* elem = win_elems.start; elem; elem = elem->next) {
		elem->data->Drag(displace);
	}
}

void GuiWindow::PushButtonName( p2SString txt) {

	if (win_buttons_txt.count() >= MAX_BUTTON_NUM) {
		LOG("Exceded max num buttons");
		return;
	}
	win_buttons_txt.add(txt);
}

void GuiWindow::AddWindowElement(j1UI_Elem* elem) { win_elems.add(elem); }