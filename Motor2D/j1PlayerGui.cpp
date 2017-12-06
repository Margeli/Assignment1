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
	for (int i = 0; i < LIFES; i++) 
	{
		full_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/Hearts.png", { 0,0,32, 35 }, { 40 * i + HEART_POS, HEART_POS }); 
		empty_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/Hearts.png", { 32,0,32, 35 }, { 40 * i + HEART_POS, HEART_POS });
	}
	DrawHearts(App->entities->player->lifes);

	
	SDL_Color bananacolor = { 255, 255, 0, 255 };

	points_img = App->gui->AddImage(ALIGN_CENTERED, "gui/banapoints.png", { 0, 0, 43, 40 }, { HEART_POS, HEART_POS});
	points_text = App->gui->AddText(ALIGN_RIGHT, "0", {-40, 20 }, ZEALAND, bananacolor);
	pickups_text = App->gui->AddText(ALIGN_CENTERED, "0", { 50, 20 }, ZEALAND, bananacolor);

	//------TIME----
	//------COINS----

	return true;
}

bool j1PlayerGui::Update(float dt) 
{
	p2SString player_score = { " %i", App->entities->player->points };
	
	points_text->ChangeText(player_score);//update the same label or change it itereatively

	p2SString player_pickups = { "%i", App->entities->player->pickups_counter };
	pickups_text->ChangeText(player_pickups);

	return true;
}

bool j1PlayerGui::CleanUp() {
	for (int i = 0; i < LIFES; i++) {
		full_heart[i]->CleanUp();
		empty_heart[i]->CleanUp();		
	}
	points_img->CleanUp();
	points_text->CleanUp();

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