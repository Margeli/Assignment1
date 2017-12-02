#include "j1PlayerGui.h"
#include "j1Gui.h"
#include "j1App.h"



j1PlayerGui::j1PlayerGui()
{
	for (int i = 0; i < LIFES; i++) {
		full_heart[i] = nullptr;
		empty_heart[i] = nullptr;
	}
}


j1PlayerGui::~j1PlayerGui()
{
}

bool j1PlayerGui::Start() {

	//---------PLAYER LIFES----
	for (int i = 0; i < LIFES; i++) {
		full_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/Hearts2.png", { 0,0,32, 35 }, { 40 * i, 20 }); //need to fins the right coords for the rect and imporve the heart image
		empty_heart[i] = App->gui->AddImage(ALIGN_LEFT, "gui/Hearts2.png", { 32,0,32, 35 }, { 40 * i, 20 });
		}
	DrawHearts(App->entities->player->lifes);
	//---------SCORE-------
	//------TIME----
	//------COINS----


	return true;
}

bool j1PlayerGui::Update(float dt) {

	return true;
}

bool j1PlayerGui::CleanUp() {
	for (int i = 0; i < LIFES; i++) {
		App->gui->DestroyElement(full_heart[i]);
		App->gui->DestroyElement(empty_heart[i]);
	}
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