#pragma once
#include "../valve_sdk/csgostructs.hpp"
#include "../singleton.hpp"
struct TickInfo2
{
	TickInfo2()
	{
		HeadPosition.Init();
		SimulationTime = -1.f;
		MatrixBuilt = false;
	}
	TickInfo2(C_BasePlayer* Player)
	{
		HeadPosition = Player->GetHitboxPos(HITBOX_HEAD);
		SimulationTime = Player->m_flSimulationTime();
		MatrixBuilt = false;
		if (Player->SetupBones(BoneMatrix, 128, BONE_USED_BY_ANYTHING, g_pGlobalVars->curtime))
			MatrixBuilt = true;
	}
	Vector HeadPosition;
	float SimulationTime;

	bool MatrixBuilt;
	matrix3x4_t BoneMatrix[128];
};
class Backtrack :
	public Singleton<Backtrack>
{
	friend class Singleton<Backtrack>;
private:

public:
	bool IsTickValid(float SimulationTime, float MaxTime);
	float GetLerpTime();
	void Do(CUserCmd* cmd);
	std::vector<TickInfo2> BacktrackRecords[65];
	int ClosestPlayer;
	int ClosestTick;
};

namespace Legitbot
{
	extern bool Enabled;
	extern int AimType;
	extern float Fov;
	extern float Smooth;
	extern bool Recoil;
	extern int Hitbox;
	extern Vector HitboxPos;

	void WeaponConfig(C_BaseCombatWeapon* Weapon);
	int GetHitboxFromInt(int Hitbox);
	int GetClosestHitbox(C_BasePlayer* Player, matrix3x4_t* BoneMatrix, Vector EyePos);

	void Do(CUserCmd* cmd, C_BaseCombatWeapon* Weapon);
}



