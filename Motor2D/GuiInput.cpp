#include "GuiInput.h"
#include "j1Gui.h"
#include "j1UI_Elem.h"
#include "j1App.h"
#include "j1Render.h"


GuiInput::GuiInput(Alignment alignment) : j1UI_Elem(UIType::INPUTBOX, Alignment::NO_ALIGN) {
	align = alignment;
}


GuiInput::~GuiInput()
{
}

bool GuiInput::Start() {

	
	return true;
}

bool GuiInput::CleanUp() {

	//App->tex->UnLoad(tex);
	return true;

}
bool GuiInput::Update(float dt) {

	UpdateAlignment();
	App->render->Blit(tex, position.x + displacement.x, position.y + displacement.y, &rect);
	return true;
}