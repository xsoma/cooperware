#include "predictionsystem.h"
#include "../globals/globals.hpp"

namespace Prediction
{
	float m_flOldCurTime;
	float m_flOldFrametime;
	CMoveData moveData;
	int* predictionRandomSeed = NULL;
	C_BasePlayer* predictionPlayer = NULL;

	void Init()
	{
		predictionRandomSeed = *(int**)(Utils::FindSignature(("client.dll"), ("A3 ? ? ? ? 66 0F 6E 86")) + 0x2);
		predictionPlayer = *reinterpret_cast<C_BasePlayer**>(Utils::FindSignature(("client.dll"), "89 35 ? ? ? ? F3 0F 10 48") + 0x2);
	}

	void StartPrediction(CUserCmd* pCmd, C_BasePlayer* entity)
	{
		*predictionRandomSeed = MD5_PseudoRandom(pCmd->command_number) ^ 0x7FFFFFFF;
		predictionPlayer = entity;

		m_flOldCurTime = g_pGlobalVars->curtime;
		m_flOldFrametime = g_pGlobalVars->frametime;

		g_pGlobalVars->curtime = entity->m_nTickBase() * g_pGlobalVars->interval_per_tick;
		g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;

		Vector oldVelocity = entity->m_vecVelocity();

		//Here we're doing CBasePlayer::UpdateButtonState
		moveData.m_nButtons = pCmd->buttons;
		int buttonsChanged = pCmd->buttons ^ *reinterpret_cast<int*>(uintptr_t(entity) + 0x31E8);
		*reinterpret_cast<int*>(uintptr_t(entity) + 0x31DC) = (uintptr_t(entity) + 0x31E8);
		*reinterpret_cast<int*>(uintptr_t(entity) + 0x31E8) = pCmd->buttons;
		*reinterpret_cast<int*>(uintptr_t(entity) + 0x31E0) = pCmd->buttons & buttonsChanged;  //m_afButtonPressed ~ The changed ones still down are "pressed"
		*reinterpret_cast<int*>(uintptr_t(entity) + 0x31E4) = buttonsChanged & ~pCmd->buttons; //m_afButtonReleased ~ The ones not down are "released"

		g_GameMovement->StartTrackPredictionErrors(entity);

		memset(&moveData, 0, sizeof(CMoveData));
		g_MoveHelper->SetHost(entity);
		g_Prediction->SetupMove(entity, pCmd, g_MoveHelper, &moveData);
		g_GameMovement->ProcessMovement(entity, &moveData);
		g_Prediction->FinishMove(entity, pCmd, &moveData);

		entity->m_vecVelocity() = oldVelocity;
	}

	void EndPrediction(C_BasePlayer* entity)
	{
		g_GameMovement->FinishTrackPredictionErrors(entity);
		g_MoveHelper->SetHost(nullptr);

		*predictionRandomSeed = -1;
		predictionPlayer = nullptr;

		g_pGlobalVars->curtime = m_flOldCurTime;
		g_pGlobalVars->frametime = m_flOldFrametime;
	}

}





















