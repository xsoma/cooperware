

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


namespace Hooks
{

	void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<decltype(&hkPaintTraverse)>(index::PaintTraverse);

		if (Variables.Visuals.noscope && strcmp("HudZoom", g_VGuiPanel->GetName(panel)) == 0)
			return;

		oPaintTraverse(g_VGuiPanel, edx, panel, forceRepaint, allowForce);

		if (!panelId) {
			const auto panelName = g_VGuiPanel->GetName(panel);
			if (!strcmp(panelName, "FocusOverlayPanel")) {
				panelId = panel;
			}

			if (!strcmp(panelName, "MatSystemTopPanel")) {
				panelId = panel;
			}
		}
		else if (panelId == panel) {

			ConVar* PostProcVar = g_CVar->FindVar("mat_postprocess_enable");
			PostProcVar->SetValue(!Variables.Visuals.nopostprocsess);

			int ScreenWidth, ScreenHeight;
			g_EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
			//Render::Get().Text(1, 1, "skeet.com", Render::Get().VerdanaBold12, Color(255, 255, 255, 255), false);


			if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected() || !g_LocalPlayer)
				return;

			if (g_LocalPlayer && IN_SCORE && Variables.Misc.reveal_rank)
				Utils::RankRevealAll();

			if (Variables.Visuals.noscope && g_LocalPlayer->m_bIsScoped())
			{
				Render::Get().Line(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, Color(0, 0, 0, 150));
				Render::Get().Line(0, ScreenHeight / 2, ScreenWidth, ScreenHeight / 2, Color(0, 0, 0, 150));
			}

			if (Variables.Misc.eventlogs)
				in_game_logger.Do();

			if (g_LocalPlayer->IsAlive())
				aw_hitmarker->paint();

			Visuals::Get().run_viewmodel();
			Visuals::Get().aaindicator();
			Visuals::Get().DrawSpreedCircle();

			if (Variables.Visuals.selfcircle && g_Input->m_fCameraInThirdPerson)
				Visuals::Get().drawcirclearoundlocal();

			for (int i = 1; i <= 64; i++)
			{
				auto Player = C_BasePlayer::GetPlayerByIndex(i);

				if (!Player)
					continue;

				//if (entity == g_LocalPlayer)
				//	continue;
				//Visuals::Get().debugshotsmissed(Player);

				if (i < 65 && Player->IsAlive())
				{
					if (Visuals::Get().Begin(Player))
					{
						if (Variables.Visuals.enemy_box)
							Visuals::Get().Box();
						if (Variables.Visuals.enemy_health)
							Visuals::Get().Health();
						if (Variables.Visuals.enemy_armor)
							Visuals::Get().RenderArmour();
						if (Variables.Visuals.enemy_name)
							Visuals::Get().Name();
						if (Variables.Visuals.enemy_weapon[0] || Variables.Visuals.enemy_weapon[1] || Variables.Visuals.enemy_weapon[2])
							Visuals::Get().Weapon();
						if (Variables.Visuals.enemy_skeleton)
							Visuals::Get().RenderSkelet();
						//	if (Variables.Visuals.enemy_skeleton_history)
							//	Visuals::Get().RenderSkeletBacktrack(Player);

						if (Variables.Visuals.enemy_flags)
							Visuals::Get().DrawFlags();

						if (Variables.Visuals.enemy_gossip_arrow)
							Visuals::Get().DrawPoV();

					}
				}
			}
			for (int i = 1; i <= g_EntityList->GetHighestEntityIndex(); i++)
			{
				C_BasePlayer* pBaseEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

				if (!pBaseEntity)
					continue;

				if (Variables.Visuals.weapon_dropped && pBaseEntity->IsWeapon())
					Visuals::Get().RenderWeapon(pBaseEntity);

				Visuals::Get().RenderNadeEsp(pBaseEntity);
				Vector buf, pos = pBaseEntity->GetAbsOrigin();

				if (pos.x == 0 || pos.y == 0 || pos.z == 0 || !Math::WorldToScreen(pos, buf))
					continue;

				if (pBaseEntity != g_LocalPlayer && !pBaseEntity->IsDormant())
				{
					ClientClass* pClass = (ClientClass*)pBaseEntity->GetClientClass();
					if (!pClass)
						continue;

					if (Variables.Visuals.drawc4 && pClass->m_ClassID == (int)ClassId::ClassId_CPlantedC4)
						Visuals::Get().drawbombplanetd(pBaseEntity);
				}
			}

			CCSGrenadeHint::Get().Paint();
		}
	}
}



















