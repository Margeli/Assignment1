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
		background_rect = { 0,0,1920, 1080 };

		//---Buttons
		play = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 0,200 }, this);
		play->SetButtonTex( "gui/Buttons/PlayButton.png", "gui/Buttons/PlayButtonHover.png");

		cont = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 0,310 }, this);
		cont->SetButtonTex("gui/Buttons/ContinueButton.png", "gui/Buttons/ContinueButtonHover.png", "gui/Buttons/ContinueButtonPressed.png");

		settings = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 0,420 }, this);
		settings->SetButtonTex("gui/Buttons/SettingsButton.png", "gui/Buttons/SettingsButtonHover.png");

		credits = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 0,530 }, this);
		credits->SetButtonTex("gui/Buttons/CreditsButton.png", "gui/Buttons/CreditsButtonHover.png");

		exit = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 0,640 }, this);
		exit->SetButtonTex("gui/Buttons/ExitButton.png", "gui/Buttons/ExitButtonHover.png");
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
	App->tex->CleanUp();
	if (exit != NULL)
		exit->CleanUp();
	if (credits != NULL)
		credits->CleanUp();
	if (settings != NULL)
		settings->CleanUp();
	if (cont != NULL)
		cont->CleanUp();
	if (play != NULL)
		play->CleanUp();
	if (window != NULL)
		window->CleanUp();


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


bool j1SceneMenu::OnEventChange(j1UI_Elem* elem, ButtonEvent event)  {
	/*if (elem == exit) {
	if (event == ButtonEvent::RIGHT_CLICK) {
	web->ChangeText("RIGHT CLICK");
	}
	if (event == ButtonEvent::MOUSE_INSIDE || event == ButtonEvent::MOUSE_OUTSIDE) {
	web->ChangeText("WoWps.org TBC");
	}
	if (event == ButtonEvent::LEFT_CLICK) {
	web->ChangeText("LEFT CLICK");
	}
	}*/
	switch (event) {
	
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
	
	if (elem == exit) {
		if (event == ButtonEvent::LEFT_CLICK) {
			return false;
		}
	}
	if (elem == play) {
		if (event == ButtonEvent::LEFT_CLICK) {
			SceneChange();
		}
	}


	return true;
}