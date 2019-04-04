#ifndef _BUNNYHOP_H_
#define _BUNNYHOP_H_

#include "..\main.h"

class Bunnyhop
{
public:
	void Start()
	{
	try{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			
			if (cfg->panicMode)
				continue;
				
			if (!miscUtils->IsCSGOActiveWindow())
				continue;
				
			if (!Setting::MBunnyhop)
				continue;
			
			if (!csgo->IsInGame())
				continue;
				
			if (!csgo->IsTeamSelected())
				continue;
				
			if (LocalEntity.IsDead())
				continue;
				
			if (mem->Read<int>(client->GetImage() + ofs->m_dwForceJump) == 5) {
				while (GetAsyncKeyState(VK_SPACE) & 0x8000) {
					if (LocalEntity.GetFlags() & FL_ONGROUND) {
						mem->Write<int>(client->GetImage() + ofs->m_dwForceJump, 6);
					}
					Sleep(1);
				}
			}
		}
	} catch(...) {
		mem->debuglog(__FILE__);
	}
	}

private:	
	

};

#endif 