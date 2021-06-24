



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


auto ptr_accumulate_layers = reinterpret_cast<void*> (Utils::FindSignature("client.dll", "84 C0 75 0D F6 87"));
auto setupvelocity_call = reinterpret_cast<void*> (Utils::FindSignature("client.dll", "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0"));


namespace Hooks
{

	bool __fastcall IsHltv(uintptr_t ecx, uintptr_t edx) 
	{

		auto ret = engine_hook.get_original<decltype(&IsHltv)>(93);
		if (!g_EngineClient->IsInGame() || !g_LocalPlayer)
			return ret(ecx, edx);

		if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (ptr_accumulate_layers))
			return true;

		if (reinterpret_cast<uintptr_t> (_ReturnAddress()) == reinterpret_cast<uintptr_t> (setupvelocity_call))
			return true;

		return ret(ecx, edx);
	}
}



















