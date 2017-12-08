#ifndef __j1SCENE_MENU_H__
#define __j1SCENE_MENU_H__

#include "j1Module.h"
#include "j1Render.h"
#include "p2List.h"

struct SDL_Texture;
class SDL_Rect;
class GuiWindow;
class GuiButton;
class GuiImage;
class GuiLabel;
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

	

	bool OnEventChange(j1UI_Elem* elem, ButtonEvent event) ;

private:

	void CreateSettingWindow();
	void DestroySettingWindow();
	void ShiftVolumeLeft();
	void ShiftVolumeRight();
	
	bool settingwindowcreated = false;

	GuiWindow*	window = nullptr;
	GuiButton* winquit = nullptr;
	GuiButton* winsoundmin = nullptr;
	GuiButton* winsoundplus = nullptr;
	GuiImage* winsoundbar = nullptr;
	GuiImage* winsetticon = nullptr;
	GuiLabel* winsoundtxt = nullptr;
	GuiImage* winsoundtile[10];

	GuiButton*	play = nullptr;
	GuiButton*	cont = nullptr;
	GuiButton*	settings = nullptr;
	GuiButton*	credits = nullptr;
	GuiButton*	exit = nullptr;

	SDL_Texture* background = nullptr;
	SDL_Rect	background_rect;

	p2List<j1UI_Elem*> menu_elems;
};

#endif // __j1SCENE_MENU_H__