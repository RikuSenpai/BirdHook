#include <iostream>
#include <Windows.h>
#include <codecvt>

#include "Game.h"
#include "mem.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

std::vector<int> primary = { items::mag7, items::nova, items::sawedoff, items::xm1014, items::m249, items::negev, items::mac10, items::mp9, items::mp7, items::ump, items::p90, items::bizon, items::galil, items::famas, items::ak47, items::m4a4, items::sg553, items::aug, items::scout, items::awp, items::scar20, items::g3sg1 };
std::vector<int> secondary = { items::glock, items::usp, items::p250, items::duals, items::tec9, items::fiveseven, items::deagle };

HackSwitches hackSwitches;
Modules modules;
KnownAddresses objects;
Variables val;
Keys hackSwitchKeys;

bool getProcID()
{
	modules.procID = MemClass.getProcess(L"csgo.exe", 2);
	if (modules.procID) return true;
	return false;
}

bool getClientModule()
{
	modules.gameModule = MemClass.getModule(modules.procID, L"client_panorama.dll", 1);
	if (modules.gameModule) return true;
	return false;
}

bool getLocalPlayer()
{
	objects.localPlayer = MemClass.readMem<uintptr_t>(modules.gameModule + dwLocalPlayer);
	if (objects.localPlayer == NULL)
	{
		return false;
	}
	return true;
}

void GetEntityName(unsigned char index, std::wstring *name)
{
	uintptr_t one = MemClass.readMem<uintptr_t>(modules.gameModule + dwRadarBase);
	uintptr_t two = MemClass.readMem<uintptr_t>(one + offsets::m_iRadarBasePtr);
	PlayerName nam = MemClass.readMem<PlayerName>(two + offsets::m_iRadarStructPos + (index + 2) * offsets::m_iRadarStructSize);
	if (nam.name[0] == 0) return;

	std::string buf = "";
	for (int i = 0; nam.name[i] != '\0'; ++i)
	{
		buf += nam.name[i];
	}
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	*name = myconv.from_bytes(buf);
}

int GetEntityClassID(const int entity)
{
	uintptr_t one = MemClass.readMem<uintptr_t>(entity + 0x8);
	uintptr_t two = MemClass.readMem<uintptr_t>(one + 0x8);
	uintptr_t three = MemClass.readMem<uintptr_t>(two + 0x1);

	int classID = MemClass.readMem<int>(three + 0x14);

	return classID;
}

void GetEntityLocation(const int entity, std::wstring *location)
{
	*location = L"";
	Location loc = MemClass.readMem<Location>(entity + m_szLastPlaceName);
	for (short int j(0); loc.loc[j] != '\0'; ++j)
	{
		*location += loc.loc[j];
	}
}

bool EntityCarriesBomb(const int entity)
{
	for (char i(0); i < 64; ++i)
	{
		int hWeapon = MemClass.readMem<int>(entity + m_hMyWeapons + i * 4);
		int weaponEntity = MemClass.readMem<int>(modules.gameModule + dwEntityList + ((hWeapon & 0xFFF) - 1) * 0x10);
		if (weaponEntity == 0) continue;
		int weaponClassID = GetEntityClassID(weaponEntity);
		if (weaponClassID == 32)
		{
			return true;
		}
	}
	return false;
}

bool IsPrimaryWeapon(const int classID)
{
	for (unsigned char i(0); i < primary.size(); ++i)
	{
		if (classID == primary[i])
		{
			return true;
		}
	}
	return false;
}

bool IsSecondaryWeapon(const int classID)
{
	for (unsigned char i(0); i < secondary.size(); ++i)
	{
		if (classID == secondary[i])
		{
			return true;
		}
	}
	return false;
}

void GetEntityModelName(const int entity, ModelName *modName)
{
	uintptr_t a = MemClass.readMem<uintptr_t>(entity + offsets::m_iModelOffset) + offsets::m_iModelAdd;
	*modName = MemClass.readMem<ModelName>(a);
}

void GetEntityPosition(const int entity, float *pos)
{
	Position p = MemClass.readMem<Position>(entity + m_vecOrigin);
	memcpy(pos, &p, sizeof(Position));
}

void GetPlayerInfo(unsigned char index, Player *player)
{
	player->entity = MemClass.readMem<int>(modules.gameModule + dwEntityList + index * 0x10);
	if (player->entity == 0)
	{
		player->active = false;
		return;
	}
	player->dormant = GetEntityDormant(player->entity);
	if (player->dormant) return;
	player->health = GetEntityHealth(player->entity);
	player->team = GetEntityTeam(player->entity);
	GetEntityLocation(player->entity, &player->location);
	GetEntityName(index, &player->name);
	GetEntityPosition(player->entity, player->position);
}

void GetPlayersInfo(Player *plays)
{
	for (unsigned char i(0); i < MAXPLAYERS; ++i)
	{
		GetPlayerInfo(i, &plays[i]);
	}
}

void GetPlayersInfoString(Player *plays, std::wstring *info)
{
	*info = L"";
	for (unsigned char i(0); i < MAXPLAYERS; ++i)
	{
		if (!plays[i].active) continue;
		if (plays[i].team == val.localPlayerTeam) continue;
		if (plays[i].health < 1) continue;
		*info += L"(";
		*info += std::to_wstring(plays[i].health);
		*info += L"%)";
		*info += plays[i].name;
		*info += L"{";
		*info += plays[i].location;
		*info += L"}";
		if (plays[i].dormant)
		{
			*info += L" - Last Seen";
		}
		*info += L'\n';
	}
	if (info->length() > 0)
	{
		info->pop_back();
	}
}

void GetStringFromClassID(const int classID, std::wstring& name)
{
	name = L"none";
	switch (classID)
	{
		/* Not Shooting */
	case items::knife: // Knife
		name = L"knife";
		break;
	case items::bomb: // bomb (not planted)
		name = L"bomb";
		break;
	case items::bombPlanted: // bomb (planted)
		name = L"bombPlanted";
		break;
	case items::zeus: // Zeus (taser)
		name = L"zeus";
		break;
	case items::healthshot: // Healthshot
		name = L"healthshot";
		break;
	case items::molotov: // Molotov
		name = L"molotov";
		break;
	case items::decoy: // Decoy
		name = L"decoy";
		break;
	case items::flashbang: // Flashbang
		name = L"flashbang";
		break;
	case items::hegrenade: // HEGrenade
		name = L"hegrenade";
		break;
	case items::smoke: // Smoke
		name = L"smoke";
		break;
	case items::incendiary: // Incendiary
		name = L"incendiary";
		break;
	case items::tacticalAwarenessGrenade: // Tactical Awareness Grenade
		name = L"tactical";
		break;
	case items::breachcharge: // Breachcharge
		name = L"breachcharge";
		break;
	case items::axeHammerSpanner: // Axe; Hammer; Spanner (Wrench)
		name = L"axe";
		break;
	/*case items::tablet: // Tablet
		name = L"tablet";
		break;*/
	/*case items::turret: // Turret
		name = L"turret";
		break;*/
	/*case items::amazonDrone: // Amazon Drone
		name = L"drone";
		break;*/
	case items::dangerZoneMoney: // Danger Zone Money
		name = L"dangerZoneMoney";
		break;
	case items::crate: // Loot Crate
		name = L"crate";
		break;
	/*case items::chicken: // Chicken
		name = L"chicken";
		break;*/

			// PISTOLS // 
	case items::glock: // Glock
		name = L"glock";
		break;
	case items::usp: // USP-S; P2000
		name = L"usp";
		break;
	case items::p250: // P250; CZ75
		name = L"p250";
		break;
	case items::duals: // Duals
		name = L"duals";
		break;
	case items::tec9: // TEC-9
		name = L"tec9";
		break;
	case items::fiveseven: // Fiveseven
		name = L"fiveseven";
		break;
	case items::deagle: // Deagle; revolver
		name = L"deagle";
		break;

		// HEAVY //
	case items::mag7: // MAG-7
		name = L"mag7";
		break;
	case items::nova: // Nova
		name = L"nova";
		break;
	case items::sawedoff: // Sawed-Off
		name = L"sawedoff";
		break;
	case items::xm1014: // XM1014
		name = L"xm1014";
		break;
	case items::m249: // M249
		name = L"m249";
		break;
	case items::negev: // Negev
		name = L"negev";
		break;

		// SMGs //
	case items::mac10: // MAC-10
		name = L"mac10";
		break;
	case items::mp9: // MP9
		name = L"mp9";
		break;
	case items::mp7: // MP7
		name = L"mp7";
		break;
	case items::ump: // UMP
		name = L"ump";
		break;
	case items::p90: // P90
		name = L"p90";
		break;
	case items::bizon: // Bizon
		name = L"bizon";
		break;

		// Rifles //
	case items::galil: // Galil
		name = L"gali";
		break;
	case items::famas: // Famas
		name = L"famas";
		break;
	case items::ak47: // AK-47
		name = L"ak47";
		break;
	case items::m4a4: // M4A4; M421-S
		name = L"m4a4";
		break;
	case items::sg553: // SG553
		name = L"sg553";
		break;
	case items::aug: // AUG
		name = L"aug";
		break;

		// SNIPERS //
	case items::scout: // SSG 08 (Scout)
		name = L"scout";
		break;
	case items::awp: // AWP
		name = L"awp";
		break;
	case items::scar20: // SCAR-20
		name = L"scar20";
		break;
	case items::g3sg1: // G3SG1
		name = L"g3sg1";
		break;
	}
}