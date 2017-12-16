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
#include "j1Map.h"
#include "j1Gui.h"


#define GITHUB_URL "https://margeli.github.io/Assignment3/"
//should be a link to the webpage

j1SceneMenu::j1SceneMenu() : j1Module()
{
	name.create("menu");
	for (int i = 0; i < NUM_BAR_TILES; i++) { winsoundtile[i] = nullptr; winfxtile[i] = nullptr; }
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
	current_volume = (float)MIX_MAX_VOLUME/2;

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
		play->SetButtonTex("gui/Menu/PlayButton.png", "gui/Menu/PlayButtonHover.png", "gui/Menu/PlayButtonPressed.png");
		play->rect = { 0,0,195, 63 };
		menu_elems.add(play);

		cont = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,300 }, this);
		cont->SetButtonTex("gui/Menu/ContinueButton.png", "gui/Menu/ContinueButtonHover.png", "gui/Menu/ContinueButtonPressed.png");
		if (!App->savefilefound)cont->active = false;
		menu_elems.add(cont);

		settings = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,400 }, this);
		settings->SetButtonTex("gui/Menu/SettingsButton.png", "gui/Menu/SettingsButtonHover.png", "gui/Menu/SettingsButtonPressed.png");
		menu_elems.add(settings);

		credits = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,500 }, this);
		credits->SetButtonTex("gui/Menu/CreditsButton.png", "gui/Menu/CreditsButtonHover.png", "gui/Menu/CreditsButtonPressed.png");
		credits->rect = { 0,0,280, 63 };
		menu_elems.add(credits);

		exit = App->gui->AddButton(ALIGN_LEFT, nullptr, { 30,600 }, this);
		exit->SetButtonTex("gui/Menu/ExitButton.png", "gui/Menu/ExitButtonHover.png", "gui/Menu/ExitButtonPressed.png");
		exit->rect = { 0,0,180, 63 };
		menu_elems.add(exit);

		App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
	}
	return true;
}

bool j1SceneMenu::PreUpdate()
{
	App->input->GetMousePosition(mouse_x, mouse_y);
	iPoint pos = { mouse_x, mouse_y };

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
		if (elem == settings && evnt == LEFT_CLICK_UP) { elem->StateChanging(UP_L); }
	
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
					App->audio->SetMusicVolume(MIX_MAX_VOLUME / NUM_BAR_TILES *sound_bar_length);
				}

				if (elem == winsoundplus)
				{
					App->audio->PlayFx(button_sound);
					ShiftVolumeBarRight(); 
					window->can_move = false; 
					App->audio->SetMusicVolume(MIX_MAX_VOLUME / NUM_BAR_TILES *sound_bar_length);
				}

				if (elem == winfxplus)
				{
					App->audio->PlayFx(button_sound);
					ShiftFXBarRight(); 
					window->can_move = false; 
					App->audio->SetFxVolume(MIX_MAX_VOLUME/ NUM_BAR_TILES *fx_bar_length);
				}

				if (elem == winfxmin) 
				{ 
					App->audio->PlayFx(button_sound);
					ShiftFXBarLeft(); 
					window->can_move = false; 
					App->audio->SetFxVolume(MIX_MAX_VOLUME / NUM_BAR_TILES *fx_bar_length);
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
		if (elem == credits && evnt == LEFT_CLICK_UP) { elem->StateChanging(UP_L); }
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
	window->tex = window->LoadTexture("gui/Menu/window.png");
	window->rect = { 0,0, 741, 768 };

	winquit = App->gui->AddButton(ALIGN_CENTERED,  nullptr, { -320,120 }, this);
	winquit->SetButtonTex("gui/Menu/Settings/QuitButt.png", "gui/Menu/Settings/QuitButtPressed.png");	
	winquit->rect = { 0,0, 61, 72 };
	window->AddWindowElement(winquit);

	winsetticon = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/SettingsIcon.png", { 0,0,68,82 }, { 0,100 });
	window->AddWindowElement(winsetticon);

	fullscreen = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -315, 225 }, this);
	fullscreen->SetButtonTex("gui/Menu/Settings/fullscreen.png", "gui/Menu/Settings/fullscreenII.png");
	fullscreen->rect = { 0,0, 50, 50 };
	window->AddWindowElement(fullscreen);

	settingsback = App->gui->AddText(ALIGN_CENTERED, "BACK", { -230, 135 });
	window->AddWindowElement(settingsback);

	fullscreenlabel = App->gui->AddText(ALIGN_CENTERED, "FULLSCREEN", { -170, 225 });
	window->AddWindowElement(fullscreenlabel);

	int winsoundbar_y = 300;
	winsoundtxt = App->gui->AddText(ALIGN_CENTERED, "SOUND", { 0,winsoundbar_y });
	window->AddWindowElement(winsoundtxt);

	winsoundmin = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -310,winsoundbar_y + 50 }, this);
	winsoundmin->SetButtonTex("gui/Menu/Settings/MinusButt.png", "gui/Menu/Settings/MinusButtPressed.png");
	winsoundmin->rect = { 0,0, 69, 80 };
	window->AddWindowElement(winsoundmin);

	winsoundplus = App->gui->AddButton(ALIGN_CENTERED, nullptr, {310,winsoundbar_y + 50 }, this);
	winsoundplus->SetButtonTex("gui/Menu/Settings/PlusButt.png", "gui/Menu/Settings/PlusButtPressed.png");
	winsoundplus->rect = { 0,0, 72, 84 };
	window->AddWindowElement(winsoundplus);	

	winsoundbar = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/back_bar.png", { 0,0,543,88 }, { 0,winsoundbar_y+50 });
	window->AddWindowElement(winsoundbar);

	winsoundtile[0] = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/Leftbar.png", { 0,0,48,75 }, { -235,winsoundbar_y+ 57 });
	window->AddWindowElement(winsoundtile[0]);

	UpdateBarWithCurrentVolume(sound_bar_length);

	for (int i = 1; i < NUM_BAR_TILES-1; i++) {
		winsoundtile[i] = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/Midbar.png", { 0,0,45,75 }, { -235 + 52 * i,winsoundbar_y + 57 });
		if (i >= sound_bar_length) {
			winsoundtile[i]->draw = false; }
		window->AddWindowElement(winsoundtile[i]);
	}
	winsoundtile[NUM_BAR_TILES-1] = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/Rightbar.png", { 0,0,48,75 }, { 235,winsoundbar_y + 57 });
	winsoundtile[NUM_BAR_TILES-1]->draw = false;
	window->AddWindowElement(winsoundtile[NUM_BAR_TILES-1]);

	int winfxbar_y = 490;
	winfxtxt = App->gui->AddText(ALIGN_CENTERED, "FX", { 0,winfxbar_y });
	window->AddWindowElement(winfxtxt);

	winfxmin = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -310,winfxbar_y + 50 }, this);
	winfxmin->SetButtonTex("gui/Menu/Settings/MinusButt.png", "gui/Menu/Settings/MinusButtPressed.png");
	winfxmin->rect = { 0,0, 69, 80 };
	window->AddWindowElement(winfxmin);

	winfxplus = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 310,winfxbar_y + 50 }, this);
	winfxplus->SetButtonTex("gui/Menu/Settings/PlusButt.png", "gui/Menu/Settings/PlusButtPressed.png");
	winfxplus->rect = { 0,0, 72, 84 };
	window->AddWindowElement(winfxplus);

	winfxbar = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/back_bar.png", { 0,0,543,88 }, { 0,winfxbar_y + 50 });
	window->AddWindowElement(winfxbar);

	winfxtile[0] = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/Leftbar.png", { 0,0,48,75 }, { -235,winfxbar_y + 57 });
	window->AddWindowElement(winfxtile[0]);

	UpdateBarWithCurrentVolume(fx_bar_length);

	for (int i = 1; i <NUM_BAR_TILES-1; i++) {
		winfxtile[i] = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/Midbar.png", { 0,0,45,75 }, { -235 + 52 * i,winfxbar_y + 57 });
		if (i >= fx_bar_length) { winfxtile[i]->draw = false; }
		window->AddWindowElement(winfxtile[i]);
	}
	winfxtile[NUM_BAR_TILES - 1] = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Settings/Rightbar.png", { 0,0,48,75 }, { 235,winfxbar_y + 57 });
	winfxtile[NUM_BAR_TILES - 1]->draw = false;
	window->AddWindowElement(winfxtile[9]);

	settingwindowcreated = true;
}

void j1SceneMenu::CreateCreditsWindow()
{
	credits->StateChanging(IDLE);

	window = App->gui->AddWindow(ALIGN_CENTERED, 0, nullptr, { 0,20 }, this);
	window->tex = window->LoadTexture("gui/Menu/window.png");
	window->rect = { 0,0, 741, 768 };

	winquit = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -320,120 }, this);
	winquit->SetButtonTex("gui/Menu/Settings/QuitButt.png", "gui/Menu/Settings/QuitButtPressed.png");
	winquit->rect = { 0,0, 61, 72 };
	window->AddWindowElement(winquit);

	license = App->gui->AddImage(ALIGN_CENTERED, "gui/Menu/Credits/license_txt.png", { 0,0, 741, 768 }, { 0,20 });
	window->AddWindowElement(license);

	link = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 220,170 }, this);
	link->SetButtonTex("gui/Menu/Credits/github_icon.png", "gui/Menu/Credits/github_icon.png");
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
	if (sound_bar_length == 0) { return; }
	sound_bar_length--;
	for (int i = 0; i < NUM_BAR_TILES; i++) 
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
	sound_bar_length++;
	for (int i = 0; i < NUM_BAR_TILES; i++)
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
	if (fx_bar_length == 0) { return; }
	fx_bar_length--;
	for (int i = 0; i < NUM_BAR_TILES; i++)
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
	fx_bar_length++;
	for (int i = 0; i < NUM_BAR_TILES; i++)
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

void j1SceneMenu::UpdateBarWithCurrentVolume(uint &bar) {

	if (&bar == &sound_bar_length) {
		float volume = App->audio->GetMusicVolume();
		bar = ((10 * volume) / MIX_MAX_VOLUME);
	}
	if (&bar == &fx_bar_length) {
		float volume = App->audio->GetFxVolume();
		bar = ((10 * volume) / MIX_MAX_VOLUME);
	}

}