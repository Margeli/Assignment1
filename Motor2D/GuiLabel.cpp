#include "GuiLabel.h"
#include "j1Gui.h"
#include "j1UI_Elem.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Brofiler\Brofiler.h"

GuiLabel::GuiLabel(Alignment alignment) : j1UI_Elem(UIType::LABEL, Alignment::NO_ALIGN) 
{
	align = alignment;	
	
}

GuiLabel::~GuiLabel()
{}

bool GuiLabel::Start() 
{
	int width, height;
	App->font->CalcSize(text.GetString(), width, height, text_font);
	rect.w = width;
	rect.h = height;
	return true;
}

bool GuiLabel::CleanUp() 
{
	to_delete = true;
	App->tex->UnLoad(tex);

	return true;
}

void GuiLabel::CreateText(p2SString txt, SDL_Color color, FontType font)
{
	_TTF_Font* fnt = nullptr;
	int font_size = DEFAULT_FONT_SIZE;
	const char* path = nullptr;
	switch (font)
	{
	case FRIZQT:
		path = "fonts/FRIZQT__.ttf";	
		break;
	case MORPHEUS:
		path = "fonts/MORPHEUS.ttf";		
		break;
	case SKURRI:
		path = "fonts/skurri.ttf";
		break;
	case ARIALN:
		path= "fonts/ARIALN.ttf";
		break;
	case ZEALAND:
		path = "fonts/DK Zealand.otf";
		break;
	}
	fnt = App->font->Load(path, font_size);


	text_color = color;
	text_font = fnt;
	text = txt;
	tex = App->font->Print(text.GetString(), text_color, text_font);
}

bool GuiLabel::Update(float dt) 
{
	BROFILER_CATEGORY("GuiLabel_Update", Profiler::Color::OrangeRed);
	UpdateAlignment();
	App->render->Blit(tex, position.x+ displacement.x, position.y+ displacement.y);
	return true;
}

void GuiLabel::ChangeText(p2SString newtext) 
{
	App->tex->UnLoad(tex);
	text = newtext;	
	tex = App->font->Print(newtext.GetString(), text_color, text_font);
}

void GuiLabel::Drag(iPoint displace) 
{
	displacement.x += displace.x;
	displacement.y += displace.y;
}