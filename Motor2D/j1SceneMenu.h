#ifndef __j1SCENE_MENU_H__
#define __j1SCENE_MENU_H__

#include "j1Module.h"
#include "j1Render.h"

struct SDL_Texture;
class SDL_Rect;
class GuiWindow;
class GuiButton;
class jUI_Elem;
enum ButtonEvent;

class j1SceneMenu : public j1Module
{
public:

	j1SceneMenu();

	virtual ~j1SceneMenu();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void SceneChange();

	bool fading = false;

	

	void OnEventChange(j1UI_Elem* elem, ButtonEvent event) const;

private:

	GuiWindow*	window = nullptr;

	GuiButton*	play = nullptr;
	GuiButton*	cont = nullptr;
	GuiButton*	settings = nullptr;
	GuiButton*	credits = nullptr;
	GuiButton*	exit = nullptr;

	SDL_Texture* background = nullptr;
	SDL_Rect	background_rect;
};

#endif // __j1SCENE_MENU_H__