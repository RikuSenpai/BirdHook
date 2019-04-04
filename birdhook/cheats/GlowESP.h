#ifndef _GLOWESP_H_
#define _GLOWESP_H_

#include "..\main.h"
#include "../menu.h"

class GlowESP
{
public:
	void Start()
	{
	try{
		for (;;) {
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			
			if (cfg->panicMode)
				continue;
				
			if (!miscUtils->IsCSGOActiveWindow())
				continue;
				
			if (!Setting::VGlowEsp)
				continue;
				
			if (!csgo->IsInGame())
				continue;			
			
			DWORD GlowPointer = mem->Read<DWORD>(client->GetImage() + ofs->m_dwGlowObject);

			for (int g = 0; g < csgo->GetMaxObjects(); g++) {
				
				DWORD Entity = mem->Read<DWORD>(client->GetImage() + ofs->m_dwEntityList + g * 0x10);
						
				if (Entity != 0) {
					int EntityClassID = mem->Read<int>(mem->Read<int>(mem->Read<int>(mem->Read<int>(Entity + 0x8) + 2 * 0x4) + 0x1) + 0x14);
					if (EntityClassID == 39)
					{
						indexBomb = mem->Read<int>(Entity + ofs->m_iPlayerC4);
					}
				}
				
				DWORD glowBase = GlowPointer + (g * sizeof(GlowObjectDefinition_t));
				glowObj = mem->Read<GlowObjectDefinition_t>(glowBase);
				
				if (GlowPointer != NULL && glowObj.dwBase != NULL)
				{				
					int ClassID = mem->Read<int>(mem->Read<int>(mem->Read<int>(mem->Read<int>(glowObj.dwBase + 0x8) + 2 * 0x4) + 0x1) + 0x14);
					
					if (ClassID == 38)
					{
						bool IsDormant = mem->Read<bool>(glowObj.dwBase + ofs->m_bDormant);
						
						if (!IsDormant)
						{
							int TeamNum = mem->Read<int>(glowObj.dwBase + ofs->m_iTeamNum); 
							int LocalPlayer_TeamNum = LocalEntity.GetTeamNum();
							
							if (TeamNum != LocalPlayer_TeamNum)
							{
								int Health = mem->Read<int>(glowObj.dwBase + ofs->m_iHealth); 
								
								if (TeamNum != 2 && TeamNum != 3)
									continue;
								
								int Index = mem->Read<int>(glowObj.dwBase + ofs->m_dwIndex);
								
								bool isDefusing = false;
								if (TeamNum == 3) 
									isDefusing = mem->Read<bool>(glowObj.dwBase + ofs->m_bIsDefusing);
									
								if (Index == indexBomb || isDefusing)
								{
									DrawGlow(glowBase, CBlue );
								}
								else if (Health <= 100 && Health > 75)
								{
									DrawGlow(glowBase, CGreen );
								}
								else if (Health <= 75 && Health > 50)
								{
									DrawGlow(glowBase, CYellow);
								}
								else if (Health <= 50 && Health > 25)
								{
									DrawGlow(glowBase, COrange);
								}
								else if (Health <= 25 && Health > 1)
								{
									DrawGlow(glowBase, CRed);
								}
								else if (Health <= 1 && Health > 0)
								{
									DrawGlow(glowBase, CWhite);
								}
							}
						}
					}
				}
			}
		}
	} catch(...) {
		mem->debuglog(__FILE__);
	}
	}

private:
	
	int indexBomb = -1;
	
	struct GlowColor
	{
		float r;
		float g;
		float b;
		float a;
	};
	
	GlowColor CGreen = { 0.f, 0.4f, 0.f, 1.f};
	GlowColor CYellow = { 0.4f, 0.4f, 0.f, 1.f};
	GlowColor CRed = { 0.4f, 0.f, 0.f, 1.f};
	GlowColor CBlue = { 0.f, 0.f, 0.4f, 1.f};
	GlowColor COrange = { 1.f, 0.5, 0.f, 1.f};
	GlowColor CPink = { 1.f, 0.55f, 0.7f, 1.f};
	GlowColor CWhite = { 1.f, 1.f, 1.f, 1.f};
	
	void DrawGlow(DWORD dwBase, GlowColor Color)
	{
		glowObj.r = Color.r;
		glowObj.g = Color.g;
		glowObj.b = Color.b;
		//glowObj.a = Color.a;
		glowObj.a = sanitizeChannel(Setting::VGlowEspAplha);
		glowObj.m_bRenderWhenOccluded = true;
		glowObj.m_bRenderWhenUnoccluded = false;
		glowObj.m_bFullBloom = false;
		
		mem->Write<GlowObjectDefinition_t>(dwBase, glowObj);
		
		if (Setting::VGlowOnlyVisible)
			mem->Write<bool>(dwBase + 0x2C, true);//(OnlyVisible)
		
		if (cfg->glowesp.bodycolorenable && csgo->IsTeamSelected())
			mem->Write(glowObj.dwBase + 0x70, ColorESP(cfg->glowesp.bodyred * 1.0f, cfg->glowesp.bodygreen * 1.0f, cfg->glowesp.bodyblue * 1.0f, cfg->glowesp.bodyalpha * 1.0f));
	}
	
	float sanitizeChannel(int value) {
        if (value > 255) value = 255;
        if (value < 0) value = 0;
        return (float) value / 255;
	}
	
		
	//struct GlowObjectDefinition_t
	//{
 //       DWORD dwBase;
 //       float r;
 //       float g;
 //       float b;
 //       float a;
 //       char pad_0014[4];
 //       float flUnknown;
 //       float flBloomAmount;
 //       float flLocalPlayerIsZeroPointThree;
 //       bool m_bRenderWhenOccluded;
 //       bool m_bRenderWhenUnoccluded;
 //       bool m_bFullBloom;
 //       char pad_0027[1];
 //       int nFullBloomTestStencilValue; // Needs to be zero.
 //       int nGlowStyle;
 //       int nSplitScreenSlot; // Should be -1.
 //       int nNextFreeSlot;
	//}glowObj;

	class GlowObjectDefinition_t
	{
	public:
		GlowObjectDefinition_t() { memset(this, 0, sizeof(*this)); }

		DWORD dwBase;    //0x0000
		float r;
		float g;
		float b;
		float a;
		uint8_t pad_0014[4];               //0x0014
		float   m_flSomeFloat;             //0x0018
		uint8_t pad_001C[4];               //0x001C
		float   m_flAnotherFloat;          //0x0020
		bool    m_bRenderWhenOccluded;     //0x0024
		bool    m_bRenderWhenUnoccluded;   //0x0025
		bool m_bFullBloom;;        //0x0026
		uint8_t pad_0027[5];               //0x0027
		int32_t m_nGlowStyle;              //0x002C
		int32_t m_nSplitScreenSlot;        //0x0030
		int32_t m_nNextFreeSlot;           //0x0034

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	}glowObj;
};
	
#endif
