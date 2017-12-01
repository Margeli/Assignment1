#ifndef __j1SCENE_MENU_H__
#define __j1SCENE_MENU_H__

#include "j1Module.h"
#include "j1Render.h"

struct SDL_Texture;

class GuiWindow;
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

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void SceneChange();

	bool fading = false;

	GuiWindow* window;

	void OnEventChange(j1UI_Elem* elem, ButtonEvent event) const;
};

#endif // __j1SCENE_MENU_H__