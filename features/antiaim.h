#pragma once
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"
#include "../helpers/input.hpp"
#include <deque>


extern int AASide;

namespace AntiAim
{

	void smallmovements();
	void DoAntiaim(CUserCmd* cmd, C_BaseCombatWeapon* Weapon);
	extern matrix3x4_t m_fake_matrix[128];
	void LbyUpdate();
	void PredictLBY();
	extern bool m_got_fake_matrix;
	void desynchams();
	void slowwalk();
	void MovementFix(Vector& oldang);
}
