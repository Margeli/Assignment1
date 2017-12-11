#ifndef __j1PLAYERGUI_H__
#define __j1PLAYERGUI_H__

#include "j1Player.h"
#include "j1Timer.h"

class GuiImage;
class GuiLabel;

class j1PlayerGui
{
public:
	j1PlayerGui();
	~j1PlayerGui();


	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void DrawHearts(int current_lifes);

private:
	j1Timer		timer;
	GuiImage* full_heart[LIFES];
	GuiImage* empty_heart[LIFES];

	GuiImage* points_img;
	GuiLabel* points_text;
	GuiLabel* pickups_text;
	GuiLabel* timer_text;
	uint last_sec;
};

#endif //__j1PLAYERGUI_H__