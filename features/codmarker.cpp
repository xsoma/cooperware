#pragma comment(lib, "Winmm.lib")
#include <ctime>
#include <iostream>
#include <chrono>
#include "codmarker.h"
#include "Visuals.hpp"
#include "../Options.hpp"
#include "../helpers/Math.hpp"

using namespace std;
using namespace std::chrono;
typedef __int64	int64;

int64 GetEpochMS()
{
	int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return now;
}

CAW_Hitmarker* aw_hitmarker = new CAW_Hitmarker();

// establish listeners that will trigger our FireGameEvent function
void CAW_Hitmarker::initialize()
{
	g_GameEvents->AddListener(this, "player_hurt", false);
	g_GameEvents->AddListener(this, "bullet_impact", false);
}

template<class T, class U>
inline T cl346amp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

// iterates through hitmarkers and draws/adjusts them (call before ur esp or w/e)
void CAW_Hitmarker::paint()
{
	C_BasePlayer* pLocal = g_LocalPlayer;

	if (!Variables.Visuals.playerhitmarker)
		return;

	if (!g_EngineClient->IsConnected() || !g_EngineClient->IsInGame() || !pLocal)
	{
		if (!impacts.empty())
			impacts.clear();

		if (!hitmarkers.empty())
			hitmarkers.clear();

		return;
	}

	long long time = GetEpochMS();

	std::vector<hitmarker_info>::iterator iter;

	for (iter = hitmarkers.begin(); iter != hitmarkers.end();) {
		bool expired = time > iter->impact.time + 2000;

		if (expired && iter->alpha > 0) iter->alpha -= 5;
		else if (!(expired) && iter->alpha < 255) iter->alpha += 5;

		float alpha = iter->alpha;

		cl346amp(alpha, 0, 255);

		/*static int alpha_interval = 255 / 50;
		if (expired) iter->alpha -= alpha_interval;
		if (expired && iter->alpha <= 0) { // 25 ms expiration
			iter = hitmarkers.erase(iter);
			continue;
		}*/

		// get the location of the hit on our screen
		Vector pos3D = Vector(iter->impact.x, iter->impact.y, iter->impact.z), pos2D;
		if (!Math::WorldToScreen(pos3D, pos2D)) {
			++iter;
			continue;
		}

		// get color of the hitmarker (based on damage)
		//	Color c = get_hitmarker_color(*iter);
		//	c.SetAlpha(iter->alpha);

		auto color = Color{};

		color = Color(
			int(Variables.Visuals.markercol[0] * 255),
			int(Variables.Visuals.markercol[1] * 255),
			int(Variables.Visuals.markercol[2] * 255),
			int(alpha)
		);

		// draw it
		int lineSize = 9;

		//	g_Draw.ColoredCircle(pos2D.x, pos2D.y, 12, 255, 255, 255, 240);  gey test (circle)
		Render::Get().Line(pos2D.x - lineSize, pos2D.y - lineSize, pos2D.x - (lineSize / 4), pos2D.y - (lineSize / 4), color);
		Render::Get().Line(pos2D.x + lineSize, pos2D.y - lineSize, pos2D.x + (lineSize / 4), pos2D.y - (lineSize / 4), color);
		Render::Get().Line(pos2D.x - lineSize, pos2D.y + lineSize, pos2D.x - (lineSize / 4), pos2D.y + (lineSize / 4), color);
		Render::Get().Line(pos2D.x + lineSize, pos2D.y + lineSize, pos2D.x + (lineSize / 4), pos2D.y + (lineSize / 4), color);

		++iter;
	}

}

void CAW_Hitmarker::player_hurt(IGameEvent* event)
{

	C_BasePlayer* attacker = get_player(event->GetInt("attacker"));
	C_BasePlayer* victim = get_player(event->GetInt("userid"));
	C_BasePlayer* pLocal = g_LocalPlayer;
	if (!pLocal)
		return;
	int damage = event->GetInt("dmg_health");

	if (!attacker || !victim || attacker != pLocal)
		return;

	Vector enemyPosition = victim->m_vecOrigin();
	impact_info best_impact;
	float best_impact_distance = -1;
	long long time = GetEpochMS();

	std::vector<impact_info>::iterator iter;
	for (iter = impacts.begin(); iter != impacts.end();) {

		// expire in 25 ms
		if (time > iter->time + 25) {
			iter = impacts.erase(iter);
			continue;
		}

		// get the best impact (closest to hurt player)
		Vector position = Vector(iter->x, iter->y, iter->z);
		float distance = position.DistTo(enemyPosition);
		if (distance < best_impact_distance || best_impact_distance == -1) {
			best_impact_distance = distance;
			best_impact = *iter;
		}

		++iter;

	}

	if (best_impact_distance == -1)
		return;

	// establish new hitmarker, add to logs
	hitmarker_info info;
	info.impact = best_impact;
	info.alpha = 255;
	info.damage = damage;
	hitmarkers.push_back(info);
}

void CAW_Hitmarker::bullet_impact(IGameEvent* event)
{

	C_BasePlayer* shooter = get_player(event->GetInt("userid"));
	C_BasePlayer* pLocal = g_LocalPlayer;
	if (!pLocal)
		return;
	if (!shooter || shooter != pLocal)
		return;

	// establish bullet impact, add to logs
	impact_info info;
	info.x = event->GetFloat("x");
	info.y = event->GetFloat("y");
	info.z = event->GetFloat("z");
	info.time = GetEpochMS();
	impacts.push_back(info);

}

Color CAW_Hitmarker::get_hitmarker_color(hitmarker_info hitmarker)
{
	return Color(200, 200, 200, 255);
}

// function to simply get a player entity from a userid (provided in events)
C_BasePlayer* CAW_Hitmarker::get_player(int userid)
{
	int index = g_EngineClient->GetPlayerForUserID(userid);
	C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(index);
	return entity;
}

// call the corresponding function when an event is triggered
void CAW_Hitmarker::FireGameEvent(IGameEvent* event)
{
	C_BasePlayer* pLocal = g_LocalPlayer;
	if (!pLocal)
		return;
	if (!Variables.Visuals.playerhitmarker)
		return;

	if (!event || !pLocal)
		return;

	if (!strcmp(event->GetName(), "player_hurt"))
		player_hurt(event);

	if (!strcmp(event->GetName(), "bullet_impact"))
		bullet_impact(event);

}

int CAW_Hitmarker::GetEventDebugID(void)
{
	return 0x2A;
}




































































