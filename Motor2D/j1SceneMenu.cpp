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

#define DEFAULT_BAR_LENGHT 5

j1SceneMenu::j1SceneMenu() : j1Module()
{
	name.create("menu");

	for (int i = 0; i < 10; i++) {
		winsoundtile[i] = nullptr;
	}
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
	if (!settingwindowcreated) {
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
	}
	else {
	
		if(elem == window || elem == winquit || elem == winsoundmin || elem == winsoundplus){
			switch (evnt) {
			case ButtonEvent::LEFT_CLICK:
				elem->StateChanging(PRESSED_L);
				if (elem == winquit) DestroySettingWindow();
				if (elem == winsoundmin) ShiftVolumeLeft();
				if (elem == winsoundplus) ShiftVolumeRight();

				break;
			case ButtonEvent::LEFT_CLICK_UP:
				elem->StateChanging(UP_L);
				break;
			case ButtonEvent::MOUSE_INSIDE:
				elem->StateChanging(HOVER);
				break;
			case ButtonEvent::MOUSE_OUTSIDE:
				elem->StateChanging(IDLE);
				break;
			}		
		}
		
	
	
	}
	return true;
}

void j1SceneMenu::CreateSettingWindow() {

	settings->StateChanging(IDLE);
	
	window = App->gui->AddWindow(ALIGN_CENTERED, 0, nullptr, {0,20},this);
	window->tex = window->LoadTexture("gui/Settings/window.png");
	window->rect = { 0,0, 741, 768 };

	winquit = App->gui->AddButton(ALIGN_CENTERED,  nullptr, { -320,120 }, this);
	winquit->SetButtonTex("gui/Settings/QuitButt.png", "gui/Settings/QuitButtPressed.png");	
	winquit->rect = { 0,0, 61, 72 };
	window->AddWindowElement(winquit);

	winsetticon = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/SettingsIcon.png", { 0,0,68,82 }, { 0,100 });
	window->AddWindowElement(winsetticon);



	int winsoundbar_y = 370;
	winsoundtxt = App->gui->AddText(ALIGN_CENTERED, "SOUND", { 0,winsoundbar_y });
	window->AddWindowElement(winsoundtxt);

	winsoundmin = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -310,winsoundbar_y + 50 }, this);
	winsoundmin->SetButtonTex("gui/Settings/MinusButt.png", "gui/Settings/MinusButtPressed.png");
	winsoundmin->rect = { 0,0, 69, 80 };
	window->AddWindowElement(winsoundmin);

	winsoundplus = App->gui->AddButton(ALIGN_CENTERED, nullptr, {310,winsoundbar_y + 50 }, this);
	winsoundplus->SetButtonTex("gui/Settings/PlusButt.png", "gui/Settings/PlusButtPressed.png");
	winsoundplus->rect = { 0,0, 72, 84 };
	window->AddWindowElement(winsoundplus);	

	winsoundbar = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/back_bar.png", { 0,0,543,88 }, { 0,winsoundbar_y+50 });
	window->AddWindowElement(winsoundbar);

	winsoundtile[0] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Leftbar.png", { 0,0,48,75 }, { -235,winsoundbar_y+ 57 });
	window->AddWindowElement(winsoundtile[0]);

	for (int i = 1; i < 9; i++) {
		winsoundtile[i] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Midbar.png", { 0,0,45,75 }, { -235+ 52*i,winsoundbar_y + 57 });
		if (i >= DEFAULT_BAR_LENGHT) { winsoundtile[i]->draw = false; }
		window->AddWindowElement(winsoundtile[i]);
	}
	winsoundtile[9] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Rightbar.png", { 0,0,48,75 }, { 235,winsoundbar_y + 57 });
	winsoundtile[9]->draw = false;
	window->AddWindowElement(winsoundtile[9]);



	settingwindowcreated = true;
}

void j1SceneMenu::DestroySettingWindow() {
	window->to_delete = true;
	winquit->to_delete = true;
	winsoundplus->to_delete = true;
	winsoundmin->to_delete = true;
	winsoundbar->to_delete = true;
	winsetticon->to_delete = true;
	winsoundtxt->to_delete = true;

	for (int i = 0; i < 10; i++) {
		if(winsoundtile[i]!=nullptr)
		winsoundtile[i]->to_delete = true;
	}

	settingwindowcreated = false;
}

void j1SceneMenu::ShiftVolumeLeft() {

	for (int i = 0; i < 10; i++) {
		if (winsoundtile[i]->draw == false) {
			winsoundtile[i - 1]->draw = false;
			return;
		}
	}
	winsoundtile[9]->draw = false;
}

void j1SceneMenu::ShiftVolumeRight() {

	for (int i = 0; i < 10; i++) {
		if (winsoundtile[i]->draw == false) {
			winsoundtile[i]->draw = true;
			return;
		}
	}
}