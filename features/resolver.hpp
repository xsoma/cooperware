#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"


extern float printvalueeyeyaw;
extern float printvaluegoalfeetyaw;
extern float printvalueeyeangles;
extern float printabsrotation;
extern float printlby;

extern int shots_missed[65];
extern int shots_hit[65];
extern int shots_fire[65];

extern bool missed3shots;
extern bool missed2shots;

enum correction_flags
{
	DESYNC,
	NO_DESYNC,
	SLOW_WALK
};


struct RInfo
{
	float lastMaxDSC;
	float MaxDSC;
	float EyeAng;
};

/*
namespace RGlobals
{
float MoveDesync;
float DuckDesyncStatic;
float DuckDesyncMove;
float StandingDesyncMax;
//RInfo data[65];
}
*/





namespace Resolver
{

	extern float sideangle[65];

	extern float sideangle2[65];
	extern float sideangle3[65];


	void Pitchresolver(C_BasePlayer* entity);
	void Resolverswitch(C_BasePlayer* entity);
	void AnimationFix(C_BasePlayer* entity);
	void ABSROTATION(C_BasePlayer* player);
	void framestagenotify(ClientFrameStage_t stage);
}


