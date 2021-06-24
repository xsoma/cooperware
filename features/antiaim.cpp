#include "antiaim.h"
#include "../globals/globals.hpp"
#include "misc.hpp"
int AASide = 1;
float get_estimate_server_time(CUserCmd* cmd) {
	const auto net_chan = static_cast<INetChannelInfo*> (g_EngineClient->GetNetChannelInfo());

	const auto local_latency = net_chan->GetAvgLatency(INetChannelInfo::LOCALPLAYER);
	const auto generic_latency = net_chan->GetAvgLatency(INetChannelInfo::GENERIC);

	return local_latency + generic_latency + TICKS_TO_TIME(1) + TICKS_TO_TIME(cmd->tick_count);
}
namespace AntiAim
{
	bool set_curtime_after_prediction = false;
	bool in_lby_break = false;
	bool ShouldSwitch{ false };
	float LastPreYaw{ 0.f };
	float NextLBYUpdateTime{ 0.f };
	void LbyUpdate()
	{
		in_lby_break = false;

		if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
			return;

		static auto spawntime = g_LocalPlayer->m_flSpawnTime();
		auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();

		if (!weapon)
			return;

		if (weapon->m_Item().m_iItemDefinitionIndex() != WEAPON_C4 && (globals::pCmd->buttons & IN_USE || globals::pCmd->buttons & IN_ATTACK))
			return;

		auto state = reinterpret_cast<CCSGOPlayerAnimState*> (g_LocalPlayer->GetPlayerAnimState());

		if (!state)
			return;

		static auto next_update = -1.f;

		const auto curtime = get_estimate_server_time(globals::pCmd);
		{
			if (spawntime != g_LocalPlayer->m_flSpawnTime())
			{
				spawntime = g_LocalPlayer->m_flSpawnTime();
				next_update = -1.f;
			}

			if (state->get_vec_velocity().Length2D() > 0.1f)
				next_update = curtime + 0.22f;
			else if (next_update == -1.f || curtime >= next_update)
				next_update = curtime + 1.1f;
		}

		if (g_LocalPlayer->m_vecVelocity().Length2D() > 0.1f)
			return;


		const auto in_lby_update = g_LocalPlayer->m_fFlags() & FL_ONGROUND && next_update - curtime <= g_pGlobalVars->
			interval_per_tick;

		auto broke_this_tick = false;

		if (in_lby_update)
		{
			broke_this_tick = true;

			globals::bSendPacket = true;
			if (ShouldSwitch)
				globals::pCmd->viewangles.y = LastPreYaw + 120.f;
			else
				globals::pCmd->viewangles.y = LastPreYaw - 120.f;
		}

		static auto broke_last_tick = false;
		static auto was_break_lastlast_tick = false;

		if (was_break_lastlast_tick)
		{
			globals::bSendPacket = false;
			was_break_lastlast_tick = false;
			return;
		}

		if (broke_last_tick)
		{
			was_break_lastlast_tick = true;
			globals::bSendPacket = true;
			globals::pCmd->viewangles.y = LastPreYaw;
			broke_last_tick = false;
			return;
		}

		if (broke_this_tick)
		{
			if (!weapon->IsGrenade())
				globals::pCmd->buttons &= ~IN_ATTACK;

			globals::bSendPacket = false;

			set_curtime_after_prediction = true;
			in_lby_break = true;
			broke_last_tick = true;
		}
	}

	void manualaa_stand()
	{
		//key setup
		if (GetAsyncKeyState(Variables.rageaimbot.right))
		{
			globals::aaside = 0;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.left))
		{
			globals::aaside = 1;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.back))
		{
			globals::aaside = 2;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.front))
		{
			globals::aaside = 3;
		}

		//antiaim setup
		if (globals::aaside == 0)
		{
			globals::pCmd->viewangles.y -= 90;
		}
		else if (globals::aaside == 1)
		{
			globals::pCmd->viewangles.y += 90;
		}
		else if (globals::aaside == 2)
		{
			globals::pCmd->viewangles.y += 180;
		}
		else if (globals::aaside == 3)
		{
			globals::pCmd->viewangles.y += 0;
		}

	}

	void manualaa_move()
	{
		//key setup
		if (GetAsyncKeyState(Variables.rageaimbot.right))
		{
			globals::aaside = 0;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.left))
		{
			globals::aaside = 1;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.back))
		{
			globals::aaside = 2;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.front))
		{
			globals::aaside = 3;
		}

		//antiaim setup
		if (globals::aaside == 0)
		{
			globals::pCmd->viewangles.y -= 90;
		}
		else if (globals::aaside == 1)
		{
			globals::pCmd->viewangles.y += 90;
		}
		else if (globals::aaside == 2)
		{
			globals::pCmd->viewangles.y += 180;
		}
		else if (globals::aaside == 3)
		{
			globals::pCmd->viewangles.y += 0;
		}

	}

	void manualaa_air()
	{
		//key setup
		if (GetAsyncKeyState(Variables.rageaimbot.right))
		{
			globals::aaside = 0;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.left))
		{
			globals::aaside = 1;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.back))
		{
			globals::aaside = 2;
		}
		else if (GetAsyncKeyState(Variables.rageaimbot.front))
		{
			globals::aaside = 3;
		}

		//antiaim setup
		if (globals::aaside == 0)
		{
			globals::pCmd->viewangles.y -= 90;
		}
		else if (globals::aaside == 1)
		{
			globals::pCmd->viewangles.y += 90;
		}
		else if (globals::aaside == 2)
		{
			globals::pCmd->viewangles.y += 180;
		}
		else if (globals::aaside == 3)
		{
			globals::pCmd->viewangles.y += 0;
		}

	}

	Vector get_hitbox_pzos(C_BasePlayer* entity, int hitbox_id)
	{
		auto getHitbox = [](C_BasePlayer* entity, int hitboxIndex) -> mstudiobbox_t*
		{
			if (entity->IsDormant() || entity->m_iHealth() <= 0) return NULL;

			const auto pModel = entity->GetModel();
			if (!pModel) return NULL;

			auto pStudioHdr = g_MdlInfo->GetStudiomodel(pModel);
			if (!pStudioHdr) return NULL;

			auto pSet = pStudioHdr->GetHitboxSet(0);
			if (!pSet) return NULL;

			if (hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0) return NULL;

			return pSet->GetHitbox(hitboxIndex);
		};

		auto hitbox = getHitbox(entity, hitbox_id);
		if (!hitbox) return Vector(0, 0, 0);

		auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

		Vector bbmin, bbmax;
		Math::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
		Math::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

		return (bbmin + bbmax) * 0.5f;
	}

	inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
	{

		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}

	void NormalizeNum(Vector& vIn, Vector& vOut)
	{
		float flLen = vIn.Length();
		if (flLen == 0) {
			vOut.Init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}

	float fov_player(Vector ViewOffSet, Vector View, C_BasePlayer* entity, int hitbox)
	{
		const float MaxDegrees = 180.0f;
		Vector Angles = View, Origin = ViewOffSet;
		Vector Delta(0, 0, 0), Forward(0, 0, 0);
		Vector AimPos = get_hitbox_pzos(entity, hitbox);

		Math::AngleVectors(Angles, Forward);
		VectorSubtract(AimPos, Origin, Delta);
		NormalizeNum(Delta, Delta);

		float DotProduct = Forward.Dot(Delta);
		return (acos(DotProduct) * (MaxDegrees / M_PI));
	}

	int closest_to_crosshair()
	{
		int index = -1;
		float lowest_fov = INT_MAX;

		auto local_player = g_LocalPlayer;

		if (!local_player)
			return -1;

		Vector local_position = local_player->GetEyePos();

		Vector angles;
		g_EngineClient->GetViewAngles(angles);

		for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
		{
			C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

			if (!entity || entity->m_iHealth() <= 0 || entity->m_iTeamNum() == local_player->m_iTeamNum() || entity->IsDormant() || entity == local_player)
				continue;

			float fov = fov_player(local_position, angles, entity, 0);

			if (fov < lowest_fov)
			{
				lowest_fov = fov;
				index = i;
			}
		}

		return index;
	}
	float last_real;
	void autoDirection(CUserCmd* cmd)
	{
		auto local_player = g_LocalPlayer;

		if (!local_player)
			return;


		bool no_active = true;
		float bestrotation = 0.f;
		float highestthickness = 0.f;
		Vector besthead;

		auto leyepos = local_player->m_vecOrigin() + local_player->m_vecViewOffset();
		auto headpos = get_hitbox_pzos(local_player, 0);
		auto origin = local_player->GetAbsOrigin();

		auto checkWallThickness = [&](C_BasePlayer* pPlayer, Vector newhead) -> float
		{
			Vector endpos1, endpos2;
			Vector eyepos = pPlayer->GetEyePos();

			Ray_t ray;
			ray.Init(newhead, eyepos);

			CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

			trace_t trace1, trace2;
			g_EngineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

			if (trace1.DidHit())
				endpos1 = trace1.endpos;
			else
				return 0.f;

			ray.Init(eyepos, newhead);
			g_EngineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

			if (trace2.DidHit())
				endpos2 = trace2.endpos;

			float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
			return endpos1.DistTo(endpos2) + add / 3;
		};

		int index = closest_to_crosshair();
		auto entity = (C_BasePlayer*)g_EntityList->GetClientEntity(index);
		if (!entity)
			return;
		float step = (2 * M_PI) / 18.f;
		float radius = fabs(Vector(headpos - origin).Length2D());

		if (index == -1)
		{
			no_active = true;
		}
		else
		{
			for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
			{
				Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

				float totalthickness = 0.f;

				no_active = false;

				totalthickness += checkWallThickness(entity, newhead);

				if (totalthickness > highestthickness)
				{
					highestthickness = totalthickness;
					bestrotation = rotation;
					besthead = newhead;
				}
			}
		}
		if (no_active)
			cmd->viewangles.y -= 180.f;
		else
			cmd->viewangles.y = RAD2DEG(bestrotation);

	}

	float get_max_desync_delta_aimware()
	{
		if (!g_LocalPlayer) 
			return 0.f;

		auto thisptr = g_LocalPlayer->GetPlayerAnimState();
		auto v1 = ((float*)thisptr)[62];
		auto v2 = 1.0f;
		auto v3 = 0.0f;
		auto v4 = 0.f;
		auto v7 = 0.0f;
		auto v8 = 0.f;
		auto v10 = 0.0f;
		auto v11 = v1;

		if (v1 <= 1.0f) 
		{
			v4 = v10;

			if (v1 >= 0.0)
				v4 = v11;

			v3 = v4;
		}
		else
			v3 = 1.0f;

		auto v5 = ((float*)thisptr)[41];
		auto v6 = (float)((float)((float)(((float*)thisptr)[71] * -0.3f) - 2.0f) * v3) + 1.0f;

		if (v5 > 0.0) 
		{
			v7 = ((float*)thisptr)[63];
			v11 = 0.0;
			v10 = v7;

			if (v7 <= 1.0f) {
				v8 = v11;

				if (v7 >= 0.0)
					v8 = v10;

				v2 = v8;
			}

			v6 = v6 + (float)((float)(v2 * v5) * (float)(0.5f - v6));
		}

		return ((float*)thisptr)[205] * v6;
	};

	void PredictLBY() {
		if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
			return;

		static auto spawntime = -1.f;
		auto state = reinterpret_cast<CCSGOPlayerAnimState*> (g_LocalPlayer->GetPlayerAnimState());

		if (!state)
			return;

		if (spawntime != g_LocalPlayer->m_flSpawnTime())
		{
			spawntime = g_LocalPlayer->m_flSpawnTime();
			NextLBYUpdateTime = 0.f;
		}

		if (state->get_vec_velocity().Length2D() > 0.1f)
			NextLBYUpdateTime = globals::curtime + 0.22f;
		else if (NextLBYUpdateTime == -1.f || globals::curtime >= NextLBYUpdateTime)
			NextLBYUpdateTime = globals::curtime + 1.1f;
	}

	void DoFake(CUserCmd* userCMD)
	{
		static bool inverter = false;
		static size_t lastTime = 0;

		if (GetAsyncKeyState(Variables.rageaimbot.staticinverter))
		{
			if (GetTickCount() > lastTime) 
			{
				inverter = !inverter;
				lastTime = GetTickCount() + 450;
			}
		}

		if (inverter)
		{
			LbyUpdate();
			userCMD->viewangles.y -= (Variables.rageaimbot.staticleftsidevalue - 100);
			if (globals::bSendPacket)
			{
				LbyUpdate();
				userCMD->viewangles.y -= g_LocalPlayer->MaxDesyncDelta();
			}
			else
			{
				if (LbyUpdate)
					userCMD->viewangles.y += (g_LocalPlayer->MaxDesyncDelta() * 2);
				else
					LbyUpdate();
				userCMD->viewangles.y -= 119.f;
			}
		}
		else
		{
			LbyUpdate();
			userCMD->viewangles.y += (Variables.rageaimbot.staticrightsidevalue - 100);
			if (globals::bSendPacket)
			{
				LbyUpdate();
				userCMD->viewangles.y += g_LocalPlayer->MaxDesyncDelta();
			}
			else
			{
				LbyUpdate();
				if (LbyUpdate)
					userCMD->viewangles.y -= (g_LocalPlayer->MaxDesyncDelta() * 2);
				else
					LbyUpdate();
				userCMD->viewangles.y += 119.f;
			}
		}
	}

	void static_desyncov(CUserCmd* userCMD)
	{
		static bool inverter = false;
		static size_t lastTime = 0;

		if (GetAsyncKeyState(Variables.rageaimbot.staticinverter))
		{
			if (GetTickCount() > lastTime) {
				inverter = !inverter;
				lastTime = GetTickCount() + 450;
			}
		}

		static bool flip = false;
		flip = !flip;

		if (inverter)
		{
			LbyUpdate();
			if (g_LocalPlayer->m_vecVelocity().Length2D() == 0.f) //ghetto as fuck fix for animations while moving lmfao
				userCMD->viewangles.y += flip ? -120.f : 120.f;
		}
		else
		{
			LbyUpdate();
			userCMD->viewangles.y += flip ? -g_LocalPlayer->MaxDesyncDelta() : g_LocalPlayer->MaxDesyncDelta();
		}
	}

	void DoAntiaim(CUserCmd* cmd, C_BaseCombatWeapon* Weapon)
	{
		if (!g_LocalPlayer)
			return;

		if (!g_LocalPlayer->IsAlive())
			return;

		C_BaseCombatWeapon* pWeapon = g_LocalPlayer->m_hActiveWeapon().Get();

		if (!pWeapon)
			return;

		float flServerTime = g_LocalPlayer->m_nTickBase() * g_pGlobalVars->interval_per_tick;
		bool canShoot = (pWeapon->m_flNextPrimaryAttack() <= flServerTime);

		if (g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER) 
			return;

		if (cmd->buttons & IN_USE)
			return;

		if (cmd->buttons & IN_ATTACK && canShoot) 
			return;

		if (pWeapon->IsGrenade()) 
			return;

		if (pWeapon->IsKnife())
			return;

		if (!(GetAsyncKeyState(Variables.rageaimbot.fakeduckkey)))
		{
			if ((globals::pCmd->buttons & IN_ATTACK))
			{
				if (globals::aimangle == Vector(0, 0, 0))
				{
					return;
				}
				else
				{

					if (globals::bSendPacket)
						globals::pCmd->viewangles = globals::aimangle;
					else
						globals::pCmd->viewangles.y = get_max_desync_delta_aimware() + 20.0f;
				}
			}
		}

		static bool flip = false;
		flip = !flip;

		static bool zxzzz = false;
		zxzzz = !zxzzz;

		if (Variables.rageaimbot.autodirection)	
			autoDirection(cmd);		

		switch (Variables.rageaimbot.pitch)
		{
		case 0: //off
			break;
		case 1: //default
			cmd->viewangles.x = 89.f;
			break;
		case 2:
			cmd->viewangles.x = 991.f;
			break;
		case 3:
			cmd->viewangles.x = -991.f;
			break;
		}

		if (g_LocalPlayer->m_vecVelocity().Length2D() < 10) //standing
		{
			if (Variables.rageaimbot.jitterenablestanding)
			{
				switch (Variables.rageaimbot.standingaa)
				{
				case 0: //off
					break;
				case 1: //default
					cmd->viewangles.y += 180 + (zxzzz ? -Variables.rageaimbot.jittervaluestanding : Variables.rageaimbot.jittervaluestanding);
					break;
				case 2:
					manualaa_stand();
					break;
				case 3: //default
					cmd->viewangles.y -= 180 + (zxzzz ? -Variables.rageaimbot.jittervaluestanding : Variables.rageaimbot.jittervaluestanding);
					break;
				}
			}
			else
			{
				switch (Variables.rageaimbot.standingaa)
				{
				case 0: //off
					break;
				case 1://default
					cmd->viewangles.y += 180;
					break;
				case 2:
					manualaa_stand();
					break;
				case 3://default
					cmd->viewangles.y -= 180;
					autoDirection(cmd);
					break;
				}

			}
		}
		else
		{
			if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND)) //air
			{
				if (Variables.rageaimbot.jitterenableair)
				{
					switch (Variables.rageaimbot.airaa)
					{
					case 0: //off
						break;
					case 1://default
						cmd->viewangles.y += 180 + (zxzzz ? -Variables.rageaimbot.jittervalueair : Variables.rageaimbot.jittervalueair);
						break;
					case 2:
						manualaa_air();
						break;
					case 3://default
						cmd->viewangles.y -= 180 + (zxzzz ? -Variables.rageaimbot.jittervalueair : Variables.rageaimbot.jittervalueair);
						autoDirection(cmd);
						break;
					}
				}
				else
				{
					switch (Variables.rageaimbot.airaa)
					{
					case 0: //off
						break;
					case 1://default
						cmd->viewangles.y += 180;
						break;
					case 2:
						manualaa_air();
						break;
					case 3://default
						cmd->viewangles.y -= 180;
						autoDirection(cmd);
						break;
					}
				}
			}
			else // moving
			{
				if (Variables.rageaimbot.jitterenablemoving)
				{
					switch (Variables.rageaimbot.movingaa)
					{
					case 0: //off
						break;
					case 1://default
						cmd->viewangles.y += 180 + (zxzzz ? -Variables.rageaimbot.jittervaluemoving : Variables.rageaimbot.jittervaluemoving);
						break;
					case 2:
						manualaa_move();
						break;
					case 3://default
						cmd->viewangles.y -= 180 + (zxzzz ? -Variables.rageaimbot.jittervaluemoving : Variables.rageaimbot.jittervaluemoving);
						autoDirection(cmd);
						break;
					}
				}
				else
				{
					switch (Variables.rageaimbot.movingaa)
					{
					case 0: //off
						break;
					case 1://default
						cmd->viewangles.y += 180;
						break;
					case 2:
						manualaa_move();
						break;
					case 3://default
						cmd->viewangles.y -= 180;
						autoDirection(cmd);
						break;
					}
				}
			}
		}

		if (Variables.rageaimbot.desynctype == 1)
			DoFake(cmd);
		if (Variables.rageaimbot.desynctype == 2)
			static_desyncov(cmd);
		if (globals::bSendPacket)
		{
			if (Variables.rageaimbot.desynctype == 3)
				cmd->viewangles.y -= 180 + (flip ? -58 : 58);
		}
		if (globals::bSendPacket && Variables.rageaimbot.desynctype == 4)
		{
			static bool desync_flip = false;
			desync_flip = !desync_flip;
			globals::pCmd->viewangles.y -= 45 + 45 + 45 + 45 + (desync_flip ? -g_LocalPlayer->get_max_desync_delta() : g_LocalPlayer->get_max_desync_delta());
		}

		cmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);
	}

	void smallmovements()
	{
		if (Variables.rageaimbot.antiaimenable && globals::pCmd)
		{
			if (!(g_LocalPlayer->m_nMoveType() == MoveType_t::MOVETYPE_LADDER))
			{
				if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
				{
					if (g_ClientState->iChokedCommands) {
						static bool flip_movement = false;

						if (globals::pCmd->sidemove == 0.0f && !(globals::pCmd->buttons & (int)IN_JUMP))
							globals::pCmd->sidemove = flip_movement ? -1.011f : 1.011f;

						flip_movement = !flip_movement;
					}

					if (g_ClientState->iChokedCommands) {
						static bool flip_movement = false;

						if (globals::pCmd->forwardmove == 0.0f && !(globals::pCmd->buttons & (int)IN_JUMP))
							globals::pCmd->forwardmove = flip_movement ? -1.011f : 1.011f;

						flip_movement = !flip_movement;
					}
				}
			}
		}
	}
	void slowwalk()
	{
		if (globals::pCmd)
		{
			if (Variables.Misc.slowwalk)
			{
				if (GetAsyncKeyState(Variables.rageaimbot.slowwalkkey))
					if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
						Misc::ClampMovement(globals::pCmd, Variables.Misc.slowwalkspeed * 2);
			}
		}

	}
	bool m_should_update_fake = false;
	std::array< AnimationLayer, 15 > m_fake_layers; // 13
	std::array< float, 24 > m_fake_poses; // 20
	CBasePlayerAnimState* m_fake_state = nullptr;
	bool init_fake_anim = false;
	float m_fake_spawntime = 0.f;
	matrix3x4_t m_fake_matrix[128];
	matrix3x4_t m_fake_position_matrix[128];
	bool m_got_fake_matrix = false;

	void desynchams()
	{
		if (!g_LocalPlayer || !g_LocalPlayer->IsAlive()) {
			m_should_update_fake = true;
			return;
		}

		if (m_fake_spawntime != g_LocalPlayer->m_flSpawnTime() || m_should_update_fake)
		{
			init_fake_anim = false;
			m_fake_spawntime = g_LocalPlayer->m_flSpawnTime();
			m_should_update_fake = false;
		}

		if (!init_fake_anim)
		{
			m_fake_state = reinterpret_cast<CBasePlayerAnimState*> (g_pMemAlloc->Alloc(sizeof(CBasePlayerAnimState)));

			if (m_fake_state != nullptr)
				g_LocalPlayer->CreateAnimationState(m_fake_state);

			init_fake_anim = true;
		}

		if (globals::bSendPacket)
		{
			int OldFrameCount = g_pGlobalVars->framecount;
			int OldTickCount = g_pGlobalVars->tickcount;
			static auto host_timescale = g_CVar->FindVar(("host_timescale"));

			g_pGlobalVars->framecount = TIME_TO_TICKS(g_LocalPlayer->m_flSimulationTime());
			g_pGlobalVars->tickcount = TIME_TO_TICKS(g_LocalPlayer->m_flSimulationTime());

			std::memcpy(m_fake_layers.data(), g_LocalPlayer->GetAnimOverlays(), sizeof(m_fake_layers));
			std::memcpy(m_fake_poses.data(), g_LocalPlayer->m_flPoseParameter().data(), sizeof(m_fake_poses));

			//g_LocalPlayer->UpdateAnimationState(m_fake_state, globals::pCmd->viewangles);

			const auto backup_absangles = g_LocalPlayer->GetAbsAngles();

			/* invalidate bone cache */
			g_LocalPlayer->GetMostRecentModelBoneCounter() = 0;
			g_LocalPlayer->GetLastBoneSetupTime() = -FLT_MAX;

			m_got_fake_matrix = g_LocalPlayer->SetupBones(m_fake_matrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING & ~BONE_USED_BY_ATTACHMENT, g_pGlobalVars->curtime);

			memcpy(m_fake_position_matrix, m_fake_matrix, sizeof(m_fake_position_matrix));

			const auto org_tmp = g_LocalPlayer->GetRenderOrigin();

			if (m_got_fake_matrix)
			{
				for (auto& i : m_fake_matrix)
				{
					i[0][3] -= org_tmp.x;
					i[1][3] -= org_tmp.y;
					i[2][3] -= org_tmp.z;
				}
			}

			g_LocalPlayer->SetAbsAngles(backup_absangles); // restore real abs rotation

			/* invalidate bone cache */
			g_LocalPlayer->GetMostRecentModelBoneCounter() = 0;
			g_LocalPlayer->GetLastBoneSetupTime() = -FLT_MAX;

			std::memcpy(g_LocalPlayer->GetAnimOverlays(), m_fake_layers.data(), sizeof(m_fake_layers));
			std::memcpy(g_LocalPlayer->m_flPoseParameter().data(), m_fake_poses.data(), sizeof(m_fake_poses));

			g_pGlobalVars->framecount = OldFrameCount;
			g_pGlobalVars->tickcount = OldTickCount;
		}
	}

	inline float VectorNormalize(Vector& v)
	{
		Assert(v.IsValid());
		float l = v.Length();
		if (l != 0.0f)
		{
			v /= l;
		}
		else
		{
			// FIXME:
			// Just copying the existing implemenation; shouldn't res.z == 0?
			v.x = v.y = 0.0f; v.z = 1.0f;
		}
		return l;
	}
	float GRD_TO_BOG(float GRD) {
		return (M_PI / 180.f) * GRD;
	}
	void sin_cos(float radian, float* sin, float* cos)
	{
		*sin = std::sin(radian);
		*cos = std::cos(radian);
	}
	void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sp, sy, sr, cp, cy, cr;

		sin_cos(GRD_TO_BOG(angles.x), &sp, &cp);
		sin_cos(GRD_TO_BOG(angles.y), &sy, &cy);
		sin_cos(GRD_TO_BOG(angles.z), &sr, &cr);

		if (forward != nullptr)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right != nullptr)
		{
			right->x = -1 * sr * sp * cy + -1 * cr * -sy;
			right->y = -1 * sr * sp * sy + -1 * cr * cy;
			right->z = -1 * sr * cp;
		}

		if (up != nullptr)
		{
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}
	void MovementFix(Vector& oldang)
	{
		Vector aWishDir = oldang;
		aWishDir.Clamp();

		Vector aRealDir = globals::pCmd->viewangles;
		aRealDir.Clamp();

		Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
		AngleVectors(aWishDir, &view_fwd, &view_right, &view_up);
		AngleVectors(aRealDir, &cmd_fwd, &cmd_right, &cmd_up);

		const auto v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
		const auto v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
		const auto v12 = sqrtf(view_up.z * view_up.z);

		const Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
		const Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
		const Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

		const auto v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
		const auto v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
		const auto v18 = sqrtf(cmd_up.z * cmd_up.z);

		const Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
		const Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
		const Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

		const auto v22 = norm_view_fwd.x * globals::pCmd->forwardmove;
		const auto v26 = norm_view_fwd.y * globals::pCmd->forwardmove;
		const auto v28 = norm_view_fwd.z * globals::pCmd->forwardmove;
		const auto v24 = norm_view_right.x * globals::pCmd->sidemove;
		const auto v23 = norm_view_right.y * globals::pCmd->sidemove;
		const auto v25 = norm_view_right.z * globals::pCmd->sidemove;
		const auto v30 = norm_view_up.x * globals::pCmd->upmove;
		const auto v27 = norm_view_up.z * globals::pCmd->upmove;
		const auto v29 = norm_view_up.y * globals::pCmd->upmove;

		globals::pCmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
			+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
			+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
		globals::pCmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
			+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
			+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
		globals::pCmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
			+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
			+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

		aWishDir = globals::pCmd->viewangles;

		if (g_LocalPlayer->m_nMoveType() != MOVETYPE_LADDER)
			globals::pCmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);
	}
}
