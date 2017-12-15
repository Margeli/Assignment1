#ifndef __j1SCENE_MENU_H__
#define __j1SCENE_MENU_H__

#include "j1Module.h"
#include "j1Render.h"
#include "p2List.h"

#define NUM_BAR_TILES 10
#define DEFAULT_BAR_LENGHT 5

struct SDL_Texture;
struct Mix_Chunk;
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

	bool OnEventChange(j1UI_Elem* elem, ButtonEvent event) ;

	

private:

	void LoadGame();
	void CreateSettingWindow();
	void CreateCreditsWindow();
	void DestroyWindow();
	void ShiftVolumeBarLeft();
	void ShiftVolumeBarRight();
	void ShiftFXBarLeft();
	void ShiftFXBarRight();
	
	
	bool settingwindowcreated = false;
	bool creditswindowcreated = false;
	bool toChangeScene = false;
	bool fullscreenbool = false;

	float fx_volume;
	float current_volume;

	GuiWindow*	window = nullptr;
	GuiButton* winquit = nullptr;
	GuiButton* link = nullptr;
	GuiButton* fullscreen = nullptr;
	GuiButton* winsoundplus = nullptr;	
	GuiButton* winsoundmin = nullptr;
	GuiButton* winfxmin = nullptr;
	GuiButton* winfxplus = nullptr;
	GuiImage* license = nullptr;
	GuiImage* winsoundbar = nullptr;
	GuiImage* winfxbar = nullptr;
	GuiImage* winsetticon = nullptr;

	GuiLabel* winsoundtxt = nullptr;
	GuiLabel* settingsback = nullptr;
	GuiLabel* fullscreenlabel = nullptr;
	GuiLabel* winfxtxt = nullptr;

	GuiImage* winfxtile[NUM_BAR_TILES];
	GuiImage* winsoundtile[NUM_BAR_TILES];

	GuiButton*	play = nullptr;
	GuiButton*	cont = nullptr;
	GuiButton*	settings = nullptr;
	GuiButton*	credits = nullptr;
	GuiButton*	exit = nullptr;

	SDL_Texture* background = nullptr;
	SDL_Rect	background_rect;
	Mix_Chunk* sample;
	Uint32 winfullscr = 0;
	p2List<j1UI_Elem*> menu_elems;

public:
	uint sound_bar_length;
	uint fx_bar_length;
	uint button_sound;
	uint door_sound;
};

#endif // __j1SCENE_MENU_H__