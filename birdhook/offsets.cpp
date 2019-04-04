#include "main.h"
#include "csgo\netvars.h"

Ofs* ofs = new Ofs();

std::string Ofs::toHex(DWORD offset) {
    std::stringstream ss;
    ss << std::hex << offset;
    std::string x = ss.str();
    for (auto &c : x) c = toupper(c);

    return x;
}

void Ofs::FindOffsets() {
	if (!c_netvars::get().init())
	{
		printf("Can't initialize netvar manager!");
		return;
	}

	if (cfg->debugEnable)
		c_netvars::get().dump();

	static auto find_ptr = [](Module* mod, const char* sig, DWORD sig_add, DWORD off_add, bool sub_base = true) -> DWORD
	{
		auto off = mem->FindPattern(mod, sig);
		auto sb = sub_base ? mod->GetImage() : 0;
		off = mem->Read<DWORD>(off + sig_add);
		
		return (!off ? 0 : off + off_add - sb);
	};


	m_dwClientState = find_ptr(engine, "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0", 0x1, 0);
	m_dwViewAngles = find_ptr(engine, "F3 0F 11 80 ? ? ? ? D9 46 04 D9 05", 0x4, 0, false);
	m_dwInGame = find_ptr(engine, "83 B8 ? ? ? ? 06 0F 94 C0 C3", 0x2, 0, false);
	//assert(m_dwInGame);
	m_nDeltaTick = find_ptr(engine, "83 BE ? ? ? ? FF 74 25 8D 4E 08 E8", 0x2, 0, false);
	//assert(m_nDeltaTick);
	m_dwLocalPlayer = find_ptr(client, "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF", 0x3, 0x4);
	m_dwEntityList = find_ptr(client, "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", 0x1, 0);
	m_dwGlowObject = find_ptr(client, "A1 ? ? ? ? A8 01 75 4B", 0x1, 0x4);
	m_dwForceJump = find_ptr(client, "8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02", 0x2, 0);
	m_dwForceAttack = find_ptr(client, "89 0D ? ? ? ? 8B 0D ? ? ? ? 8B F2 8B C1 83 CE 04", 0x2, 0);
	m_bDormant = find_ptr(client, "8A 81 ? ? ? ? C3 32 C0", 0x2, 0x8, false);
	m_dwGlobalVars = find_ptr(engine, "68 ? ? ? ? 68 ? ? ? ? FF 50 08 85 C0", 0x1, 0);
	m_dwPlayerResource = find_ptr(client, "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7", 0x2, 0);

	m_hActiveWeapon = GET_NETVAR("DT_BaseCombatCharacter", "m_hActiveWeapon");
	m_iCrossHairID = GET_NETVAR("DT_CSPlayer", "m_bHasDefuser") + 0x5C;
	m_iHealth = GET_NETVAR("DT_BasePlayer", "m_iHealth");
	m_iTeamNum = GET_NETVAR("DT_BaseEntity", "m_iTeamNum");
	m_iShotsFired = GET_NETVAR("DT_CSPlayer", "m_iShotsFired");
	m_fFlags = GET_NETVAR("DT_BasePlayer", "m_fFlags");
	m_lifeState = GET_NETVAR("DT_BasePlayer", "m_lifeState");
	m_bSpottedByMask = GET_NETVAR("DT_BaseEntity", "m_bSpottedByMask");
	m_vecOrigin = GET_NETVAR("DT_BaseEntity", "m_vecOrigin");
	m_vecViewOffset = GET_NETVAR("DT_BasePlayer", "m_vecViewOffset[0]");
	m_angEyeAngles = GET_NETVAR("DT_CSPlayer", "m_angEyeAngles");
	m_vecVelocity = GET_NETVAR("DT_BasePlayer", "m_vecVelocity[0]");
	m_aimPunchAngle = GET_NETVAR("DT_CSPlayer", "m_aimPunchAngle");
	m_dwBoneMatrix = GET_NETVAR("DT_BaseAnimating", "m_nForceBone") + 0x1C;
	m_flNextPrimaryAttack = GET_NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	m_nTickBase = GET_NETVAR("DT_BasePlayer", "m_nTickBase");
	m_iPlayerC4 = GET_NETVAR("DT_CSPlayerResource", "m_iPlayerC4");
	m_bIsDefusing = GET_NETVAR("DT_CSPlayer", "m_bIsDefusing");
	m_iItemDefinitionIndex = GET_NETVAR("DT_BaseCombatWeapon", "m_iItemDefinitionIndex");
	m_iWorldModelIndex = GET_NETVAR("DT_BaseCombatWeapon", "m_iWorldModelIndex");
	m_nModelIndex = GET_NETVAR("DT_BaseCombatWeapon", "m_nModelIndex");
	m_hViewModel = GET_NETVAR("DT_CSPlayer", "m_hViewModel[0]");
	m_iAccountID = GET_NETVAR("DT_BaseCombatWeapon", "m_iAccountID");
	m_nFallbackPaintKit = GET_NETVAR("DT_BaseCombatWeapon", "m_nFallbackPaintKit");
	m_OriginalOwnerXuidLow = GET_NETVAR("DT_BaseCombatWeapon", "m_OriginalOwnerXuidLow");
	m_iItemIDLow = GET_NETVAR("DT_BaseCombatWeapon", "m_iItemIDLow");
	m_hMyWeapons = GET_NETVAR("DT_BaseCombatCharacter", "m_hMyWeapons");
	m_dwIndex = 0x64;

}

bool Ofs::CheckOffsets()
{
#define CHECK_OFFSET(n) \
	if (cfg->debugEnable) \
		printf("%s = %X\n", #n, n); \
	if (!n) { \
		result = false; }

	bool result = true;

	CHECK_OFFSET(m_dwIndex);

	CHECK_OFFSET(m_dwClientState);
	CHECK_OFFSET(m_dwViewAngles);
	CHECK_OFFSET(m_dwInGame);
	CHECK_OFFSET(m_nDeltaTick);
	CHECK_OFFSET(m_dwLocalPlayer);
	CHECK_OFFSET(m_dwEntityList);
	CHECK_OFFSET(m_dwGlowObject);
	CHECK_OFFSET(m_dwForceJump);
	CHECK_OFFSET(m_dwForceAttack);
	CHECK_OFFSET(m_bDormant);
	CHECK_OFFSET(m_dwGlobalVars);
	CHECK_OFFSET(m_dwPlayerResource);

	CHECK_OFFSET(m_hActiveWeapon);
	CHECK_OFFSET(m_iCrossHairID);
	CHECK_OFFSET(m_iHealth);
	CHECK_OFFSET(m_iTeamNum);
	CHECK_OFFSET(m_iShotsFired);
	CHECK_OFFSET(m_fFlags);
	CHECK_OFFSET(m_lifeState);
	CHECK_OFFSET(m_bSpottedByMask);
	CHECK_OFFSET(m_vecOrigin);
	CHECK_OFFSET(m_vecViewOffset);
	CHECK_OFFSET(m_angEyeAngles);
	CHECK_OFFSET(m_vecVelocity);
	CHECK_OFFSET(m_aimPunchAngle);
	CHECK_OFFSET(m_dwBoneMatrix);
	CHECK_OFFSET(m_flNextPrimaryAttack);
	CHECK_OFFSET(m_nTickBase);
	CHECK_OFFSET(m_iPlayerC4);
	CHECK_OFFSET(m_bIsDefusing);
	CHECK_OFFSET(m_iItemDefinitionIndex);
	CHECK_OFFSET(m_iWorldModelIndex);
	CHECK_OFFSET(m_nModelIndex);
	CHECK_OFFSET(m_hViewModel);
	CHECK_OFFSET(m_iAccountID);
	CHECK_OFFSET(m_nFallbackPaintKit);
	CHECK_OFFSET(m_OriginalOwnerXuidLow);
	CHECK_OFFSET(m_iItemIDLow);
	CHECK_OFFSET(m_hMyWeapons);

	return result;
}

void Ofs::getOffsets() {
	FindOffsets();
	
	if (!CheckOffsets())
	{
		printf("Offsets outdated!\n");
		std::system("pause");
		exit(1);
	}
	//std::system("pause");
	Sleep(1000);

	m_dwClientState = mem->Read<DWORD_PTR>(engine->GetImage() + m_dwClientState);
}
