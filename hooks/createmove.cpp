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
#include "../features/antiaim.h"
#include "../features/predictionsystem.h"
namespace Hooks
{

	bool __fastcall hkCreateMove(PVOID _this, DWORD edx, float flInputSampleTime, CUserCmd* cmd)
	{
		auto oCreateMove = clientmode_hook.get_original<CreateMove>(index::CreateMove);

		if (!g_LocalPlayer->IsAlive())
		{
			globals::last_doubletap = cmd->tick_count;
			globals::doubletap_delta = 0;
		}
		if (!g_LocalPlayer)
		{
			globals::doubletap_delta = 0;
			globals::last_doubletap = 0;
			return oCreateMove(_this, flInputSampleTime, cmd);
		}
		if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		{
			globals::doubletap_delta = 0;
			globals::last_doubletap = 0;
			return oCreateMove(_this, flInputSampleTime, cmd);
		}
		if (!g_EngineClient->IsInGame() && !g_EngineClient->IsConnected() && !g_LocalPlayer)
			return oCreateMove(_this, flInputSampleTime, cmd);

		if (!cmd || !cmd->command_number)
			return oCreateMove(_this, flInputSampleTime, cmd);

		C_BaseCombatWeapon* Weapon = g_LocalPlayer->m_hActiveWeapon();
		if (!Weapon)
			return oCreateMove(_this, flInputSampleTime, cmd);

		uintptr_t* framePtr;
		__asm mov framePtr, ebp;
		globals::pCmd = cmd;

		if (Menu::Get().IsVisible())
			cmd->buttons &= ~IN_ATTACK;

		CCSGrenadeHint::Get().Tick(cmd->buttons);
		Misc::Init();
		Vector wish_angle = cmd->viewangles;

		Prediction::StartPrediction(cmd, g_LocalPlayer);
		if (Variables.rageaimbot.rageenable)
		{
			Aimbot_R::create_move(cmd);
			Aimbot_R::dt(cmd);

			if (Variables.rageaimbot.antiaimenable)
				AntiAim::DoAntiaim(cmd, Weapon);

			Aimbot_R::autor8();
			AntiAim::slowwalk();
			AntiAim::smallmovements();
		}
		if (Variables.enablelgit)
		{
			Legitbot::Do(cmd, Weapon);
			if (Variables.aimbot.Backtrack)
				Backtrack::Get().Do(cmd);
		}

		Prediction::EndPrediction(g_LocalPlayer);
		AntiAim::MovementFix(wish_angle);
		auto local_player = g_LocalPlayer;

		if (!globals::bSendPacket) {
			Hooks::RealAngle = cmd->viewangles;
			globals::realAngle = cmd->viewangles;
			//AntiAim::LbyUpdate();
		}
		else {
			Hooks::FakeAngle = cmd->viewangles.y;
			globals::fakeAngle = cmd->viewangles;
		}
		Math::Normalize3(cmd->viewangles);
		Math::ClampAngles(cmd->viewangles);

		//AntiAim::desynchams();

		*(bool*)(*framePtr - 0x1C) = globals::bSendPacket;

		return false;
	}
}