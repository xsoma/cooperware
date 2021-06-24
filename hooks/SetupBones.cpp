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


bool __fastcall Hooks::SetupBones_h(void* ECX, void* EDX, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	
	if (Variables.rageaimbot.rageenable)
	{
		if (ECX && ((IClientRenderable*)ECX)->GetIClientUnknown())
		{
			IClientNetworkable* pNetworkable = ((IClientRenderable*)ECX)->GetIClientUnknown()->GetClientNetworkable();
			if (pNetworkable && pNetworkable->GetClientClass() && pNetworkable->GetClientClass()->m_ClassID == ClassId::ClassId_CCSPlayer)
			{
				static auto host_timescale = g_CVar->FindVar(("host_timescale"));
				auto player = (C_BasePlayer*)ECX;
				float OldCurTime = g_pGlobalVars->curtime;
				float OldRealTime = g_pGlobalVars->realtime;
				float OldFrameTime = g_pGlobalVars->frametime;
				float OldAbsFrameTime = g_pGlobalVars->absoluteframetime;
				float OldAbsFrameTimeStart = g_pGlobalVars->absoluteframestarttimestddev;
				float OldInterpAmount = g_pGlobalVars->interpolation_amount;
				int OldFrameCount = g_pGlobalVars->framecount;
				int OldTickCount = g_pGlobalVars->tickcount;

				g_pGlobalVars->curtime = player->m_flSimulationTime();
				g_pGlobalVars->realtime = player->m_flSimulationTime();
				g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick * host_timescale->GetFloat();
				g_pGlobalVars->absoluteframetime = g_pGlobalVars->interval_per_tick * host_timescale->GetFloat();
				g_pGlobalVars->absoluteframestarttimestddev = player->m_flSimulationTime() - g_pGlobalVars->interval_per_tick * host_timescale->GetFloat();
				g_pGlobalVars->interpolation_amount = 0;
				g_pGlobalVars->framecount = TIME_TO_TICKS(player->m_flSimulationTime());
				g_pGlobalVars->tickcount = TIME_TO_TICKS(player->m_flSimulationTime());

				*(int*)((int)player + 236) |= 8; 
				bool ret_value = o_setup_bones(player, pBoneToWorldOut, nMaxBones, boneMask, g_pGlobalVars->curtime);
				*(int*)((int)player + 236) &= ~8; 

				g_pGlobalVars->curtime = OldCurTime;
				g_pGlobalVars->realtime = OldRealTime;
				g_pGlobalVars->frametime = OldFrameTime;
				g_pGlobalVars->absoluteframetime = OldAbsFrameTime;
				g_pGlobalVars->absoluteframestarttimestddev = OldAbsFrameTimeStart;
				g_pGlobalVars->interpolation_amount = OldInterpAmount;
				g_pGlobalVars->framecount = OldFrameCount;
				g_pGlobalVars->tickcount = OldTickCount;
				return ret_value;
			}
		}
	}
	return o_setup_bones(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}




















