#include "events.h"
#include "resolver.hpp"
#include "bullettracers.hpp"
#include "../sounds.h"
#include "../valve_sdk/csgostructs.hpp"
#include "../features/visuals.hpp"
#include "../menu/menu.hpp"

#define EVENT_HOOK( x )

#define HITGROUP_GENERIC	  0
#define HITGROUP_HEAD         1
#define HITGROUP_CHEST        2
#define HITGROUP_STOMACH      3
#define HITGROUP_LEFTARM      4    
#define HITGROUP_RIGHTARM     5
#define HITGROUP_LEFTLEG      6
#define HITGROUP_RIGHTLEG     7
#define HITGROUP_GEAR         10

char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_LEFTLEG:
		return "left leg";
	case HITGROUP_RIGHTLEG:
		return "right leg";
	case HITGROUP_STOMACH:
		return "stomach";
	case HITGROUP_LEFTARM:
		return "left arm";
	case HITGROUP_RIGHTARM:
		return "right arm";
	default:
		return "body";
	}
}

cGameEvent g_Event;

int gitgrp;

template<class T2>
static T2* FindHudElement2(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::FindSignature("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::FindSignature("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T2*)find_hud_element(pThis, name);
}

C_BasePlayer* get_player(int userid) {
	int i = g_EngineClient->GetPlayerForUserID(userid);
	return (C_BasePlayer*)g_EntityList->GetClientEntity(i);
}

float dot_product(const Vector& a, const Vector& b) {
	return (a.x * b.x
		+ a.y * b.y
		+ a.z * b.z);
}

static void CapsuleOverlay(C_BasePlayer* pPlayer, Color col, float duration)
{
	if (!pPlayer)
		return;

	studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel((model_t*)pPlayer->GetModel());
	if (!pStudioModel)
		return;

	static matrix3x4_t pBoneToWorldOut[128];
	if (!pPlayer->SetupBones(pBoneToWorldOut, 128, 0x00000100, 0))
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->GetHitboxSet(0);
	if (!pHitboxSet)
		return;

	auto VectorTransform2 = [](const Vector in1, matrix3x4_t in2, Vector& out)
	{

		out[0] = dot_product(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = dot_product(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = dot_product(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	};

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);
		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		VectorTransform2(pHitbox->bbmin, pBoneToWorldOut[pHitbox->bone], vMin); //nullptr???
		VectorTransform2(pHitbox->bbmax, pBoneToWorldOut[pHitbox->bone], vMax);

		if (pHitbox->m_flRadius > -1)
		{
			g_DebugOverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->m_flRadius, col.r(), col.g(), col.b(), 230, duration);
		}
	}
}

void cGameEvent::FireGameEvent(IGameEvent* event)
{
	if (!g_LocalPlayer)
		return;

	if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
	{
		const char* szEventName = event->GetName();
		if (!szEventName)
			return;

		if (!strcmp(szEventName, "player_death"))
		{

			auto userid = event->GetInt("userid");
			auto attacker = event->GetInt("attacker");

			if (!userid || !attacker)
				return;


			auto userid_id = g_EngineClient->GetPlayerForUserID(userid);
			auto attacker_id = g_EngineClient->GetPlayerForUserID(attacker);
			player_info_t userid_info, attacker_info;
			if (!g_EngineClient->GetPlayerInfo(userid_id, &userid_info))
				return;

			if (!g_EngineClient->GetPlayerInfo(attacker_id, &attacker_info))
				return;

			static DWORD* _death_notice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");
			static void(__thiscall * _clear_notices)(DWORD) = (void(__thiscall*)(DWORD))Utils::FindSignature("client.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");


			if (_death_notice)
				*(float*)((DWORD)_death_notice + 0x50) = Variables.Visuals.preservekillfeed ? 500 : 1.5;
		}

		if (strcmp(szEventName, "player_hurt") == 0)
		{
			auto entity = C_BasePlayer::GetPlayerByIndex(g_EngineClient->GetPlayerForUserID(event->GetInt("userid")));
			
			if (!entity)
				return;

			if (entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
				return;

			auto dmgdone = event->GetInt("dmg_health");

			if (!dmgdone)
				return;

			if (g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")) == g_EngineClient->GetLocalPlayer() && g_EngineClient->GetPlayerForUserID(event->GetInt("userid")) != g_EngineClient->GetLocalPlayer())
			{
				auto color = Color{};

				color = Color(
					int(Variables.Visuals.lagcomp_col[0] * 255),
					int(Variables.Visuals.lagcomp_col[1] * 255),
					int(Variables.Visuals.lagcomp_col[2] * 255)
				);

				if (Variables.Visuals.lagcomp_enable)
					CapsuleOverlay(entity, color, Variables.Visuals.lagcomp_duration);
			}

			auto attacker = get_player(event->GetInt("attacker"));
			if (attacker == g_LocalPlayer)
			{
				shots_hit[entity->GetIndex()]++;
				switch (Variables.Visuals.hitsound)
				{
				case 0: break;
				case 1: g_VGuiSurface->PlaySound_("buttons\\arena_switch_press_02.wav"); break;
				case 2: PlaySoundA(rifk7_wav, NULL, SND_ASYNC | SND_MEMORY); break;
				case 3: PlaySoundA(bame_wav, NULL, SND_ASYNC | SND_MEMORY); break;
				}
			}
		}

		/*
		if (Variables.buybot && !strcmp(szEventName, "round_prestart"))
		{

			if (Variables.dualsbuy)
			{
				g_EngineClient->ClientCmd("buy elite");
			}
			if (Variables.deaglebuy)
			{
				g_EngineClient->ClientCmd("buy deagle");
			}
			if (Variables.r8buy)
			{
				g_EngineClient->ClientCmd("buy revolver");
			}

			if (Variables.armorbuy)
			{
				g_EngineClient->ClientCmd("buy vesthelm");
				g_EngineClient->ClientCmd("buy vest");
			}

			if (Variables.zeusbuy)
			{
				g_EngineClient->ClientCmd("buy taser 34");
			}

			if (Variables.kitbuy)
			{
				g_EngineClient->ClientCmd("buy defuser");
			}

			if (Variables.awpbuy)
			{
				g_EngineClient->ClientCmd("buy awp");
			}


			if (Variables.scar20buy)
			{
				g_EngineClient->ClientCmd("buy scar20");
			}
			if (Variables.g3sg1buy)
			{
				g_EngineClient->ClientCmd("buy g3sg1");
			}
			if (Variables.ssg08buy)
			{
				g_EngineClient->ClientCmd("buy ssg08");
			}



			if (Variables.grenadebuy)
			{
				g_EngineClient->ClientCmd("buy hegrenade");
			}
			if (Variables.firenadebuy)
			{
				g_EngineClient->ClientCmd("buy incgrenade");
			}
			if (Variables.molotovbuy)
			{
				g_EngineClient->ClientCmd("buy molotov");
			}
			if (Variables.smokebuy)
			{
				g_EngineClient->ClientCmd("buy smokegrenade");
			}

		}
		*/

		if (strcmp(szEventName, "player_hurt") == 0)
		{
			auto entity = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(event->GetInt("userid")));
			
			if (!entity)
				return;

			auto attacker = get_player(event->GetInt("attacker"));
			if (attacker == g_LocalPlayer)
			{

			}
		}

		if (strstr(event->GetName(), "player_hurt"))
		{
			if (!event)
				return;

			auto* attacker = C_BasePlayer::GetPlayerByIndex(g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")));
			if (!attacker)
				return;

			if (attacker != g_LocalPlayer)
				return;

			const char* fuckthattrash;

			player_info_s player_info;
			g_EngineClient->GetPlayerInfo(g_EngineClient->GetPlayerForUserID(event->GetInt("userid")), &player_info);

			auto color = Color2{};

			color = Color2(
				int(Variables.Misc.event_logs_col[0] * 255),
				int(Variables.Misc.event_logs_col[1] * 255),
				int(Variables.Misc.event_logs_col[2] * 255),
				int(255)
			);

			InGameLogger::Log log;
			log.color_line.PushBack(("[CooperWare] "), color);
			log.color_line.PushBack("Hurt ", Color2(255, 255, 255, 255));
			log.color_line.PushBack(player_info.szName, Color2(255, 255, 255, 255));


			int hitgroup = event->GetInt("hitgroup");
			if (hitgroup > 0 && hitgroup <= 7)
			{
				log.color_line.PushBack(" in the ", Color2(255, 255, 255, 255));
				log.color_line.PushBack(HitgroupToName(hitgroup), Color2(255, 255, 255, 255));
				gitgrp = hitgroup;
				fuckthattrash = HitgroupToName(hitgroup);
			}

			log.color_line.PushBack(" for ", Color2(255, 255, 255, 255));
			log.color_line.PushBack(std::to_string(event->GetInt("dmg_health")), Color2(255, 255, 255, 255));
			log.color_line.PushBack(" damage, ", Color2(255, 255, 255, 255));

			log.color_line.PushBack("( health ", Color2(255, 255, 255, 255));
			log.color_line.PushBack(" remaining : ", Color2(255, 255, 255, 255));
			log.color_line.PushBack(std::to_string(event->GetInt("health")), Color2(255, 255, 255, 255));
			log.color_line.PushBack(" hp ) ", Color2(255, 255, 255, 255));

			if (Variables.Misc.eventlogs)
				in_game_logger.AddLog(log);
		}

		if (strstr(event->GetName(), "round_start"))
		{
			static DWORD* deathNotice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");

			static DWORD* _death_notice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");
			static void(__thiscall * _clear_notices)(DWORD) = (void(__thiscall*)(DWORD))Utils::FindSignature("client.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");

			_death_notice = FindHudElement2<DWORD>("CCSGO_HudDeathNotice");
			_clear_notices(((DWORD)_death_notice - 20));
			for (int i = 0; i < 64; i++) {

			}
		}

		if (strstr(event->GetName(), "item_purchase"))
		{
			if (!event)
				return;

			auto* entity = C_BasePlayer::GetPlayerByIndex(g_EngineClient->GetPlayerForUserID(event->GetInt("userid")));

			if (!entity)
				return;

			if (entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
				return;

			player_info_s player_info;

			if (!g_EngineClient->GetPlayerInfo(entity->EntIndex(), &player_info))
				return;

			InGameLogger::Log log;

			auto color = Color2{};

			color = Color2(
				int(Variables.Misc.event_logs_col[0] * 255),
				int(Variables.Misc.event_logs_col[1] * 255),
				int(Variables.Misc.event_logs_col[2] * 255),
				int(255)
			);

			log.color_line.PushBack(("[CooperWare] "), color);
			log.color_line.PushBack(player_info.szName, Color2(255, 255, 255, 230));
			log.color_line.PushBack(" bought ", Color2(255, 255, 255, 230));
			log.color_line.PushBack(event->GetString("weapon"), Color2(255, 255, 255, 230));

			in_game_logger.AddLog(log);
		}
	}
}

int cGameEvent::GetEventDebugID()
{
	return 42;
}

void cGameEvent::RegisterSelf()
{
	g_GameEvents->AddListener(this, "player_connect", false);
	g_GameEvents->AddListener(this, "player_hurt", false);
	g_GameEvents->AddListener(this, "round_start", false);
	g_GameEvents->AddListener(this, "round_end", false);
	g_GameEvents->AddListener(this, "player_death", false);
	g_GameEvents->AddListener(this, "weapon_fire", false);
	g_GameEvents->AddListener(this, "bullet_impact", false);
	g_GameEvents->AddListener(this, "item_purchase", false);
	g_GameEvents->AddListener(this, "round_prestart", false);
}

void cGameEvent::Register()
{
	EVENT_HOOK(FireEvent);
}

float GetCurtime325325()
{

	if (!g_LocalPlayer)
		return -1;

	return static_cast<float>(g_LocalPlayer->m_nTickBase())* g_pGlobalVars->interval_per_tick;
}

InGameLogger in_game_logger;

void ColorLine::Draw(int x, int y, unsigned int font)
{
	for (int i = 0; i < texts.size(); i++)
	{
		//DrawFF(x, y, font, false, false, colors[i], texts[i]);
		//fixed that shit with nodraw at all
		g_VGuiSurface->DrawT(x, y, colors[i], Render::Get().eventesp, false, texts[i].c_str());
		x += Render::Get().GetTextSize(font, texts[i]).x;
	}
}

void InGameLogger::Do()
{
	if (log_queue.size() > max_lines_at_once)
		log_queue.erase(log_queue.begin() + max_lines_at_once, log_queue.end());

	for (int i = 0; i < log_queue.size(); i++)
	{
		auto log = log_queue[i];
		float time_delta = fabs(GetCurtime325325() - log.time);

		int height = ideal_height + (16 * i);

		/// erase dead logs
		if (time_delta > text_time)
		{
			log_queue.erase(log_queue.begin() + i);
			break;
		}

		if (time_delta > text_time - slide_out_speed)
			height = height + (((slide_out_speed - (text_time - time_delta)) / slide_out_speed) * slide_out_distance);

		/// fade out
		if (time_delta > text_time - text_fade_out_time)
			log.color_line.ChangeAlpha(255 - (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * 255.f));
		
		/// fade in
		if (time_delta < text_fade_in_time)
			log.color_line.ChangeAlpha((time_delta / text_fade_in_time) * 255.f);

		int width = ideal_width;

		/// slide from left
		if (time_delta < text_fade_in_time)
			width = (time_delta / text_fade_in_time) * static_cast<float>(slide_in_distance) + (ideal_width - slide_in_distance);
		
		/// slider from right
		if (time_delta > text_time - text_fade_out_time)
			width = ideal_width + (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * static_cast<float>(slide_out_distance));

		log.color_line.Draw(width, height, Render::Get().eventesp);
	}
}