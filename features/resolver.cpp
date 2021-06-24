#include "resolver.hpp"
#include "../globals/globals.hpp"
#include "autowall.hpp"
#include <iostream>
#include "lagcomp.h"
int shots_missed[65];
int shots_hit[65];
int shots_fire[65];
bool missed3shots = false;
bool missed2shots = false;

float printvalueeyeyaw;
float printvaluegoalfeetyaw;
float printvalueeyeangles;
float printabsrotation;
float printlby;

struct
{
	float lean_yaw[65];
	float body_yaw[65];
	float abs_ang[65];
	float yaw[65];
	float lby_yaw[65];
	float eye_yaw[65];
	float goal_feet_yaw[65];
	float lby_delta[65];
	float jitter_delta[65];
	float abs[65];
	float first_value[65];
	float second_value[65];
	float final_value[65];
	bool using_small_movement[65];
	bool moving[65];
	bool jittering[65];
	bool on_ground[65];
	bool left[65];
	bool right[65];
	bool legit[65];
	bool crouching[65];
	float choked_packets[65];
	float last_value;
}data;

namespace Resolver
{
	void ABSROTATION(C_BasePlayer* player)
	{
		for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
		{
			C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(i);
			if (!player || !player->IsAlive() || player->IsDormant() || player == g_LocalPlayer)
				continue;

			auto feet_yaw = player->GetAnimOverlay(3)->m_flCycle > 0.9f && player->GetAnimOverlay(3)->m_flWeight > 0.9f && player->m_vecVelocity().Length2D() < 0.1f;
			auto body_max_rotation = Variables.rageaimbot.body_max_rotation;
			if (feet_yaw <= 60)
			{
				if (-60 > feet_yaw)
					player->m_angEyeAngles().y = body_max_rotation + player->m_angEyeAngles().y;
			}
			else
			{
				player->m_angEyeAngles().y = body_max_rotation - player->m_angEyeAngles().y;
			}
			if (player->GetAnimOverlay(3)->m_flCycle > 0.9)
			{
				for (int resolve_delta = 60.f; resolve_delta < -60.f; resolve_delta = resolve_delta - 20.f)
				{
					player->m_angEyeAngles().y = resolve_delta;
				}
			}
		}

	}
	void AnimationFix(C_BasePlayer* entity)
	{
		float m_flRealtime = g_pGlobalVars->realtime;
		float m_flCurtime = g_pGlobalVars->curtime;
		float m_flFrametime = g_pGlobalVars->frametime;
		float m_flAbsFrametime = g_pGlobalVars->absoluteframetime;
		float m_flInterpolation = g_pGlobalVars->interpolation_amount;
		float m_nFrames = g_pGlobalVars->framecount;
		float m_nTicks = g_pGlobalVars->tickcount;

		int m_iNextSimulationTick = g_LocalPlayer->m_flSimulationTime() / g_pGlobalVars->interval_per_tick + 1;

		g_pGlobalVars->realtime = g_LocalPlayer->m_flSimulationTime();
		g_pGlobalVars->curtime = g_LocalPlayer->m_flSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;
		g_pGlobalVars->absoluteframetime = g_pGlobalVars->interval_per_tick;
		g_pGlobalVars->framecount = m_iNextSimulationTick;
		g_pGlobalVars->tickcount = m_iNextSimulationTick;
		g_pGlobalVars->interpolation_amount = 0.f;

		if (entity->GetPlayerAnimState()->m_iLastClientSideAnimationUpdateFramecount >= m_iNextSimulationTick)
			entity->GetPlayerAnimState()->m_iLastClientSideAnimationUpdateFramecount = m_iNextSimulationTick - 1;

		entity->ClientAnimations() = true;
		entity->UpdateClientSideAnimation();

		g_pGlobalVars->realtime = m_flRealtime;
		g_pGlobalVars->curtime = m_flCurtime;
		g_pGlobalVars->frametime = m_flFrametime;
		g_pGlobalVars->absoluteframetime = m_flAbsFrametime;
		g_pGlobalVars->interpolation_amount = m_flInterpolation;
		g_pGlobalVars->frametime = m_nFrames;
		g_pGlobalVars->tickcount = m_nTicks;  

		g_pGlobalVars->realtime = g_LocalPlayer->m_flSimulationTime();
		g_pGlobalVars->curtime = g_LocalPlayer->m_flSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;
		g_pGlobalVars->absoluteframetime = g_pGlobalVars->interval_per_tick;
		g_pGlobalVars->framecount = m_iNextSimulationTick;
		g_pGlobalVars->tickcount = m_iNextSimulationTick;
		g_pGlobalVars->interpolation_amount = 0.f;

		if (entity->GetPlayerAnimState()->m_iLastClientSideAnimationUpdateFramecount >= m_iNextSimulationTick)
			entity->GetPlayerAnimState()->m_iLastClientSideAnimationUpdateFramecount = m_iNextSimulationTick - 1;

		entity->ClientAnimations() = true;
		entity->UpdateClientSideAnimation();

		g_pGlobalVars->realtime = m_flRealtime;
		g_pGlobalVars->curtime = m_flCurtime;
		g_pGlobalVars->frametime = m_flFrametime;
		g_pGlobalVars->absoluteframetime = m_flAbsFrametime;
		g_pGlobalVars->interpolation_amount = m_flInterpolation;
		g_pGlobalVars->framecount = m_nFrames;
		g_pGlobalVars->tickcount = m_nTicks;
	}

	void Pitchresolver(C_BasePlayer* entity)
	{
		static float ResolvedYaw[65];
		shots_missed[entity->EntIndex()] = shots_fire[entity->EntIndex()] - shots_hit[entity->EntIndex()];
		/*
		tip:
		eyeangles.y == goalfeetyaw always
		eyeyaw == lby always
		*/

		const auto entity_animstate = entity->GetPlayerAnimState();
		/* bruteforce now */


		switch (shots_missed[entity->EntIndex()] % 3) {
		case 0:

			break;
		case 1:
			entity->GetPlayerAnimState()->m_flEyePitch = 971.0f;
			break;
		case 2:
			entity->GetPlayerAnimState()->m_flEyePitch = -183.990005f;
			break;
		}
	}

	void Resolverswitch(C_BasePlayer* entity)
	{
		static float ResolvedYaw[65];
		shots_missed[entity->EntIndex()] = shots_fire[entity->EntIndex()] - shots_hit[entity->EntIndex()];
		const auto entity_animstate = entity->GetPlayerAnimState();
		switch (shots_missed[entity->EntIndex()] % 3) {
		case 0:
			ABSROTATION;
			break;
		case 1:
			entity->GetPlayerAnimState()->m_flAbsRotation() = -60.f;
			break;
		case 2:
			entity->GetPlayerAnimState()->m_flAbsRotation() = 60.f;
			break;
		}
	}
}






//




















































