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
#include "j1Pathfinding.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"

#define DEFAULT_BAR_LENGHT 5
#define GITHUB_URL "www.github.com/Margeli/Assignment2"

j1SceneMenu::j1SceneMenu() : j1Module()
{
	name.create("menu");
	for (int i = 0; i < 10; i++) { winsoundtile[i] = nullptr; winfxtile[i] = nullptr; }
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
	current_volume = fx_volume = MIX_MAX_VOLUME/2;

	if (App->scene1->active == true) { active = false; }
	else if (App->scene2->active == true) { active = false; }

	button_sound = App->audio->LoadFx("audio/fx/button_sound.wav");
	door_sound = App->audio->LoadFx("audio/fx/door_sound.wav");

	if (active) 
	{
		background = App->tex->Load("textures/menu_back.png");
		background_rect = { 0,0,1024, 768 };

		//---Buttons
		play = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,200 }, this);
		play->SetButtonTex( "gui/Buttons/PlayButton.png", "gui/Buttons/PlayButtonHover.png");
		play->rect= { 0,0,195, 63 };
		menu_elems.add(play);

		cont = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,300 }, this);
		cont->SetButtonTex("gui/Buttons/ContinueButton.png", "gui/Buttons/ContinueButtonHover.png", "gui/Buttons/ContinueButtonPressed.png");
		if (!App->savefilefound)cont->active = false;
		menu_elems.add(cont);

		settings = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,400 }, this);
		settings->SetButtonTex("gui/Buttons/SettingsButton.png", "gui/Buttons/SettingsButtonHover.png");
		menu_elems.add(settings);

		credits = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,500 }, this);
		credits->SetButtonTex("gui/Buttons/CreditsButton.png", "gui/Buttons/CreditsButtonHover.png");
		credits->rect = { 0,0,280, 63 };
		menu_elems.add(credits);

		exit = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,600 }, this);
		exit->SetButtonTex("gui/Buttons/ExitButton.png", "gui/Buttons/ExitButtonHover.png");
		exit->rect = { 0,0,180, 63 };
		menu_elems.add(exit);

		App->audio->PlayMusic("audio/music/music_sadpiano.ogg");

	}
	return true;
}

bool j1SceneMenu::PreUpdate()
{
	return true;
}

bool j1SceneMenu::Update(float dt)
{
	if (active) 
	{
		App->render->Blit(background, 0, 0, &background_rect);
		if (toChangeScene && !App->fade->IsFading()) { SceneChange(); }
	}
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
	App->tex->UnLoad(background);
	background = nullptr;
	for (p2List_item<j1UI_Elem*>* elem = menu_elems.end; elem != nullptr; elem = elem->prev) { elem->data->CleanUp(); elem->data = nullptr; }
	menu_elems.clear();
	if (settingwindowcreated || creditswindowcreated) { DestroyWindow(); }

	return true;
}

void j1SceneMenu::SceneChange()
{		
	this->active = false;
	App->scene1->active = true;
	CleanUp();
	
	App->scene1->Start();
	App->entities->active = true;
	App->entities->CreatePlayer();
	App->entities->Start();	
	App->collis->Start();
	App->pathfind->Start();
	App->render->SetCameraInitialPos();
	App->scene1->Update(0);

	toChangeScene = false;
}

bool j1SceneMenu::OnEventChange(j1UI_Elem* elem, ButtonEvent evnt)  
{
	if (!settingwindowcreated && !creditswindowcreated) 
	{
		if (elem == settings)		//SETTINGS
		{
			if (evnt == ButtonEvent::LEFT_CLICK) 
			{
				if (!settingwindowcreated)  App->audio->PlayFx(button_sound); CreateSettingWindow();
			}
		}
		if (elem == credits)
		{
			if (evnt == ButtonEvent::LEFT_CLICK) { if (!creditswindowcreated)  App->audio->PlayFx(button_sound); CreateCreditsWindow(); }
		}

		if (elem == exit) { if (evnt == ButtonEvent::LEFT_CLICK) { App->audio->PlayFx(door_sound);  return false; } }		//EXIT

		if (elem == play)	//PLAY	
		{	
			if (evnt == ButtonEvent::LEFT_CLICK) 
			{
				App->audio->PlayFx(button_sound);
				App->fade->FadeToBlack(this, App->scene1, 0.5f);
				toChangeScene = true;
			}
		}

		if (elem == cont&& cont->active) {
			if (evnt == ButtonEvent::LEFT_CLICK){		
				LoadGame();
			}		
		}
		switch (evnt) 
		{
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
	else if(settingwindowcreated)		//SETTINGS
	{
		if(elem == window || elem == winquit || elem == winsoundmin || elem == winsoundplus|| elem== winfxmin || elem == winfxplus || elem == fullscreen)
		{
			switch (evnt) 
			{
			case ButtonEvent::LEFT_CLICK:
				if (elem == winquit) { App->audio->PlayFx(button_sound);  DestroyWindow(); }

				if (elem == fullscreen) 
				{
					App->audio->PlayFx(button_sound);
					if (fullscreenbool == false) 
					{
						fullscreenbool = true;
						winfullscr = SDL_WINDOW_FULLSCREEN;
						SDL_SetWindowFullscreen(App->win->window, winfullscr);
					}
					else 
					{
						fullscreenbool = false; 
						winfullscr = SDL_WINDOW_MINIMIZED;
						SDL_SetWindowFullscreen(App->win->window, winfullscr);
					}
					window->can_move = false;
				}
				if (elem == winsoundmin) 
				{ 	
					App->audio->PlayFx(button_sound);
					ShiftVolumeBarLeft(); 
					window->can_move = false;
					current_volume -= 12.8f;
					if (current_volume <= 0) { current_volume = 0; }
					Mix_VolumeMusic(current_volume);					
				}

				if (elem == winsoundplus)
				{
					App->audio->PlayFx(button_sound);
					ShiftVolumeBarRight(); 
					window->can_move = false; 
					current_volume += 12.8f;
					if (current_volume > 128) { current_volume = MIX_MAX_VOLUME; }
					Mix_VolumeMusic(current_volume );
				}

				if (elem == winfxplus)
				{
					App->audio->PlayFx(button_sound);
					ShiftFXBarRight(); 
					window->can_move = false; 
					fx_volume += 12.8f;
					sample->volume = fx_volume;
					Mix_VolumeChunk(sample, fx_volume);
				}

				if (elem == winfxmin) 
				{ 
					App->audio->PlayFx(button_sound);
					ShiftFXBarLeft(); 
					window->can_move = false; 
					fx_volume -= 12.8f;
					sample->volume = fx_volume;
					Mix_VolumeChunk(sample, fx_volume);
				}

				elem->StateChanging(PRESSED_L);
				break;

			case ButtonEvent::LEFT_CLICK_UP:
				elem->StateChanging(UP_L);
				break;

			case ButtonEvent::MOUSE_INSIDE:
				elem->StateChanging(HOVER);
				break;

			case ButtonEvent::MOUSE_OUTSIDE:
				if (elem == winsoundmin|| elem == winsoundplus || elem == winfxmin|| elem == winfxplus|| elem == fullscreen) { window->can_move = true; }
				
				elem->StateChanging(IDLE);
				break;
			}		
		}
	}
	else if (creditswindowcreated)		//CREDITS
	{
		if (elem == window || elem == winquit || elem == link)
		{
			switch (evnt)
			{
			case ButtonEvent::LEFT_CLICK:
				if (elem == winquit) { App->audio->PlayFx(button_sound);  DestroyWindow(); }
				if (elem == link) 
				{
					ShellExecute(NULL, "open", GITHUB_URL, NULL, NULL, SW_SHOWNORMAL);
				}
				elem->StateChanging(PRESSED_L);
				break;

			case ButtonEvent::LEFT_CLICK_UP:
				elem->StateChanging(UP_L);
				break;

			case ButtonEvent::MOUSE_INSIDE:
				elem->StateChanging(HOVER);
				break;

			case ButtonEvent::MOUSE_OUTSIDE:
				if (elem == winsoundmin || elem == winsoundplus || elem == winfxmin || elem == winfxplus || elem == fullscreen) { window->can_move = true; }

				elem->StateChanging(IDLE);
				break;
			}
		}
	}

	return true;
}

void j1SceneMenu::CreateSettingWindow() 
{
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

	fullscreen = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -315, 215 }, this);
	fullscreen->SetButtonTex("gui/Settings/fullscreen.png", "gui/Settings/fullscreenII.png");
	fullscreen->rect = { 0,0, 50, 50 };
	window->AddWindowElement(fullscreen);

	settingsback = App->gui->AddText(ALIGN_CENTERED, "BACK", { -230, 135 });
	window->AddWindowElement(settingsback);

	fullscreenlabel = App->gui->AddText(ALIGN_CENTERED, "FULLSCREEN", { -170, 215 });
	window->AddWindowElement(fullscreenlabel);

	int winsoundbar_y = 300;
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
		winsoundtile[i] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Midbar.png", { 0,0,45,75 }, { -235 + 52 * i,winsoundbar_y + 57 });
		if (i >= DEFAULT_BAR_LENGHT) { winsoundtile[i]->draw = false; }
		window->AddWindowElement(winsoundtile[i]);
	}
	winsoundtile[9] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Rightbar.png", { 0,0,48,75 }, { 235,winsoundbar_y + 57 });
	winsoundtile[9]->draw = false;
	window->AddWindowElement(winsoundtile[9]);

	int winfxbar_y = 490;
	winfxtxt = App->gui->AddText(ALIGN_CENTERED, "FX", { 0,winfxbar_y });
	window->AddWindowElement(winfxtxt);

	winfxmin = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -310,winfxbar_y + 50 }, this);
	winfxmin->SetButtonTex("gui/Settings/MinusButt.png", "gui/Settings/MinusButtPressed.png");
	winfxmin->rect = { 0,0, 69, 80 };
	window->AddWindowElement(winfxmin);

	winfxplus = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 310,winfxbar_y + 50 }, this);
	winfxplus->SetButtonTex("gui/Settings/PlusButt.png", "gui/Settings/PlusButtPressed.png");
	winfxplus->rect = { 0,0, 72, 84 };
	window->AddWindowElement(winfxplus);

	winfxbar = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/back_bar.png", { 0,0,543,88 }, { 0,winfxbar_y + 50 });
	window->AddWindowElement(winfxbar);

	winfxtile[0] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Leftbar.png", { 0,0,48,75 }, { -235,winfxbar_y + 57 });
	window->AddWindowElement(winfxtile[0]);

	for (int i = 1; i < 9; i++) {
		winfxtile[i] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Midbar.png", { 0,0,45,75 }, { -235 + 52 * i,winfxbar_y + 57 });
		if (i >= DEFAULT_BAR_LENGHT) { winfxtile[i]->draw = false; }
		window->AddWindowElement(winfxtile[i]);
	}
	winfxtile[9] = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/Rightbar.png", { 0,0,48,75 }, { 235,winfxbar_y + 57 });
	winfxtile[9]->draw = false;
	window->AddWindowElement(winfxtile[9]);
	
	settingwindowcreated = true;
}

void j1SceneMenu::CreateCreditsWindow()
{
	credits->StateChanging(IDLE);

	window = App->gui->AddWindow(ALIGN_CENTERED, 0, nullptr, { 0,20 }, this);
	window->tex = window->LoadTexture("gui/Settings/window.png");
	window->rect = { 0,0, 741, 768 };

	winquit = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -320,120 }, this);
	winquit->SetButtonTex("gui/Settings/QuitButt.png", "gui/Settings/QuitButtPressed.png");
	winquit->rect = { 0,0, 61, 72 };
	window->AddWindowElement(winquit);

	license = App->gui->AddImage(ALIGN_CENTERED, "gui/Settings/license_txt.png", { 0,0, 741, 768 }, { 0,20 });
	window->AddWindowElement(license);

	link = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 220,170 }, this);
	link->SetButtonTex("gui/Settings/github_icon.png", "gui/Settings/github_icon.png");
	link->rect = {0,0, 128, 128 };
	window->AddWindowElement(link);

	settingsback = App->gui->AddText(ALIGN_CENTERED, "BACK", { -230, 135 });
	window->AddWindowElement(settingsback);

	creditswindowcreated = true;
}

void j1SceneMenu::DestroyWindow() 
{
	window->CleanUp();
	creditswindowcreated = false;
	settingwindowcreated = false;
}

void j1SceneMenu::ShiftVolumeBarLeft() 
{
	for (int i = 0; i < 10; i++) 
	{
		if (winsoundtile[i]->draw == false) 
		{
			winsoundtile[i - 1]->draw = false;
			return;
		}
	}
	winsoundtile[9]->draw = false;
}

void j1SceneMenu::ShiftVolumeBarRight() 
{
	for (int i = 0; i < 10; i++) 
	{
		if (winsoundtile[i]->draw == false) 
		{
			winsoundtile[i]->draw = true;
			return;
		}
	}
}
void j1SceneMenu::ShiftFXBarLeft()
{
	for (int i = 0; i < 10; i++)
	{
		if (winfxtile[i]->draw == false)
		{
			winfxtile[i - 1]->draw = false;
			return;
		}
	}
	winfxtile[9]->draw = false;
}

void j1SceneMenu::ShiftFXBarRight()
{
	for (int i = 0; i < 10; i++)
	{
		if (winfxtile[i]->draw == false)
		{
			winfxtile[i]->draw = true;
			return;
		}
	}
}
void j1SceneMenu::LoadGame() 
{
	this->active = false;
	App->scene1->active = true;
	CleanUp();

	App->scene1->Start();
	App->entities->active = true;
	App->entities->CreatePlayer();
	App->entities->Start();
	App->collis->Start();
	App->pathfind->Start();
	App->render->SetCameraInitialPos();
	App->LoadGame();
}