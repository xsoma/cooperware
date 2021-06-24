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


void CL_ParseEventDelta(void* RawData, void* pToData, RecvTable* pRecvTable)
{
	// "RecvTable_DecodeZeros: table '%s' missing a decoder.", look at the function that calls it.
	static uintptr_t CL_ParseEventDeltaF = (uintptr_t)Utils::PatternScan(GetModuleHandle("engine.dll"), ("55 8B EC 83 E4 F8 53 57"));
	__asm
	{
		mov     ecx, RawData
		mov     edx, pToData
		push	pRecvTable
		call    CL_ParseEventDeltaF
		add     esp, 4
	}
}


bool __fastcall Hooks::TempEntities_h(void* ECX, void* EDX, void* msg)
{
	static auto tempentitties = clientstatehook.get_original<decltype(&TempEntities_h)>(36);


	if (!g_LocalPlayer || !g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return tempentitties(ECX, EDX, msg);

	bool ret = tempentitties(ECX, EDX, msg);



	// Filtering events
	if (!Variables.rageaimbot.rageenable || !g_LocalPlayer->IsAlive())
		return ret;

	CEventInfo* ei = g_ClientState->m_events; //0x4DEC
	CEventInfo* next = NULL;

	if (!ei)
		return ret;

	do
	{
		next = *(CEventInfo**)((uintptr_t)ei + 0x38);

		uint16_t classID = ei->classID - 1;

		auto m_pCreateEventFn = ei->pClientClass->m_pCreateEventFn; // ei->pClientClass->m_pCreateEventFn ptr
		if (!m_pCreateEventFn)
			continue;

		IClientNetworkable* pCE = m_pCreateEventFn();
		if (!pCE)
			continue;

		if (classID == ClassId::ClassId_CTEFireBullets)
		{
			// set fire_delay to zero to send out event so its not here later.
			ei->fire_delay = 0.0f;

			auto pRecvTable = ei->pClientClass->m_pRecvTable;
			void* BasePtr = pCE->GetDataTableBasePtr();

			// Decode data into client event object and use the DTBasePtr to get the netvars
			CL_ParseEventDelta(ei->pData, BasePtr, pRecvTable);

			if (!BasePtr)
				continue;

			// This nigga right HERE just fired a BULLET MANE
			int EntityIndex = *(int*)((uintptr_t)BasePtr + 0x10) + 1;

			auto pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(EntityIndex);
			if (pEntity && pEntity->GetClientClass() && pEntity->GetClientClass()->m_ClassID == ClassId::ClassId_CCSPlayer && pEntity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum())
			{
				Vector EyeAngles = Vector(*(float*)((uintptr_t)BasePtr + 0x24), *(float*)((uintptr_t)BasePtr + 0x28), 0.0f),
					CalcedAngle = Math::CalcAngle(pEntity->GetEyePos(), g_LocalPlayer->GetEyePos());

				*(float*)((uintptr_t)BasePtr + 0x24) = CalcedAngle.x;
				*(float*)((uintptr_t)BasePtr + 0x28) = CalcedAngle.y;
				*(float*)((uintptr_t)BasePtr + 0x2C) = 0;

				float
					event_time = TICKS_TO_TIME(g_pGlobalVars->tickcount),
					player_time = pEntity->m_flSimulationTime();

				// Extrapolate tick to hit those fag scouters etc

				float shot_time = event_time;


			}
		}
		ei = next;
	} while (next != NULL);

	return ret;
}



















