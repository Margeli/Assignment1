#include "j1SceneMenu.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1EntityManager.h"
#include "j1Player.h"

j1SceneMenu::j1SceneMenu() : j1Module()
{
	name.create("menu");
}

j1SceneMenu::~j1SceneMenu()
{}

bool j1SceneMenu::Awake(pugi::xml_node&)
{
	LOG("Loading Menu Scene");
	bool ret = true;
	if (App->menu->active == false) { LOG("Unable to load ."); ret = false; }

	return ret;
}

bool j1SceneMenu::Start()
{
	if (App->scene1->active == true) { active = false; }
	else if (App->scene2->active == true) { active = false; }
	if (active) {
		background = App->tex->Load("textures/menu_back.png");
		background_rect = { 0,0,1024, 768 };

		//---Buttons
		play = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,200 }, this);
		play->SetButtonTex( "gui/Buttons/PlayButton.png", "gui/Buttons/PlayButtonHover.png");
		menu_elems.add(play);

		cont = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,300 }, this);
		cont->SetButtonTex("gui/Buttons/ContinueButton.png", "gui/Buttons/ContinueButtonHover.png", "gui/Buttons/ContinueButtonPressed.png");
		cont->active = false;
		menu_elems.add(cont);

		settings = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,400 }, this);
		settings->SetButtonTex("gui/Buttons/SettingsButton.png", "gui/Buttons/SettingsButtonHover.png");
		menu_elems.add(settings);

		credits = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,500 }, this);
		credits->SetButtonTex("gui/Buttons/CreditsButton.png", "gui/Buttons/CreditsButtonHover.png");
		menu_elems.add(credits);

		exit = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,600 }, this);
		exit->SetButtonTex("gui/Buttons/ExitButton.png", "gui/Buttons/ExitButtonHover.png");
		menu_elems.add(exit);
	}
	return true;
}

bool j1SceneMenu::PreUpdate()
{
	return true;
}

bool j1SceneMenu::Update(float dt)
{
	
	App->render->Blit(background, 0, 0,&background_rect);
	return true;
}

bool j1SceneMenu::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) { ret = false; }

	return ret;
}

bool j1SceneMenu::CleanUp()
{
	LOG("Unloading  menu.");

	for (p2List_item<j1UI_Elem*>* elem = menu_elems.end; elem!=nullptr; elem = elem->prev) {		
		elem->data->CleanUp();
		menu_elems.del(elem);
			
	}


	return true;
}

void j1SceneMenu::SceneChange()
{
	App->scene1->active = true;
	App->menu->active = false;

	CleanUp();
	
	
	App->scene1->Start();
	App->entities->CreatePlayer();
	App->entities->player->Start();
	App->render->SetCameraInitialPos();
}


bool j1SceneMenu::OnEventChange(j1UI_Elem* elem, ButtonEvent evnt)  
{
	if (elem == settings)
	{
		if (evnt == ButtonEvent::LEFT_CLICK) {
			if (!settingwindowcreated)
				CreateSettingWindow();
		}
	}
	if (elem == exit) {
		if (evnt == ButtonEvent::LEFT_CLICK) {
			return false;
		}
	}
	if (elem == play) {
		if (evnt == ButtonEvent::LEFT_CLICK) {
			SceneChange();
		}
	}

	switch (evnt) {
	
	case ButtonEvent::MOUSE_INSIDE:
		elem->StateChanging(HOVER);
		break;
	case ButtonEvent::MOUSE_OUTSIDE:
		elem->StateChanging(IDLE);
		break;
	case ButtonEvent::RIGHT_CLICK:
		elem->StateChanging(PRESSED_R);
		break;
	case ButtonEvent::LEFT_CLICK:
		elem->StateChanging(PRESSED_L);
		break;
	case ButtonEvent::LEFT_CLICK_UP:
		elem->StateChanging(UP_L);
		break;
	case ButtonEvent::RIGHT_CLICK_UP:
		elem->StateChanging(UP_R);
		break;
	}
	
	return true;
}

void j1SceneMenu::CreateSettingWindow() {

	window = App->gui->AddWindow(ALIGN_CENTERED, 0, nullptr, {0,20},this);
	window->tex = window->LoadTexture("gui/Settings/window.png");
	window->rect = { 0,0, 741, 768 };
	settingwindowcreated = true;
}