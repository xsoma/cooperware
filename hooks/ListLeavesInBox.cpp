#include "hooks.hpp"
#include <intrin.h>  


namespace Hooks
{
	int __fastcall hkListLeavesInBox( void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax ) {
		static auto o_fn = bsp_query_hook.get_original< ListLeavesInBox_t >( index::list_leaves_in_box );

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

		struct RenderableInfo_t {
			IClientRenderable* m_pRenderable;
			void* m_pAlphaProperty;
			int m_EnumCount;
			int m_nRenderFrame;
			unsigned short m_FirstShadow;
			unsigned short m_LeafList;
			short m_Area;
			uint16_t m_Flags;   // 0x0016
			uint16_t m_Flags2; // 0x0018
			Vector m_vecBloatedAbsMins;
			Vector m_vecBloatedAbsMaxs;
			Vector m_vecAbsMins;
			Vector m_vecAbsMaxs;
			int pad;
		};

		static auto insert_into_tree_call_list_leaves_in_box = Utils::FindSignature( "client.dll", "89 44 24 14 EB 08 C7 44 24 ? ? ? ? ? 8B 45" );

		if ( _ReturnAddress( ) != ( void* ) insert_into_tree_call_list_leaves_in_box )
			return o_fn( bsp, mins, maxs, pList, listMax );

		// get current renderable info from stack ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1470 )
		auto info = *( RenderableInfo_t** ) ( ( uintptr_t ) _AddressOfReturnAddress( ) + 0x14 );
		if ( !info || !info->m_pRenderable )
			return o_fn( bsp, mins, maxs, pList, listMax );

		// check if disabling occulusion for players ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1491 )
		auto base_entity = info->m_pRenderable->GetIClientUnknown( )->GetBaseEntity( );
		if ( !base_entity || !base_entity->IsPlayer( ) )
			return o_fn( bsp, mins, maxs, pList, listMax );

		// fix render order, force translucent group ( https://www.unknowncheats.me/forum/2429206-post15.html )
		// AddRenderablesToRenderLists: https://i.imgur.com/hcg0NB5.png ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L2473 )
		info->m_Flags &= ~0x100;
		info->m_Flags2 |= 0xC0;

		// extend world space bounds to maximum ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L707 )
		static const Vector map_min = Vector( MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT );
		static const Vector map_max = Vector( MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT );
		auto count = o_fn( bsp, map_min, map_max, pList, listMax );
		return count;

	}
}




















