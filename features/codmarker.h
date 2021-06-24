#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../Singleton.hpp"
struct impact_info {
	float x, y, z;
	long long time;
};

struct hitmarker_info {
	impact_info impact;
	int alpha;
	int damage;
	Color random_color;
};

class CAW_Hitmarker : IGameEventListener2 {
public:
	void initialize();
	void paint();
private:
	void FireGameEvent(IGameEvent* event) override;
	int GetEventDebugID(void) override;
	void player_hurt(IGameEvent* event);
	void bullet_impact(IGameEvent* event);
	C_BasePlayer* get_player(int userid);
	std::vector<impact_info> impacts;
	std::vector<hitmarker_info> hitmarkers;
	Color get_hitmarker_color(hitmarker_info hitmarker);
};

extern CAW_Hitmarker* aw_hitmarker;