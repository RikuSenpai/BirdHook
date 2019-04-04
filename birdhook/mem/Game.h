#pragma once

#include <Windows.h>
#include <vector>
#include "../menu.h"

#define WAITTIME 100  // Wait time between keys
#define MAXPLAYERS 64
#define MAXPLAYERNAMESIZE 32
#define MAXLOCATIONSIZE 15
#define MAXMODELNAMESIZE 64
#define MAXWEAPONS 10
#define MAXWEAPONNAMESIZE 20


extern std::vector<int> primary;
extern std::vector<int> secondary;

struct HackSwitches
{
	bool			wallhack = false;
	bool			aimbot = false;
	bool			triggerbot = false;
	bool			radarHack = false;
	bool			overlay = false;
};

struct Modules
{
	int				procID;
	uintptr_t		gameModule;
	uintptr_t		engineModule;
};

struct KnownAddresses
{
	uintptr_t		localPlayer;
	uintptr_t		glowObject;
};

struct Variables
{
	float			maxHealth = 100.0f;
	float			maxDzHealth = 120.0f;
	bool			dz = false;

	int				glowCount;
	int				localPlayerTeam;
};

struct Keys
{
	int				wallhackKey = VK_NUMPAD0;
	int				bomberKey = VK_NUMPAD1;
	int				dzKey = VK_NUMPAD2;
	int				radarHackKey = VK_NUMPAD3;
};

struct Weapons
{
	int				weapons[64];
};

struct PlayerWeapons
{
	int				primary;
	int				secondary;
};

struct Location
{
	char			loc[MAXLOCATIONSIZE];
};

struct Position
{
	float			pos[3];
};

struct Player
{
	bool			active = true;
	bool			dormant = false;
	std::wstring	name = L"";
	std::wstring	location = L"";
	float			position[3];
	int				entity = 0;
	unsigned char	index = { 0 };
	int				health = 0;
	int				team;
	int				activeWeaon = 0;
	int				weapons[MAXWEAPONS] = { 0 };
};

struct PlayerName
{
	char			name[MAXPLAYERNAMESIZE] = { 0 };
};

struct ModelName
{
	char			modelName[MAXMODELNAMESIZE] = { 0 };
};

struct ViewMatrix
{
	float			flMatrix[4][4];
};

extern HackSwitches			hackSwitches;
extern Modules				modules;
extern KnownAddresses		objects;
extern Variables			val;
extern Keys					hackSwitchKeys;

bool				getProcID();
bool				getClientModule();
bool				getLocalPlayer();  // 1 = Successful, 0 = Could not find localPlayer
void				GetEntityName(unsigned char index, std::wstring *name);
int					GetEntityClassID(const int entity);
void				GetPlayerWeapons(const int entity, PlayerWeapons& weapons);
int					GetPlayerActiveWeapon(const int entity);
void				GetEntityLocation(const int entity, std::wstring *location);
void				GetStringFromClassID(const int classID, std::wstring& name);
bool				EntityCarriesBomb(const int entity);
bool				IsPrimaryWeapon(const int classID);
bool				IsSecondaryWeapon(const int classID);
bool				IsGrenade(const int classID);
inline int			GetEntityTeam(const int entity) { return  <int>(entity + m_iTeamNum); }
inline bool			GetEntityDormant(const int entity) { return MemClass.readMem<bool>(entity + m_bDormant); }
inline bool			EntityIsDefusing(const int entity) { return MemClass.readMem<bool>(entity + m_bIsDefusing); }
inline bool			EntityHasDefuser(const int entity) { return MemClass.readMem<bool>(entity + m_bHasDefuser); }
inline int			GetEntityHealth(const int entity) { return MemClass.readMem<int>(entity + m_iHealth); }
void				GetEntityModelName(const int entity, ModelName *modelName);
void				GetEntityPosition(const int entity, float *pos);
void				GetPlayerInfo(unsigned char index, Player *player);
void				GetPlayersInfo(Player *plays);
void				GetPlayersInfoString(Player *plays, std::wstring *info);
inline void			GetViewMatrix(ViewMatrix *vm) { *vm = MemClass.readMem<ViewMatrix>(modules.gameModule + dwViewMatrix); }