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
		full_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/Hearts.png", { 0,0,32, 35 }, { 40 * i + HEART_POS, HEART_POS }); 
		empty_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/Hearts.png", { 32,0,32, 35 }, { 40 * i + HEART_POS, HEART_POS });
	}

	DrawHearts(App->entities->player->lifes);
	
	SDL_Color bananacolor = { 255, 255, 0, 255 };

	points_img = App->gui->AddImage(ALIGN_CENTERED, "gui/banapoints.png", { 0, 0, 43, 40 }, { 150, 20 } );
	points_text = App->gui->AddText(ALIGN_RIGHT, "0", {-150, 20 }, SKURRI, bananacolor);
	pickups_text = App->gui->AddText(ALIGN_CENTERED, "0", { 190, 20 }, SKURRI, bananacolor);
	timer_text = App->gui->AddText(ALIGN_CENTERED, "000", { 0, 20 }, SKURRI, bananacolor);

	//------TIME----
	//------COINS----

	return true;
}

bool j1PlayerGui::Update(float dt) 
{
	p2SString player_pickups = { "%i", App->entities->player->pickups_counter };
	p2SString player_score = { " %i", App->entities->player->points };
	
	pickups_text->ChangeText(player_pickups);		
	points_text->ChangeText(player_score);//update the same label or change it itereatively

	int sec = timer.ReadSec();
	if (sec > last_sec) {
		sec += base_time;
		if (sec > 999) { sec = 999; }
		p2SString time_txt = { "%03i", sec };
		timer_text->ChangeText(time_txt);
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

	if (pauseMenucreated) { DestroyESCWindow(); }

	return true;
}

void j1PlayerGui::DrawHearts(int current_lifes) {
	for (int i = 0; i < LIFES; i++) {
		full_heart[i]->draw = false;
		empty_heart[i]->draw = false;
	}

	for (int i = 0; i < current_lifes; i++) {
		full_heart[i]->draw = true;
	}

	for (int i = LIFES-1; i >= current_lifes; i--) {
		empty_heart[i]->draw = true;
	}
}

bool j1PlayerGui::Load(pugi::xml_node& data)
{
	pugi::xml_node gui = data; 
	App->entities->player->pickups_counter = data.child("pickups").attribute("value").as_int();
	App->entities->player->points = data.child("score").attribute("value").as_int();
	base_time = data.child("time").attribute("value").as_int();
	return true;
}

bool j1PlayerGui::Save(pugi::xml_node& data) const
{
	pugi::xml_node gui = data;

	gui.append_child("pickups").append_attribute("value") = App->entities->player->pickups_counter;
	gui.append_child("score").append_attribute("value") = App->entities->player->points;
	gui.append_child("time").append_attribute("value") = (int)timer.ReadSec();
	return true;
}

void j1PlayerGui::CreateESCWindow()
{
	window = App->gui->AddWindow(ALIGN_CENTERED,0, nullptr, { 0,100 }, App->entities);
	window->tex = window->LoadTexture("gui/Settings/ESC_window.png");
	window->rect = { 0,0, 348, 384 };

	menu = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -0, 150 }, App->entities);
	window->AddWindowElement(menu);
	menulabel = App->gui->AddText(ALIGN_CENTERED, "MENU", { -0, 150 });
	window->AddWindowElement(menulabel);

	restart = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -0, 200 }, App->entities);
	window->AddWindowElement(restart);
	restartlabel = App->gui->AddText(ALIGN_CENTERED, "RESTART", { -0, 200 });
	window->AddWindowElement(restartlabel);

	resume = App->gui->AddButton(ALIGN_CENTERED, nullptr, { -0, 250 }, App->entities);
	window->AddWindowElement(resume);
	resumelabel = App->gui->AddText(ALIGN_CENTERED, "RESUME", { -0, 250 });
	window->AddWindowElement(resumelabel);

	winquit = App->gui->AddButton(ALIGN_CENTERED, nullptr, { 5, 410 }, App->entities);
	window->AddWindowElement(winquit);
	quitlabel = App->gui->AddText(ALIGN_CENTERED, "QUIT", { 5, 410 });
	window->AddWindowElement(quitlabel);


	pauseMenucreated = true;
}

void j1PlayerGui::DestroyESCWindow()
{
	window->CleanUp();	
	pauseMenucreated = false;
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

			if (elem == window || elem == winquit || elem == restart || elem == resume || elem == menu)
			{
				switch (event)
				{
				case ButtonEvent::LEFT_CLICK:

					if (elem == winquit) { App->audio->PlayFx(App->menu->button_sound); return false; }		//This should exit, but it does not

					if (elem == restart)
					{
							App->audio->PlayFx(App->menu->button_sound);
							App->entities->player->fposition = { (float)App->scene1->initial_scene_pos.x, (float)App->scene1->initial_scene_pos.y };
							App->render->camera.x = 0;
							DestroyESCWindow();
							App->ResumeGame();
					}

					if (elem == resume) 
					{ 
						App->audio->PlayFx(App->menu->button_sound);
						DestroyESCWindow(); 
						App->ResumeGame(); 
					}

					if (elem == menu)
					{ 
						App->audio->PlayFx(App->menu->button_sound);
						DestroyESCWindow();  
						App->fade->FadeToBlack(App->entities, App->menu, 0.8f); 
						App->scene1->SceneChangeMenu(); 
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
	}

	return true;
}