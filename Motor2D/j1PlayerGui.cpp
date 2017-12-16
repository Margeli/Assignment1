#include "j1PlayerGui.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Collectables.h"
#include "j1SceneMenu.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1UI_Elem.h"

#define HEART_POS 20
#define BANANA_POS_Y 10
#define BANANA_POS_X -80

j1PlayerGui::j1PlayerGui()
{
	for (int i = 0; i < LIFES; i++)
	{
		full_heart[i] = nullptr;
		empty_heart[i] = nullptr;
	}
}

j1PlayerGui::~j1PlayerGui()
{}

bool j1PlayerGui::Start() 
{
	timer.Start();
	last_sec = 0;
	for (int i = 0; i < LIFES; i++) 
	{
		full_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/PlayerGui/Hearts.png", { 0,0,32, 35 }, { 40 * i + HEART_POS, HEART_POS }); 
		empty_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/PlayerGui/Hearts.png", { 32,0,32, 35 }, { 40 * i + HEART_POS, HEART_POS });
	}

	DrawHearts(App->entities->player->lifes);
	
	SDL_Color bananacolor = { 255, 255, 0, 255 };

	points_img = App->gui->AddImage(ALIGN_CENTERED, "gui/PlayerGui/banapoints.png", { 0, 0, 43, 40 }, { 150, 20 } );
	points_text = App->gui->AddText(ALIGN_RIGHT, "0", {-150, 20 }, SKURRI, bananacolor);
	pickups_text = App->gui->AddText(ALIGN_CENTERED, "0", { 190, 20 }, SKURRI, bananacolor);
	timer_text = App->gui->AddText(ALIGN_CENTERED, "000", { 0, 20 }, SKURRI, bananacolor);

	pausetime.SetZero();
	return true;
}

bool j1PlayerGui::Update(float dt) 
{
		p2SString player_pickups = { "%i", App->entities->player->pickups_counter };
		p2SString player_score = { " %i", App->entities->player->points };

		pickups_text->ChangeText(player_pickups);
		points_text->ChangeText(player_score);//update the same label or change it itereatively
		if (App->scene1->active) {
			if (!pausetime.IsZero()) {
				App->scene1->time.SubstractTime(pausetime);
				pausetime.SetZero();
				start_pause = true;
			}
			int sec = App->scene1->time.ReadSec();
			if (sec > last_sec) {
				sec += App->scene1->saved_time;
				if (sec > 999) { sec = 999; }
				p2SString time_txt = { "%03i", sec };
				timer_text->ChangeText(time_txt);
			}
		}
		if (App->scene2->active) {
			if (!pausetime.IsZero()) {
				App->scene2->time.SubstractTime(pausetime);
				pausetime.SetZero();
				start_pause = true;
			}
			int sec = App->scene2->time.ReadSec();
			if (sec > last_sec) {
				sec += App->scene2->saved_time;

				if (sec > 999) { sec = 999; }
				p2SString time_txt = { "%03i", sec };
				timer_text->ChangeText(time_txt);
			}
		}
	return true;
}

bool j1PlayerGui::CleanUp()
{
	for (int i = 0; i < LIFES; i++)
	{
		full_heart[i]->CleanUp();
		empty_heart[i]->CleanUp();
	}

	
	timer_text->CleanUp();
	pickups_text->CleanUp();
	points_text->CleanUp();	
	points_img->CleanUp();

	if (pauseMenucreated) { 
		DestroyESCWindow(); }

	return true;
}

void j1PlayerGui::DrawHearts(int current_lifes) 
{
	for (int i = 0; i < LIFES; i++) 
	{
		full_heart[i]->draw = false;
		empty_heart[i]->draw = false;
	}

	for (int i = 0; i < current_lifes; i++) 
	{
		full_heart[i]->draw = true;
	}

	for (int i = LIFES-1; i >= current_lifes; i--) 
	{
		empty_heart[i]->draw = true;
	}
}

bool j1PlayerGui::Load(pugi::xml_node& data)
{
	pugi::xml_node gui = data; 
	App->entities->player->pickups_counter = data.child("pickups").attribute("value").as_int();
	App->entities->player->points = data.child("score").attribute("value").as_int();
	
	return true;
}

bool j1PlayerGui::Save(pugi::xml_node& data) const
{
	pugi::xml_node gui = data;

	gui.append_child("pickups").append_attribute("value") = App->entities->player->pickups_counter;
	gui.append_child("score").append_attribute("value") = App->entities->player->points;
	
	return true;
}

void j1PlayerGui::CreateESCWindow()
{
	window = App->gui->AddWindow(ALIGN_CENTERED,0, nullptr, { 0,100 }, App->entities);
	window->tex = window->LoadTexture("gui/PauseMenu/ESC_window.png");
	window->rect = { 0,0, 509, 562 };

	menu = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -180, 200 }, App->entities);
	menu->SetButtonTex("gui/PauseMenu/menu.png", "gui/PauseMenu/menu_pressed.png");
	menu_label = App->gui->AddText(ALIGN_CENTERED, "MENU", { -80, 200 });
	window->AddWindowElement(menu_label);
	menu->rect = { 0, 0, 46, 46 };
	window->AddWindowElement(menu);

	restart = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -180, 300 }, App->entities);
	restart->SetButtonTex("gui/PauseMenu/restart.png", "gui/PauseMenu/restart_pressed.png");
	restart_label = App->gui->AddText(ALIGN_CENTERED, "RESTART", { -55, 305 });
	window->AddWindowElement(restart_label);
	restart->rect = { 0, 0, 46, 46 };
	window->AddWindowElement(restart);
	
	resume = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -180, 400 }, App->entities);
	resume->SetButtonTex("gui/PauseMenu/play.png", "gui/PauseMenu/play_pressed.png");
	resume_label = App->gui->AddText(ALIGN_CENTERED, "RESUME", { -60, 400 });
	window->AddWindowElement(resume_label);
	resume->rect = { 0, 0, 46, 46 };
	window->AddWindowElement(resume);
	winquit = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 10, 565 }, App->entities);
	winquit->SetButtonTex("gui/PauseMenu/ExitButtESC.png", "gui/PauseMenu/ExitButtESCHover.png");
	winquit->rect = { 0, 0,111,41 };
	window->AddWindowElement(winquit);

	pauseMenucreated = true;
}

void j1PlayerGui::DestroyESCWindow()
{
	window->CleanUp();
	window = nullptr;
	pauseMenucreated = false;
}

void j1PlayerGui::CreateENDWindow()
{
	App->fade->FadeToBlack(App->scene2, App->scene2, 2.0f);

	window = App->gui->AddWindow(ALIGN_CENTERED, 0, nullptr, { 0,100 }, App->entities);
	window->tex = window->LoadTexture("gui/PauseMenu/ESC_window.png");
	window->rect = { 0,0, 509, 562 };
}

void j1PlayerGui::DestroyENDWindow()
{

}


bool j1PlayerGui::OnEventChange(j1UI_Elem* elem, ButtonEvent event) 
{
	if (pauseMenucreated)
	{
		switch (event)
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

		switch (event)
		{
		case ButtonEvent::LEFT_CLICK:

			if (elem == winquit) {
				App->audio->PlayFx(App->menu->button_sound);
				return false; }		

			if (elem == restart)
			{
				App->audio->PlayFx(App->menu->button_sound);
				App->ResumeGame();
				if (App->scene1->active == true) { 
					App->scene1->CleanUp();
					App->scene2->SceneChange(); }
				if (App->scene2->active == true) {
					App->scene2->CleanUp();
					App->scene1->SceneChange(); }			
				
				return true;
			}

			if (elem == resume)
			{
				App->audio->PlayFx(App->menu->button_sound);
				DestroyESCWindow();
				App->ResumeGame();
				return true;
			}

			if (elem == menu)
			{
				App->audio->PlayFx(App->menu->button_sound);				
				App->fade->FadeToBlack(App->entities, App->menu, 0.8f);
				if (App->scene1->active == true) { App->scene1->SceneChangeMenu(); }
				if (App->scene2->active == true) { App->scene2->SceneChangeMenu(); }
				return true;
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
			if (elem == restart || elem == resume || elem == menu || elem == winquit) { window->can_move = true; }

			elem->StateChanging(IDLE);
			break;
		}
	}
	return true;
}

void j1PlayerGui::PauseTime() 
{
	if (start_pause) { pausetime.Start(); start_pause = false; }
}