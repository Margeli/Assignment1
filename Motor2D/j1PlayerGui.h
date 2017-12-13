#ifndef __j1PLAYERGUI_H__
#define __j1PLAYERGUI_H__

#include "j1Player.h"
#include "j1Timer.h"

class GuiImage;
class GuiLabel;
class GuiWindow;
class GuiButton;

class j1PlayerGui
{
public:
	j1PlayerGui();
	~j1PlayerGui();


	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void DrawHearts(int current_lifes);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	void CreateESCWindow();
	void DestroyESCWindow();

	bool OnEventChange(j1UI_Elem* elem, ButtonEvent event);

private:
	j1Timer		timer;

	GuiImage* full_heart[LIFES];
	GuiImage* empty_heart[LIFES];

	GuiImage* points_img;
	GuiLabel* points_text;
	GuiLabel* pickups_text;
	GuiLabel* timer_text;

	uint last_sec;
	uint base_time=0;

	GuiWindow* window = nullptr;

	GuiButton* winquit = nullptr;
	GuiButton* restart = nullptr;
	GuiButton* resume = nullptr;
	GuiButton* menu = nullptr;

	GuiLabel* quitlabel = nullptr;
	GuiLabel* restartlabel = nullptr;
	GuiLabel* resumelabel = nullptr;
	GuiLabel* menulabel = nullptr;

	bool pauseMenucreated = false;

};

#endif //__j1PLAYERGUI_H__