#include "../valve_sdk/sdk.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include <deque>



namespace AnimFix
{

	bool setup_bones(C_BasePlayer* target, int boneMask, float currentTime, matrix3x4_t* pBoneToWorldOut);
	bool HandleBoneSetup(C_BasePlayer* target, matrix3x4_t* pBoneToWorldOut, int boneMask, float currentTime);
	void build_server_bones(C_BasePlayer* player, matrix3x4_t* pBoneToWorldOut);
	void EnemyAnimFix(C_BasePlayer* entity);
	void LocalAnimFix(C_BasePlayer* entity);
	void build_matrix(C_BasePlayer* pl, matrix3x4_t mat[128]);
}
