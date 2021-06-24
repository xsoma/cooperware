#pragma once
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"
#include "../helpers/input.hpp"

#include <deque>

extern Vector hitscan(C_BasePlayer* entity);
extern bool HitChance(Vector angles, C_BasePlayer* ent, float chance);



struct target_selection_info
{
	target_selection_info(C_BasePlayer* player, const int& idx)
	{
		this->entity = player;
		this->i = idx;
	}
	C_BasePlayer* entity;
	int				i;
};
extern  float server_time(CUserCmd* m_pcmd);

namespace Aimbot_R
{
	extern matrix3x4_t Matrix[65][128];
	void dt(CUserCmd* cmd);
	void create_move(CUserCmd* cmd);
	void target_selection();
	void aim_func(CUserCmd* cmd);

	void autor8();





	bool can_shoot(CUserCmd* cmd);

	float hitchance(C_BaseCombatWeapon* weapon);
	void ClampMov2363526263ement(CUserCmd* pCommand, float fMaxSpeed);


}



