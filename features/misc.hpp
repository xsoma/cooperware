#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"


namespace Misc
{
	void ClampMovement(CUserCmd* pCommand, float fMaxSpeed);
	void FakeLag(CUserCmd* cmd);
	void AntiAfkKick(CUserCmd* cmd);
	void Bhop(CUserCmd* cmd);
	void AutoStrafeDirection(CUserCmd* cmd);
	void AutoStrafe(CUserCmd* cmd);
	void FastDuck(CUserCmd* cmd);
	void MoonWalk(CUserCmd* cmd);
	void Init();
}
