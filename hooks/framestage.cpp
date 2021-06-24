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


	std::vector<const char*> smoke_materials = {
	"particle/beam_smoke_01",
	"particle/particle_smokegrenade",
	"particle/particle_smokegrenade1",
	"particle/particle_smokegrenade2",
	"particle/particle_smokegrenade3",
	"particle/particle_smokegrenade_sc",
	"particle/smoke1/smoke1",
	"particle/smoke1/smoke1_ash",
	"particle/smoke1/smoke1_nearcull",
	"particle/smoke1/smoke1_nearcull2",
	"particle/smoke1/smoke1_snow",
	"particle/smokesprites_0001",
	"particle/smokestack",
	"particle/vistasmokev1/vistasmokev1",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_nearcull",
	"particle/vistasmokev1/vistasmokev1_nearcull_fog",
	"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev4_emods_nocull",
	"particle/vistasmokev1/vistasmokev4_nearcull",
	"particle/vistasmokev1/vistasmokev4_nocull"
	};

	//--------------------------------------------------------------------------------
	void __fastcall hkFrameStageNotify(void* _this, int edx, ClientFrameStage_t stage)
	{
		static auto ofunc = hlclient_hook.get_original<decltype(&hkFrameStageNotify)>(index::FrameStageNotify);
		static std::string old_Skyname = "";
		static bool OldNightmode;
		static int OldSky;
		if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected() || !g_LocalPlayer) // checking for this instead of g_LocalPlayer coz it fucks up when full updating
		{
			old_Skyname = "";
			OldNightmode = false;
			OldSky = 0;
			ofunc(g_CHLClient, edx, stage);

			return;
		}

		if (g_LocalPlayer->IsAlive())
		{

			static ConVar* r_DrawSpecificStaticProp;
			if (OldNightmode != Variables.Visuals.nightmode)
			{
				if (!r_DrawSpecificStaticProp)
					r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
				r_DrawSpecificStaticProp->SetValue(0);
				for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i)) {
					auto material = g_MatSystem->GetMaterial(i);

					if (!material)
						continue;

					if (strstr(material->GetTextureGroupName(), ("World"))) {
						material->ColorModulate(0.08f, 0.08f, 0.08f);
					}
					else if (strstr(material->GetTextureGroupName(), ("StaticProp"))) {
						material->ColorModulate(0.35f, 0.35f, 0.35f);
					}
				}
				OldNightmode = Variables.Visuals.nightmode;
			}

			if (OldSky != Variables.Visuals.skychangar)
			{
				auto LoadNamedSky = reinterpret_cast<void(__fastcall*)(const char*)>(Utils::FindSignature("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
				if (old_Skyname == "")
				{
					//old_Skyname = g_pCvar->FindVar("sv_skyname")->GetName();
				}
				int type = Variables.Visuals.skychangar;
				/*if(type == 0)
				LoadNamedSky(old_Skyname.c_str());
				else */if (type == 1)
	LoadNamedSky("cs_baggage_skybox_");
				else if (type == 2)
	LoadNamedSky("cs_tibet");
				else if (type == 3)
	LoadNamedSky("italy");
				else if (type == 4)
	LoadNamedSky("jungle");
				else if (type == 5)
	LoadNamedSky("office");
				else if (type == 6)
	LoadNamedSky("sky_cs15_daylight02_hdr");
				else if (type == 7)
	LoadNamedSky("sky_csgo_night02");
				else if (type == 8)
	LoadNamedSky("vertigo");
				else if (type == 9)
	LoadNamedSky("sky_descent");
				OldSky = Variables.Visuals.skychangar;
			}

		}

		Vector aim_punch_old;
		Vector view_punch_old;

		Vector* aim_punch = nullptr;
		Vector* view_punch = nullptr;

		if (stage == FRAME_NET_UPDATE_START)
		{
			if (g_LocalPlayer->IsAlive())
			{
				if (Variables.Visuals.nosmoke) 
				{
					static int* smokecount = *(int**)(Utils::FindSignature("client.dll", "A3 ? ? ? ? 57 8B CB") + 0x1);

					if (!smokecount)
						return;

					*smokecount = 0;
				}

				for (auto material_name : smoke_materials)
				{
					IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
					if (!mat)
						continue;

					mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Variables.Visuals.nosmoke ? true : false);
				}
			}
		}
		if (stage == FRAME_RENDER_START)
		{
			static bool init = false;

			if (GetKeyState(Variables.Visuals.thirdperson_key) && g_LocalPlayer->IsAlive())
			{
				if (init)
				{
					ConVar* sv_cheats = g_CVar->FindVar("sv_cheats");
					*(int*)((DWORD)&sv_cheats->m_fnChangeCallbacks + 0xC) = 0; // ew
					sv_cheats->SetValue(1);
					g_EngineClient->ExecuteClientCmd("thirdperson");
				}
				init = false;
			}
			else
			{
				if (!init)
				{
					ConVar* sv_cheats = g_CVar->FindVar("sv_cheats");
					*(int*)((DWORD)&sv_cheats->m_fnChangeCallbacks + 0xC) = 0; // ew
					sv_cheats->SetValue(1);
					g_EngineClient->ExecuteClientCmd("firstperson");
				}
				init = true;
			}

			if (GetKeyState(Variables.Visuals.thirdperson_key) && g_LocalPlayer->IsAlive())
			{
				Vector{ Hooks::RealAngle.x, Hooks::RealAngle.y, 0 };
			}

			AnimFix::LocalAnimFix(g_LocalPlayer);
			for (int i = 1; i <= 64; i++)
			{
				C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
				if (!Player || !Player->IsPlayer() || Player == g_LocalPlayer) continue;

				*(int*)((uintptr_t)Player + 0xA30) = g_pGlobalVars->framecount;
				*(int*)((uintptr_t)Player + 0xA28) = 0;
			}

			if (Variables.Misc.serverhitboxes)
			{
				if (g_LocalPlayer && g_LocalPlayer->IsAlive())
				{
					static auto util_playerbyindex = Utils::FindSignature("server.dll", "85 C9 7E 2A A1");
					static auto draw_server_hitboxes = Utils::FindSignature("server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE");

					auto t = -1.0f;

					auto idx = g_EngineClient->GetLocalPlayer();

					__asm {
						mov ecx, idx
						call util_playerbyindex
						cmp eax, 0
						je gay
						pushad
						movss xmm1, t
						push 0
						mov ecx, eax
						call draw_server_hitboxes
						popad
					}

				gay:
					{

					}
				}


			}


			if (Variables.Visuals.novisrecoil)
			{
				aim_punch = &g_LocalPlayer->m_aimPunchAngle();
				view_punch = &g_LocalPlayer->m_viewPunchAngle();

				aim_punch_old = *aim_punch;
				view_punch_old = *view_punch;

				*aim_punch = Vector(0.f, 0.f, 0.f);
				*view_punch = Vector(0.f, 0.f, 0.f);
			}

			if (Variables.Visuals.noflash && g_LocalPlayer)
				if (g_LocalPlayer->m_flFlashDuration() > 0.f)
					g_LocalPlayer->m_flFlashDuration() = 0.f;

		}
		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
		{

			for (auto i = 1; i <= 64; i++) {
				auto e = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

				// Entity validation checks
				if (!e || !e->IsAlive() || e->IsDormant() || !e->GetPlayerAnimState() || e == g_LocalPlayer || !g_EngineClient->IsConnected() && !g_EngineClient->IsInGame())
					continue;

				e->should_interp(false);//raw data 
			}
		}


		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			if (Variables.rageaimbot.rageenable)

				//Resolver::resolverz( entity );
				for (int i = 1; i <= 65; i++)
				{

					auto entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
					if (!entity)
						continue;
					bool is_local_player = entity == g_LocalPlayer;
					bool is_teammate = g_LocalPlayer->m_iTeamNum() == entity->m_iTeamNum() && !is_local_player;

					if (is_local_player)
						continue;

					if (is_teammate)
						continue;

					if (entity->m_iHealth() <= 0)
						continue;

					if (entity->IsDormant())
						continue;

					if (Variables.rageaimbot.pitchresolver)
						Resolver::Pitchresolver(entity);
					Resolver::AnimationFix(entity);
					if (Variables.rageaimbot.resolverenable)
						Resolver::Resolverswitch(entity);
					Resolver::AnimationFix(entity);
				}
		}
		ofunc(g_CHLClient, edx, stage);

		if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_END)
		{

			for (int i = 1; i <= 65; i++)
			{
				auto entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

				if (!entity)
					continue;

				bool is_local_player = entity == g_LocalPlayer;
				bool is_teammate = g_LocalPlayer->m_iTeamNum() == entity->m_iTeamNum() && !is_local_player;

				if (is_local_player)
					continue;

				if (is_teammate)
					continue;

				if (entity->m_iHealth() <= 0)
					continue;

				if (entity->IsDormant())
					continue;
				//	Resolver::sub_1000F8C0(entity);


			}

		}

		if (stage == ClientFrameStage_t::FRAME_RENDER_START)
		{
			if (Variables.Visuals.novisrecoil && (aim_punch && view_punch))
			{
				*aim_punch = aim_punch_old;
				*view_punch = view_punch_old;
			}
		}

	}
}
