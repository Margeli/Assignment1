#include "j1PlayerGui.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Collectables.h"

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

bool j1PlayerGui::CleanUp() {
	for (int i = 0; i < LIFES; i++) {
		full_heart[i]->CleanUp();
		empty_heart[i]->CleanUp();		
	}

	timer_text->CleanUp();

	pickups_text->CleanUp();
	points_text->CleanUp();
	points_img->CleanUp();

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
	window = App->gui->AddWindow(ALIGN_CENTERED, 4, nullptr, { 0,100 }, App->entities);
	window->tex = window->LoadTexture("gui/Settings/ESC_window.png");
	window->rect = { 0,0, 741, 768 };
}

void j1PlayerGui::DestroyESCWindow()
{
	window->CleanUp();	//TODO
}

bool j1PlayerGui::OnEventChange(j1UI_Elem* elem, ButtonEvent event) 
{
	return true;
}