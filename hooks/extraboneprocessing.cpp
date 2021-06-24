
#include "hooks.hpp"
#include <intrin.h>  
#include "../helpers/input.hpp"
#include "../options.hpp"
#include "../helpers/utils.hpp"
#include "../features/visuals.hpp"
#include "../features/ragebot.hpp"
#include "../menu/menu.hpp"
#include "../features/misc.hpp"
#include "../globals/globals.hpp"
#include "../features/resolver.hpp"
#include "../features/animfix.h"
#include "../features/events.h"
#include "../features/bullettracers.hpp"
#include "../features/grenadepred.h"
#include "../features/legitbot.hpp"
#include "../features/lagcomp.h"
#include "../features/codmarker.h"
#include "../features/Skinchanger.hpp"




namespace Hooks
{


	void __fastcall doextraboneprocessing(void* ecx, void* edx, int a2, int a3, int a4, int a5, int a6, int a7) {
		//static auto ofunc = mdlrender_hook.get_original<decltype(&doextraboneprocessing)>(195);
		static auto ofunc = clientmode_hook.get_original<doextraboneprocessing_fn>(197);
		auto e = (C_BasePlayer*)ecx;

		if (!e) {
			ofunc(ecx, a2, a3, a4, a5, a6, a7);
			return;
		}

		auto animstate = e->GetPlayerAnimState();

		if (!animstate || !animstate->pBaseEntity) {
			ofunc(ecx, a2, a3, a4, a5, a6, a7);
			return;
		}
		bool enablefixfit = true;

		if (!enablefixfit) {
			ofunc(ecx, a2, a3, a4, a5, a6, a7);
			return;
		}

		// prevent call to do_procedural_foot_plant
		auto o_on_ground = animstate->m_bOnGround;
		animstate->m_bOnGround = false;
		ofunc(ecx, a2, a3, a4, a5, a6, a7);
		animstate->m_bOnGround = o_on_ground;
	}
	void HookPlayers()
	{

		static bool Hooked[65];

		for (int i = 1; i < g_EngineClient->GetMaxClients(); ++i)
		{
			C_BasePlayer* pPlayerEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

			if (!pPlayerEntity
				|| !pPlayerEntity->IsAlive()
				|| pPlayerEntity->IsDormant())
			{
				if (Hooked[i])
					entity_hook.unhook_index(197);

				Hooked[i] = false;
				continue;
			}



			if (Hooked[i])
				entity_hook.unhook_index(197);


			if (!Hooked[i])
			{
				entity_hook.setup(pPlayerEntity);
				entity_hook.hook_index(197, doextraboneprocessing);

				Hooked[i] = true;
			}
		}
	}
}





















