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
	using ScreenAspectRatio = void(__thiscall*)(void* pEcx, void* pEdx, int32_t iWidth, int32_t iHeight);
	float __fastcall Hook_GetScreenAspectRatio(void* pEcx, void* pEdx, int32_t iWidth, int32_t iHeight)
	{
		static auto ofunct = NENEAPEOCHI.get_original<ScreenAspectRatio>(101);
		int x, y; g_EngineClient->GetScreenSize(x, y);
		if (!Variables.Visuals.aspectratioenable) return ((float)x / y);
		else return ((float)Variables.Visuals.aspectratiox / Variables.Visuals.aspectratioxy);
		ofunct(pEcx, pEdx, iWidth, iHeight);
	}
}