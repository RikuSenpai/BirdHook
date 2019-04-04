#pragma once
#include <cstdint>
#include <vcruntime.h>

namespace items
{
	constexpr ::std::ptrdiff_t knife = 105;
	constexpr ::std::ptrdiff_t bomb = 32;
	constexpr ::std::ptrdiff_t bombPlanted = 126;
	constexpr ::std::ptrdiff_t zeus = 263;
	constexpr ::std::ptrdiff_t healthshot = 102;
	constexpr ::std::ptrdiff_t molotov = 110;
	constexpr ::std::ptrdiff_t decoy = 45;
	constexpr ::std::ptrdiff_t flashbang = 75;
	constexpr ::std::ptrdiff_t hegrenade = 94;
	constexpr ::std::ptrdiff_t smoke = 152;
	constexpr ::std::ptrdiff_t incendiary = 97;
	constexpr ::std::ptrdiff_t tacticalAwarenessGrenade = 148;
	constexpr ::std::ptrdiff_t breachcharge = 28;
	constexpr ::std::ptrdiff_t axeHammerSpanner = 109;
	constexpr ::std::ptrdiff_t tablet = 168;
	constexpr ::std::ptrdiff_t turret = 48;
	constexpr ::std::ptrdiff_t amazonDrone = 47;
	constexpr ::std::ptrdiff_t dangerZoneMoney = 103;
	constexpr ::std::ptrdiff_t crate = 123;
	constexpr ::std::ptrdiff_t chicken = 34;
	constexpr ::std::ptrdiff_t glock = 241;
	constexpr ::std::ptrdiff_t usp = 242;
	constexpr ::std::ptrdiff_t p250 = 254;
	constexpr ::std::ptrdiff_t duals = 235;
	constexpr ::std::ptrdiff_t tec9 = 264;
	constexpr ::std::ptrdiff_t fiveseven = 237;
	constexpr ::std::ptrdiff_t deagle = 44;
	constexpr ::std::ptrdiff_t mag7 = 247;
	constexpr ::std::ptrdiff_t nova = 252;
	constexpr ::std::ptrdiff_t sawedoff = 256;
	constexpr ::std::ptrdiff_t xm1014 = 268;
	constexpr ::std::ptrdiff_t m249 = 243;
	constexpr ::std::ptrdiff_t negev = 251;
	constexpr ::std::ptrdiff_t mac10 = 246;
	constexpr ::std::ptrdiff_t mp9 = 250;
	constexpr ::std::ptrdiff_t mp7 = 249;
	constexpr ::std::ptrdiff_t ump = 266;
	constexpr ::std::ptrdiff_t p90 = 255;
	constexpr ::std::ptrdiff_t bizon = 231;
	constexpr ::std::ptrdiff_t galil = 240;
	constexpr ::std::ptrdiff_t famas = 236;
	constexpr ::std::ptrdiff_t ak47 = 1;
	constexpr ::std::ptrdiff_t m4a4 = 245;
	constexpr ::std::ptrdiff_t sg553 = 261;
	constexpr ::std::ptrdiff_t aug = 228;
	constexpr ::std::ptrdiff_t scout = 262;
	constexpr ::std::ptrdiff_t awp = 229;
	constexpr ::std::ptrdiff_t scar20 = 257;
	constexpr ::std::ptrdiff_t g3sg1 = 238;

	constexpr ::std::ptrdiff_t baseGrenadeProjectile = 9;
	constexpr ::std::ptrdiff_t breachChargeProjectile = 29;
	constexpr ::std::ptrdiff_t decoyProjectile = 46;
	constexpr ::std::ptrdiff_t molotovProjectile = 111;
	constexpr ::std::ptrdiff_t sensorGrenadeProjectile = 149;
	constexpr ::std::ptrdiff_t smokeGrenadeProjectile = 153;

	constexpr char fragGrenadeDroppedModelName[] = "w_eq_fraggrenade_dropped.mdl";
	constexpr char flashbangDroppedModelName[] = "w_eq_flashbang_dropped.mdl";
};

namespace offsets
{
	/*
	uintptr_t one = MemClass.readMem<uintptr_t>(modules.gameModule + dwRadarBase);
	uintptr_t two = MemClass.readMem<uintptr_t>(one + m_iRadarBasePtr);
	Name nam = MemClass.readMem<Name>(two + m_iRadarStructPos + (index + 2) * m_iRadarStructSize);
	*/
	constexpr ::std::ptrdiff_t m_iRadarBasePtr = 0x74;
	constexpr ::std::ptrdiff_t m_iRadarStructSize = 0x168;
	constexpr ::std::ptrdiff_t m_iRadarStructPos = 0x18;
	/*
	uintptr_t o = MemClass.readMem<uintptr_t>(entity + m_iModelOffset) + m_iModelAdd;
	Name nam = MemClass.readMem<Name>(o);
	*/
	constexpr ::std::ptrdiff_t m_iModelOffset = 0x6C;
	constexpr ::std::ptrdiff_t m_iModelAdd = 0x13;
	/*
	client.dll + lPlayer, offset = m_iTotalHitsOnServer
	*/
	constexpr ::std::ptrdiff_t m_iTotalHitsOnServer = 0xA388;
};