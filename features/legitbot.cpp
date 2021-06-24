#include "legitbot.hpp"
#include "../config.hpp"
#include "../helpers/math.hpp"

bool Backtrack::IsTickValid(float SimulationTime, float MaxTime)
{
	INetChannelInfo* NetChannelInfo = g_EngineClient->GetNetChannelInfo();
	if (!NetChannelInfo) return true;
	float Correct = 0;
	Correct += NetChannelInfo->GetLatency(FLOW_OUTGOING);
	Correct += NetChannelInfo->GetLatency(FLOW_INCOMING);
	Correct += GetLerpTime();

	std::clamp(Correct, 0.f, g_CVar->FindVar("sv_maxunlag")->GetFloat());

	float DeltaTime = Correct - (g_pGlobalVars->curtime - SimulationTime);

	float TimeLimit = std::clamp(MaxTime, 0.001f, 0.2f);
	

	if (fabsf(DeltaTime) > TimeLimit)
		return false;

	return true;
}
float Backtrack::GetLerpTime()
{
	int ud_rate = g_CVar->FindVar("cl_updaterate")->GetInt();
	ConVar* min_ud_rate = g_CVar->FindVar("sv_minupdaterate");
	ConVar* max_ud_rate = g_CVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = g_CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = g_CVar->FindVar("cl_interp")->GetFloat();
	ConVar* c_min_ratio = g_CVar->FindVar("sv_client_min_interp_ratio");
	ConVar* c_max_ratio = g_CVar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = std::clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return std::max(lerp, (ratio / ud_rate));
}
void Backtrack::Do(CUserCmd* cmd)
{
	if (!g_LocalPlayer || !Variables.aimbot.Backtrack || !Variables.aimbot.BacktrackTime)
		return;

	for (int i = 1; i <= 64; i++)
	{
		C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
		if (!Player ||
			Player->IsDormant() ||
			!Player->IsPlayer() ||
			!Player->IsAlive() ||
			!Player->IsEnemy())
		{
			if (BacktrackRecords[i].size() > 0)
			{
				for (int Tick = 0; Tick < BacktrackRecords[i].size(); Tick++)
				{
					BacktrackRecords[i].erase(BacktrackRecords[i].begin(), BacktrackRecords[i].begin() + Tick);
				}
			}
			continue;
		}

		BacktrackRecords[i].insert(BacktrackRecords[i].begin(), TickInfo2(Player));

		for (auto Tick : BacktrackRecords[i])
			if (!IsTickValid(Tick.SimulationTime, float(Variables.aimbot.BacktrackTime) / 1000.f))
				BacktrackRecords[i].pop_back();
	}

	if (!g_LocalPlayer->IsAlive())
		return;

	int BacktrackedPlayer = -1;
	float MaxPlayerFov = FLT_MAX;

	for (int i = 1; i <= 64; i++)
	{
		C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
		if (!Player ||
			Player->IsDormant() ||
			!Player->IsPlayer() ||
			!Player->IsAlive() ||
			!Player->IsEnemy())
			continue;

		Vector ViewAngles;
		Math::AngleVectors(cmd->viewangles + (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()), ViewAngles);
		float FovDistance = Math::DistancePointToLine(BacktrackRecords[i].front().HeadPosition, g_LocalPlayer->GetEyePos(), ViewAngles);

		if (MaxPlayerFov > FovDistance)
		{
			MaxPlayerFov = FovDistance;
			BacktrackedPlayer = i;
			ClosestPlayer = BacktrackedPlayer;
		}
	}
	ClosestTick = -1;
	float MaxTickFov = FLT_MAX;
	if (BacktrackedPlayer != -1)
	{
		for (int t = 0; t < BacktrackRecords[BacktrackedPlayer].size(); t++)
		{
			Vector ViewAngles2;
			Math::AngleVectors(cmd->viewangles + (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()), ViewAngles2);
			float FovDistance2 = Math::DistancePointToLine(BacktrackRecords[BacktrackedPlayer].at(t).HeadPosition, g_LocalPlayer->GetEyePos(), ViewAngles2);

			if (MaxTickFov > FovDistance2)
			{
				MaxTickFov = FovDistance2;
				ClosestTick = t;
			}
		}
		if (ClosestTick != -1 && BacktrackRecords[BacktrackedPlayer].at(ClosestTick).SimulationTime != -1.f && (cmd->buttons & IN_ATTACK))
			cmd->tick_count = TIME_TO_TICKS(BacktrackRecords[BacktrackedPlayer].at(ClosestTick).SimulationTime + GetLerpTime());

	}
}


namespace Legitbot
{


	bool Enabled = false;
	int AimType = 0;
	float Fov = 0.f;
	float Smooth = 0.f;
	bool Recoil = false;
	int Hitbox = 0;
	Vector HitboxPos = Vector{};


void WeaponConfig(C_BaseCombatWeapon* Weapon)
{
	if (Weapon->IsPistol())
	{
		Enabled = Variables.aimbot.Pistols.Enabled;
		AimType = Variables.aimbot.Pistols.AimType;
		Fov = Variables.aimbot.Pistols.Fov;
		Smooth = Variables.aimbot.Pistols.Smooth;
		Recoil = Variables.aimbot.Pistols.Recoil;
		Hitbox = Variables.aimbot.Pistols.Hitbox;
	}
	else if (Weapon->IsSniper())
	{
		Enabled = Variables.aimbot.Snipers.Enabled;
		if (!g_LocalPlayer->m_bIsScoped()) Enabled = false;
		AimType = Variables.aimbot.Snipers.AimType;
		Fov = Variables.aimbot.Snipers.Fov;
		Smooth = Variables.aimbot.Snipers.Smooth;
		Recoil = Variables.aimbot.Snipers.Recoil;
		Hitbox = Variables.aimbot.Snipers.Hitbox;
	}
	else
	{
		Enabled = Variables.aimbot.Rifles.Enabled;
		AimType = Variables.aimbot.Rifles.AimType;
		Fov = Variables.aimbot.Rifles.Fov;
		Smooth = Variables.aimbot.Rifles.Smooth;
		Recoil = Variables.aimbot.Rifles.Recoil;
		Hitbox = Variables.aimbot.Rifles.Hitbox;
	}
}
void Do(CUserCmd* cmd, C_BaseCombatWeapon* Weapon)
{
	if (!g_LocalPlayer ||
		!g_LocalPlayer->IsAlive() ||
		!Weapon ||
		Weapon->IsKnife() ||
		Weapon->IsGrenade() ||
		!(cmd->buttons & IN_ATTACK))
		return;

	WeaponConfig(Weapon);

	if (!Enabled)
		return;

	float MaxPlayerFov = Fov;
	Vector AimAngle = Vector{};
	Vector ViewAngle = Vector{};
	g_EngineClient->GetViewAngles(ViewAngle);
	Vector EyePos = g_LocalPlayer->GetEyePos();
	int ClosestPlayerIndex = -1;

	for (int i = 1; i <= 64; i++)
	{
		C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
		if (!Player ||
			!Player->IsPlayer() ||
			Player->IsDormant() ||
			!Player->IsAlive() ||
			!Player->IsEnemy())
			continue;

		matrix3x4_t BoneMatrix[128];
		if (!Player->SetupBones(BoneMatrix, 128, BONE_USED_BY_HITBOX, g_pGlobalVars->curtime))
			continue;

		Vector CurHitbox = Player->GetHitboxPos((Hitbox == 3) ?
				GetClosestHitbox(Player, BoneMatrix, EyePos)
				: GetHitboxFromInt(Hitbox), BoneMatrix);

		float FovDistance = Math::GetFOV(ViewAngle + (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()), Math::CalcAngle(EyePos, CurHitbox));
		if (MaxPlayerFov > FovDistance)
		{
			if (g_LocalPlayer->CanSeePlayer(Player,CurHitbox))
			{
				MaxPlayerFov = FovDistance;
				ClosestPlayerIndex = i;
				HitboxPos = CurHitbox;
			}
		}
	}
	if (ClosestPlayerIndex != -1)
	{
		C_BasePlayer* ClosestPlayer = C_BasePlayer::GetPlayerByIndex(ClosestPlayerIndex);
		if (!ClosestPlayer) return;
		AimAngle = Math::CalcAngle(EyePos, HitboxPos);
		Math::SanitizeAngles(AimAngle);
		if (Recoil)
		{
			Vector RecoilAngle = g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat();
			Math::SanitizeAngles(RecoilAngle);
			AimAngle -= RecoilAngle;
		}
		Math::SanitizeAngles(ViewAngle);
		Vector DeltaAngle = ViewAngle - AimAngle;
		Math::SanitizeAngles(DeltaAngle);
		float Smoothing = (AimType == 1 && Smooth > 1) ? Smooth : 1;
		Vector FinalAngle = ViewAngle - DeltaAngle / Smoothing;
		if (!Math::SanitizeAngles(FinalAngle))
			return;
		cmd->viewangles = FinalAngle;
		if (AimType != 2)
			g_EngineClient->SetViewAngles(cmd->viewangles);
	}
}
int GetClosestHitbox(C_BasePlayer* Player, matrix3x4_t* BoneMatrix, Vector EyePos)
{
	Vector ViewAngle = Vector{};
	g_EngineClient->GetViewAngles(ViewAngle);
	float MaxFov = Fov;
	int ClosestHitbox = 0;
	for (int i = 0; i <= 6; i++)
	{
		float FovDistance = Math::GetFOV(ViewAngle + (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()), Math::CalcAngle(EyePos, Player->GetHitboxPos(i, BoneMatrix)));

		if (MaxFov > FovDistance)
		{
			MaxFov = FovDistance;
			ClosestHitbox = i;
		}
	}
	return ClosestHitbox;
}
int GetHitboxFromInt(int Hitbox)
{
	if (Hitbox != HITBOX_HEAD && Hitbox != HITBOX_NECK)
		return HITBOX_CHEST;

	return Hitbox;
}
}