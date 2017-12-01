#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1UI_Elem.h"



j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	p2List_item<j1UI_Elem*>* elem;

	for (elem = elements.start; elem != NULL; elem = elem->next)
	{
		elem->data->Start();
	}

	return true;
}

bool j1Gui::PreUpdate() {

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {	debug = !debug;}	

	UpdateElemEvent();
	ManageElemEvent();
	

	return true;
}

// Update all guis
bool j1Gui::Update(float dt)
{
	p2List_item<j1UI_Elem*>* elem;

	for (elem = elements.start; elem != NULL; elem = elem->next)
	{
		elem->data->Update(dt);
		if (debug)
		{
			elem->data->DebugDraw();
		}
	}
	

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	App->tex->UnLoad(atlas);
	LOG("Freeing GUI");
	p2List_item<j1UI_Elem*>* elem;

	for (elem = elements.start; elem != NULL; elem = elem->next)
	{
		elem->data->CleanUp();
		DestroyElement(elem->data);
	}
	elements.clear();
	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::UpdateElemEvent() const {

	iPoint pos;
	App->input->GetMousePosition(pos.x, pos.y);

	p2List_item<j1UI_Elem*>* elem;
	for (elem = elements.start; elem != NULL; elem = elem->next)
	{
		iPoint elem_pos = { elem->data->position.x + elem->data->displacement.x,elem->data->position.y + elem->data->displacement.y };
		if ((pos.x > elem_pos.x && pos.x < elem_pos.x + elem->data->rect.w) && (pos.y > elem_pos.y && pos.y < elem_pos.y + elem->data->rect.h)) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
				elem->data->event = ButtonEvent::RIGHT_CLICK;
				
			}	
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
				elem->data->event = ButtonEvent::RIGHT_CLICK_UP;
				break;
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				elem->data->event = ButtonEvent::LEFT_CLICK;
				
			}	
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				elem->data->event = ButtonEvent::LEFT_CLICK_UP;
				break;

			}
			else {
				elem->data->event = ButtonEvent::MOUSE_INSIDE;
			}
		}
		else
		{
			elem->data->event = ButtonEvent::MOUSE_OUTSIDE;
		}
	}

}

void j1Gui::ManageElemEvent() {
	p2List_item<j1UI_Elem*>* elem;
	for (elem = elements.start; elem != NULL; elem = elem->next)
	{
		if ((elem->data->event != elem->data->previous_event)
			&& elem->data->listener != nullptr) {
			elem->data->listener->OnEventChange(elem->data, elem->data->event);
			elem->data->previous_event = elem->data->event;
		}
	}
}

// class Gui ---------------------------------------------------

void j1Gui::DestroyElement(j1UI_Elem* elem) {
		
	int num = elements.find(elem);
	RELEASE(elements.At(num)->data);
	elements.del(elements.At(num));
}

j1UI_Elem* j1Gui::AddElement(UIType type, Alignment alignment) {

	j1UI_Elem* ret = nullptr;

	switch (type)
	{
	case CHECK:
		ret = new GuiCheck(alignment);
		elements.add(ret);
		break;
	case IMAGE:
		ret = new GuiImage(alignment);
		elements.add(ret);
		break;
	case BUTTON:
		ret = new GuiButton(alignment);
		elements.add(ret);
		break;
	case INPUTBOX:
		ret = new GuiInput(alignment);
		elements.add(ret);
		break;
	case LABEL:
		ret = new GuiLabel(alignment);
		elements.add(ret);
		break;	
	case WINDOW:
		ret = new GuiWindow(alignment);
		elements.add(ret);
		break;
	}
	return ret;
}

GuiImage* j1Gui::AddImage(Alignment align, char* path, SDL_Rect texture, iPoint displacement, j1Module* listener) {
	GuiImage* image = (GuiImage*)App->gui->AddElement(IMAGE, align);
	image->rect = texture;
	image->displacement = displacement;
	image->listener = listener;
	if (path != nullptr) {	
		p2SString pat = path;
		image->tex = image->LoadTexture(pat.GetString());
	}
	return image;
}

GuiButton* j1Gui::AddButton(Alignment align, p2SString text, iPoint displacement, j1Module* listener) {
	GuiButton* button = (GuiButton*)App->gui->AddElement(BUTTON, align);
	button->displacement = displacement;
	button->text = text;	
	button->listener = listener;
	return button;

}

GuiCheck* j1Gui::AddCheck(Alignment align, p2SString text, iPoint displacement, j1Module* listener) {
	GuiCheck* check = (GuiCheck*)App->gui->AddElement(CHECK, align);
	check->displacement = displacement;
	check->text = text;
	check->listener = listener;
	return check;

}

GuiLabel* j1Gui::AddText(Alignment align, p2SString text, iPoint displacement,FontType font, SDL_Color color, j1Module* listener) {
	GuiLabel* tex = (GuiLabel*)App->gui->AddElement(LABEL, align);
	tex->CreateText(text, color, font);
	tex->displacement = displacement;
	tex->listener = listener;
	return tex;

}

GuiWindow* j1Gui::AddWindow(Alignment align, uint num_buttons, p2SString title, iPoint displacement, j1Module* listener) {
	GuiWindow* window = (GuiWindow*)App->gui->AddElement(WINDOW, align);
	window->displacement = displacement;
	window->num_buttons = num_buttons;
	window->title = title;
	window->listener = listener;
	return window;

}