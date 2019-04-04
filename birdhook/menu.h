#pragma once
#include "nuklear/src/nuklear.h"
int menuinit();

namespace Setting
{
	extern int VGlowEsp;
	extern int Atriggerbot;
	extern int MBunnyhop;
	extern int AtriggerbotKey;
	extern int Atriggerbotdelay;
	extern int AtriggerbotfovBased;
	extern int VGlowEspAplha;
	extern int VGlowOnlyVisible;
	extern int AAimbotenable;

	extern int AHitboxList[];
	extern int AFOVList[];
	extern int ADelayList[];
	extern int ArcsList[];
	extern int ASmoothList[];
	extern int AKillDelayList[];
	extern int AStartBList[];
	extern int AendbulletList[];
}

class NK_Keybind
{
public:
	void Run(struct nk_context *ctx, int *BindKey, char *KeyName);
private:
	char *KeyString = "[ _ ]";
	bool StartBind = false;
};