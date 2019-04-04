#include <Windows.h>
#include <iostream>

#include "Game.h"
#include "Glow.h"
#include "MemMan.h"
#include "csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

ModelName modName;

void GlowHack::SetPlayerGlow(CGlow& glow, const int& entity)
{
	int entityHealth = GetEntityHealth(entity);
	if (val.dz)  // DZ Enabled
	{
		Glow.r = entityHealth / -val.maxDzHealth + 1;
		Glow.g = entityHealth / val.maxDzHealth;
		Glow.b = 0.0f;
		Glow.a = 1.0f;
		return;
	}

	int entityTeam = GetEntityTeam(entity);

	if (val.localPlayerTeam == entityTeam)
	{
		Glow.r = 0.0f;
		Glow.g = 0.0f;
		Glow.b = 1.0f;
		Glow.a = 1.0f;
	}
	else
	{
		if (GetAsyncKeyState(hackSwitchKeys.bomberKey) == -32768)
		{
			if (entityTeam == 3 && val.localPlayerTeam == 2)
			{
				if (EntityIsDefusing(entity))
				{
					if (EntityHasDefuser(entity))
					{
						Glow.r = 0.0f;
						Glow.g = 0.0f;
						Glow.b = 1.0f;
						Glow.a = 1.0f;
					}
					else
					{
						Glow.r = 1.0f;
						Glow.g = 1.0f;
						Glow.b = 1.0f;
						Glow.a = 1.0f;
					}
					return;
				}
			}
			else if (entityTeam == 2 && val.localPlayerTeam == 3)
			{
				if (EntityCarriesBomb(entity))
				{
					Glow.r = 1.0f;
					Glow.g = 1.0f;
					Glow.b = 1.0f;
					Glow.a = 1.0f;
					return;
				}
			}
		}
		Glow.r = entityHealth / -val.maxHealth + 1;
		Glow.g = entityHealth / val.maxHealth;
		Glow.b = 0.0f;
		Glow.a = 1.0f;
	}
}

void GlowHack::GlowOn()
{
	for (int i = 0; i < val.glowCount; ++i)
	{
		int entity = MemClass.readMem<int>(objects.glowObject + 0x38 * i);

		if (entity == 0) continue;

		Glow = MemClass.readMem<CGlow>(objects.glowObject + (i * 0x38) + 0x4);

		Glow.m_bRenderWhenOccluded = true;
		Glow.m_bRenderWhenUnoccluded = false;

		int entityClassID = GetEntityClassID(entity);
		switch (entityClassID)
		{
		case 38:  // Player
			SetPlayerGlow(Glow, entity);
			break;
		default:  // Sth else
			SetWeaponGlow(Glow, entity, entityClassID);
			break;
		}
		MemClass.writeMem<CGlow>(objects.glowObject + (i * 0x38) + 0x4, Glow);
	}
}

void GlowHack::GlowOff()
{
	for (int i = 0; i < val.glowCount; ++i)
	{
		int entity = MemClass.readMem<int>(objects.glowObject + 0x38 * i);

		if (entity == 0) continue;

		Glow = MemClass.readMem<CGlow>(objects.glowObject + (i * 0x38) + 0x4);

		Glow.r = 0.0f;
		Glow.g = 0.0f;
		Glow.b = 0.0f;
		Glow.a = 0.0f;
		Glow.m_bRenderWhenOccluded = false;

		MemClass.writeMem<CGlow>(objects.glowObject + (i * 0x38) + 0x4, Glow);
	}
}

void GlowHack::SetWeaponGlow(CGlow& glow, const int& entity, const int& classID)
{
	switch (classID)
	{
	case items::decoyProjectile:
		Glow.r = 1.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 1.0f;
		break;
	case items::molotovProjectile:
		Glow.r = 0.5f;
		Glow.g = 0.5f;
		Glow.b = 0.0f;
		Glow.a = 1.0f;
		break;
	case items::smokeGrenadeProjectile:
		Glow.r = 1.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 1.0f;
		break;
	case items::breachChargeProjectile:
		Glow.r = 1.0f;
		Glow.g = 0.0f;
		Glow.b = 0.0f;
		Glow.a = 1.0f;
		break;
	case items::sensorGrenadeProjectile:
		Glow.r = 1.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 1.0f;
		break;
	case items::baseGrenadeProjectile:
		GetEntityModelName(entity, &modName);
		if (!strcmp(modName.modelName, items::fragGrenadeDroppedModelName))
		{
			Glow.r = 1.0f;
			Glow.g = 0.0f;
			Glow.b = 0.0f;
			Glow.a = 1.0f;
			break;
		}
		else
		{
			Glow.r = 0.0f;
			Glow.g = 0.0f;
			Glow.b = 1.0f;
			Glow.a = 1.0f;
			break;
		}
		/* Not Shooting */
	case items::knife: // Knife
		Glow.r = 1.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 0.5f;
		break;
	case items::bomb: // bomb (not planted)
		Glow.r = 1.0f;
		Glow.g = 0.0f;
		Glow.b = 0.0f;
		Glow.a = 1.0f;
		break;
	case items::bombPlanted: // bomb (planted)
		Glow.r = 1.0f;
		Glow.g = 0.0f;
		Glow.b = 0.0f;
		Glow.a = 1.0f;
		break;
	case items::zeus: // Zeus (taser)
		Glow.r = 1.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 0.5f;
		break;
	case items::healthshot: // Healthshot
		Glow.r = 0.0f;
		Glow.g = 0.0f;
		Glow.b = 1.0f;
		Glow.a = 1.0f;
		break;
	case items::molotov: // Molotov
		Glow.r = 1.0f;
		Glow.g = 0.5f;
		Glow.b = 0.0f;
		Glow.a = 0.5f;
		break;
	case items::decoy: // Decoy
		Glow.r = 1.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 0.5f;
		break;
	case items::flashbang: // Flashbang
		Glow.r = 0.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 0.5f;
		break;
	case items::hegrenade: // HEGrenade
		Glow.r = 1.0f;
		Glow.g = 0.0f;
		Glow.b = 0.0f;
		Glow.a = 0.5f;
		break;
	case items::smoke: // Smoke
		Glow.r = 1.0f;
		Glow.g = 0.0f;
		Glow.b = 1.0f;
		Glow.a = 0.5f;
		break;
	case items::incendiary: // Incendiary
		Glow.r = 1.0f;
		Glow.g = 0.5f;
		Glow.b = 0.0f;
		Glow.a = 0.5f;
		break;
	case items::tacticalAwarenessGrenade: // Tactical Awareness Grenade
		Glow.r = 0.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 0.5f;
		break;
	case items::breachcharge: // Breachcharge
		Glow.r = 1.0f;
		Glow.g = 0.0f;
		Glow.b = 0.0f;
		Glow.a = 0.5f;
		break;
	case items::axeHammerSpanner: // Axe; Hammer; Spanner (Wrench)
		Glow.r = 1.0f;
		Glow.g = 1.0f;
		Glow.b = 1.0f;
		Glow.a = 0.5f;
		break;
		/*case items::tablet: // Tablet
			Glow.r = 1.0f;
			Glow.g = 1.0f;
			Glow.b = 1.0f;
			Glow.a = 0.5f;
			break;*/
			/*case items::turret: // Turret
				Glow.r = 1.0f;
				Glow.g = 0.0f;
				Glow.b = 0.0f;
				Glow.a = 1.0f;
				break;*/
				/*case items::amazonDrone: // Amazon Drone
					Glow.r = 0.0f;
					Glow.g = 1.0f;
					Glow.b = 0.0f;
					Glow.a = 1.0f;
					break;*/
	case items::dangerZoneMoney: // Danger Zone Money
		Glow.r = 0.0f;
		Glow.g = 1.0f;
		Glow.b = 0.0f;
		Glow.a = 1.0f;
		break;
	case items::crate: // Loot Crate
		Glow.r = 0.0f;
		Glow.g = 0.0f;
		Glow.b = 1.0f;
		Glow.a = 0.7f;
		break;
		/*case items::chicken: // Chicken
			Glow.r = 1.0f;
			Glow.g = 1.0f;
			Glow.b = 1.0f;
			Glow.a = 1.0f;
			break;*/

			// PISTOLS // 
	case items::glock: // Glock
		Glow.r = pistolGlow.r;
		Glow.g = pistolGlow.g;
		Glow.b = pistolGlow.b;
		Glow.a = pistolGlow.a;
		break;
	case items::usp: // USP-S; P2000
		Glow.r = pistolGlow.r;
		Glow.g = pistolGlow.g;
		Glow.b = pistolGlow.b;
		Glow.a = pistolGlow.a;
		break;
	case items::p250: // P250; CZ75
		Glow.r = pistolGlow.r;
		Glow.g = pistolGlow.g;
		Glow.b = pistolGlow.b;
		Glow.a = pistolGlow.a;
		break;
	case items::duals: // Duals
		Glow.r = pistolGlow.r;
		Glow.g = pistolGlow.g;
		Glow.b = pistolGlow.b;
		Glow.a = pistolGlow.a;
		break;
	case items::tec9: // TEC-9
		Glow.r = pistolGlow.r;
		Glow.g = pistolGlow.g;
		Glow.b = pistolGlow.b;
		Glow.a = pistolGlow.a;
		break;
	case items::fiveseven: // Fiveseven
		Glow.r = pistolGlow.r;
		Glow.g = pistolGlow.g;
		Glow.b = pistolGlow.b;
		Glow.a = pistolGlow.a;
		break;
	case items::deagle: // Deagle; revolver
		Glow.r = pistolGlow.r;
		Glow.g = pistolGlow.g;
		Glow.b = pistolGlow.b;
		Glow.a = pistolGlow.a;
		break;

		// HEAVY //
	case items::mag7: // MAG-7
		Glow.r = heavyGlow.r;
		Glow.g = heavyGlow.g;
		Glow.b = heavyGlow.b;
		Glow.a = heavyGlow.a;
		break;
	case items::nova: // Nova
		Glow.r = heavyGlow.r;
		Glow.g = heavyGlow.g;
		Glow.b = heavyGlow.b;
		Glow.a = heavyGlow.a;
		break;
	case items::sawedoff: // Sawed-Off
		Glow.r = heavyGlow.r;
		Glow.g = heavyGlow.g;
		Glow.b = heavyGlow.b;
		Glow.a = heavyGlow.a;
		break;
	case items::xm1014: // XM1014
		Glow.r = heavyGlow.r;
		Glow.g = heavyGlow.g;
		Glow.b = heavyGlow.b;
		Glow.a = heavyGlow.a;
		break;
	case items::m249: // M249
		Glow.r = heavyGlow.r;
		Glow.g = heavyGlow.g;
		Glow.b = heavyGlow.b;
		Glow.a = heavyGlow.a;
		break;
	case items::negev: // Negev
		Glow.r = heavyGlow.r;
		Glow.g = heavyGlow.g;
		Glow.b = heavyGlow.b;
		Glow.a = heavyGlow.a;
		break;

		// SMGs //
	case items::mac10: // MAC-10
		Glow.r = smgGlow.r;
		Glow.g = smgGlow.g;
		Glow.b = smgGlow.b;
		Glow.a = smgGlow.a;
		break;
	case items::mp9: // MP9
		Glow.r = smgGlow.r;
		Glow.g = smgGlow.g;
		Glow.b = smgGlow.b;
		Glow.a = smgGlow.a;
		break;
	case items::mp7: // MP7
		Glow.r = smgGlow.r;
		Glow.g = smgGlow.g;
		Glow.b = smgGlow.b;
		Glow.a = smgGlow.a;
		break;
	case items::ump: // UMP
		Glow.r = smgGlow.r;
		Glow.g = smgGlow.g;
		Glow.b = smgGlow.b;
		Glow.a = smgGlow.a;
		break;
	case items::p90: // P90
		Glow.r = smgGlow.r;
		Glow.g = smgGlow.g;
		Glow.b = smgGlow.b;
		Glow.a = smgGlow.a;
		break;
	case items::bizon: // Bizon
		Glow.r = smgGlow.r;
		Glow.g = smgGlow.g;
		Glow.b = smgGlow.b;
		Glow.a = smgGlow.a;
		break;

		// Rifles //
	case items::galil: // Galil
		Glow.r = rifleGlow.r;
		Glow.g = rifleGlow.g;
		Glow.b = rifleGlow.b;
		Glow.a = rifleGlow.a;
		break;
	case items::famas: // Famas
		Glow.r = rifleGlow.r;
		Glow.g = rifleGlow.g;
		Glow.b = rifleGlow.b;
		Glow.a = rifleGlow.a;
		break;
	case items::ak47: // AK-47
		Glow.r = rifleGlow.r;
		Glow.g = rifleGlow.g;
		Glow.b = rifleGlow.b;
		Glow.a = rifleGlow.a;
		break;
	case items::m4a4: // M4A4; M421-S
		Glow.r = rifleGlow.r;
		Glow.g = rifleGlow.g;
		Glow.b = rifleGlow.b;
		Glow.a = rifleGlow.a;
		break;
	case items::sg553: // SG553
		Glow.r = rifleGlow.r;
		Glow.g = rifleGlow.g;
		Glow.b = rifleGlow.b;
		Glow.a = rifleGlow.a;
		break;
	case items::aug: // AUG
		Glow.r = rifleGlow.r;
		Glow.g = rifleGlow.g;
		Glow.b = rifleGlow.b;
		Glow.a = rifleGlow.a;
		break;

		// SNIPERS //
	case items::scout: // SSG 08 (Scout)
		Glow.r = sniperGlow.r;
		Glow.g = sniperGlow.g;
		Glow.b = sniperGlow.b;
		Glow.a = sniperGlow.a;
		break;
	case items::awp: // AWP
		Glow.r = sniperGlow.r;
		Glow.g = sniperGlow.g;
		Glow.b = sniperGlow.b;
		Glow.a = sniperGlow.a;
		break;
	case items::scar20: // SCAR-20
		Glow.r = sniperGlow.r;
		Glow.g = sniperGlow.g;
		Glow.b = sniperGlow.b;
		Glow.a = sniperGlow.a;
		break;
	case items::g3sg1: // G3SG1
		Glow.r = sniperGlow.r;
		Glow.g = sniperGlow.g;
		Glow.b = sniperGlow.b;
		Glow.a = sniperGlow.a;
		break;
	default:
		Glow.m_bRenderWhenOccluded = false;
		break;
	}
}