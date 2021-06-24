#include "valve_sdk/sdk.hpp"
#include "valve_sdk/csgostructs.hpp"
#include <deque>
#include "../features/animfix.h"

struct player_anims2
{
	CBaseHandle handle;
	float spawn_time;
	CBasePlayerAnimState* animstate;
	float animvel;
};
player_anims2 player_data[64];