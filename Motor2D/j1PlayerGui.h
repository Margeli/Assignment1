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
	void PauseTime();

	bool OnEventChange(j1UI_Elem* elem, ButtonEvent event);

	GuiImage* points_img = nullptr;
	GuiLabel* points_text = nullptr;
	GuiLabel* pickups_text = nullptr;
	GuiLabel* timer_text = nullptr;

private:
	j1Timer		timer;
	j1Timer		pausetime;

	GuiImage* full_heart[LIFES];
	GuiImage* empty_heart[LIFES];

	

	uint last_sec;
	uint base_time=0;

	GuiWindow* window = nullptr;

	GuiButton* winquit = nullptr;
	GuiButton* restart = nullptr;
	GuiButton* resume = nullptr;
	GuiButton* menu = nullptr;

	bool pauseMenucreated = false;
	bool start_pause = true;
};

#endif //__j1PLAYERGUI_H__