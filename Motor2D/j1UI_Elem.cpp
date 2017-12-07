#include "j1UI_Elem.h"
#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Input.h"

j1UI_Elem::j1UI_Elem(UIType type, Alignment align, j1Module* listener) : type(type), align(align), listener(listener) {}

bool j1UI_Elem::Start() { return true; }

bool j1UI_Elem::CleanUp() { return true; }

j1UI_Elem::~j1UI_Elem() {}

SDL_Texture* j1UI_Elem::LoadTexture(p2SString path) 
{

	SDL_Texture* ret = App->tex->Load(path.GetString());
	return ret;
}

void j1UI_Elem::UpdateAlignment() 
{
	switch (align) 
	{
	case NO_ALIGN:
		break;
	case ALIGN_CENTERED:
		position.x = (-App->render->camera.x + App->render->camera.w / 2) -(rect.w / 2);
		position.y = -App->render->camera.y;
		break;
	case ALIGN_RIGHT:
		position.x = (-App->render->camera.x + App->render->camera.w) - rect.w;
		position.y = -App->render->camera.y;
		break;
	case ALIGN_LEFT:
		position.x = -App->render->camera.x;
		position.y = -App->render->camera.y;
		break;
	}
}

bool j1UI_Elem::Update(float dt) { return true; }

void j1UI_Elem::DebugDraw() 
{
	
	App->render->DrawQuad({position.x+ displacement.x, position.y+displacement.y, rect.w, rect.h }, 255, 255, 0, 100, false);
}
