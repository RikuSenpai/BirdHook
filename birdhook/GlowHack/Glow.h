#pragma once

#include <Windows.h>

struct CGlow
{
	float r;
	float g;
	float b;
	float a;
	unsigned char p[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	unsigned char p2[10];
};

struct TeamGlow
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 1.0f;
	float a = 1.0f;
};

struct PistolGlow
{
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;
};

struct SniperGlow
{
	float r = 0.398f;
	float g = 0.0f;
	float b = 0.797f;
	float a = 1.0f;
};

struct SmgGlow
{
	float r = 1.0f;
	float g = 0.3f;
	float b = 0.0f;
	float a = 1.0f;
};

struct RifleGlow
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 1.0f;
	float a = 1.0f;
};

struct HeavyGlow
{
	float r = 0.0f;
	float g = 0.797f;
	float b = 0.0f;
	float a = 1.0f;
};


class GlowHack
{
private:
	CGlow Glow;
	TeamGlow tg;
	PistolGlow pistolGlow;
	SniperGlow sniperGlow;
	SmgGlow smgGlow;
	RifleGlow rifleGlow;
	HeavyGlow heavyGlow;
public:
	void GlowOn();
	void GlowOff();
private:
	void SetWeaponGlow(CGlow& glow, const int& entity, const int& classID);
	void SetPlayerGlow(CGlow& glow, const int& entityTeam);
};
