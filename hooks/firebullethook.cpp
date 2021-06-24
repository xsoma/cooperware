#include "hooks.hpp"
#include <intrin.h>  
#include "../helpers/input.hpp"
#include "../options.hpp"
#include "../helpers/utils.hpp"
#include "../features/visuals.hpp"
#include "../features/ragebot.hpp"
#include "../menu/menu.hpp"
#include "../features/misc.hpp"
#include "../globals/globals.hpp"
#include "../features/resolver.hpp"
#include "../features/animfix.h"
#include "../features/events.h"
#include "../features/bullettracers.hpp"
#include "../features/grenadepred.h"
#include "../features/legitbot.hpp"
#include "../features/lagcomp.h"
#include "../features/codmarker.h"
#include "../features/Skinchanger.hpp"


void __stdcall FireBullets_PostDataUpdate(C_TEFireBullets* thisptr, DataUpdateType_t updateType)
{

	static auto firebullets = Hooks::firebullethook.get_original<decltype(&FireBullets_PostDataUpdate)>(7);


	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return firebullets(thisptr, updateType);

	if (Variables.rageaimbot.rageenable && thisptr)
	{
		int iPlayer = thisptr->m_iPlayer + 1;
		if (iPlayer < 64)
		{
			auto player = C_BasePlayer::GetPlayerByIndex(iPlayer);

			if (player && player != g_LocalPlayer && !player->IsDormant() && !player->IsTeamMate())
			{
				Vector eyeAngles = Vector(thisptr->m_vecAngles.x, thisptr->m_vecAngles.y, thisptr->m_vecAngles.z);
				Vector calcedAngle = Math::CalcAngle(player->GetEyePos(), g_LocalPlayer->GetEyePos());

				thisptr->m_vecAngles.x = calcedAngle.x;
				thisptr->m_vecAngles.y = calcedAngle.y;
				thisptr->m_vecAngles.z = 0.f;

				float
					event_time = g_pGlobalVars->tickcount,
					player_time = player->m_flSimulationTime();

				// Extrapolate tick to hit scouters etc
			

				float shot_time = TICKS_TO_TIME(event_time);
		
								/*if (!lag_records.empty())
								{
									int choked = floorf((event_time - player_time) / g_GlobalVars->interval_per_tick) + 0.5;
									choked = (choked > 14 ? 14 : choked < 1 ? 0 : choked);
									player->m_vecOrigin() = (lag_records.begin()->m_vecOrigin + (g_GlobalVars->interval_per_tick * lag_records.begin()->m_vecVelocity * choked));
								}*/

			}
		}
	}

	firebullets(thisptr, updateType);
}

__declspec (naked) void __stdcall Hooks::TEFireBulletsPostDataUpdate_h(DataUpdateType_t updateType)
{
	__asm
	{
		push[esp + 4]
		push ecx
		call FireBullets_PostDataUpdate
		retn 4
	}
}






















