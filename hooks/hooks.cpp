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
#include "../features/antiaim.h"
#include "../features/Skinchanger.hpp"
#pragma intrinsic(_ReturnAddress) 

Vector Hooks::RealAngle;
float Hooks::FakeAngle;
float Hooks::Pitch;

#pragma intrinsic(_ReturnAddress)  
int32_t nTickBaseShift = 0;
int32_t nSinceUse = 0;
bool bInSendMove = false, bFirstSendMovePack = false;

namespace Hooks {

	SetupBones_t o_setup_bones = nullptr;

	void Initialize( )
	{
		//	auto dwFireBullets = *(DWORD**)(Utils::PatternScan("client.dll", "55 8B EC 51 53 56 8B F1 BB ? ? ? ? B8") + 0x131);


			//firebullethook.setup(dwFireBullets);
		direct3d_hook.setup( g_D3DDevice9 ); //
		hlclient_hook.setup( g_CHLClient );//
		vguipanel_hook.setup( g_VGuiPanel );//
		vguisurf_hook.setup( g_VGuiSurface );//
		mdlrender_hook.setup( g_MdlRender );//
		clientmode_hook.setup( g_ClientMode );//
		matsys_hook.setup( g_MatSystem );//
		ConVar* sv_cheats_con = g_CVar->FindVar( "sv_cheats" );
		sv_cheats.setup( sv_cheats_con );
		bsp_query_hook.setup( g_EngineClient->GetBSPTreeQuery( ) );

		engine_hook.setup( g_EngineClient );
		engine_hook.hook_index( 93, IsHltv );

		//firebullethook.hook_index(7, TEFireBulletsPostDataUpdate_h);

		engine_hook.hook_index(index::is_playing_demo, is_playing_demo);
		direct3d_hook.hook_index( index::EndScene, hkEndScene );
		direct3d_hook.hook_index( index::Reset, hkReset );
		hlclient_hook.hook_index( index::FrameStageNotify, hkFrameStageNotify );
		clientmode_hook.hook_index( index::CreateMove, hkCreateMove );
		vguipanel_hook.hook_index( index::PaintTraverse, hkPaintTraverse );
		vguisurf_hook.hook_index( index::LockCursor, hkLockCursor );
		mdlrender_hook.hook_index( index::DrawModelExecute, hkDrawModelExecute );
		clientmode_hook.hook_index( index::DoPostScreenSpaceEffects, hkDoPostScreenEffects );
		clientmode_hook.hook_index( index::OverrideView, hkOverrideView );
		sv_cheats.hook_index( index::SvCheatsGetBool, hkSvCheatsGetBool );
		bsp_query_hook.hook_index( index::list_leaves_in_box, hkListLeavesInBox );
		hlclient_hook.hook_index( index::WriteUsercmd, hkWriteUsercmdDeltaToBuffer );

		NENEAPEOCHI.setup( g_EngineClient );
		NENEAPEOCHI.hook_index( 101, Hook_GetScreenAspectRatio );

		auto tmp_addr = Utils::FindSignature( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" );
		const auto dw_ccs_player_renderablevftable = *reinterpret_cast< DWORD* > (tmp_addr + 0x4A);

		o_setup_bones = vftable_hook::hook_manual<SetupBones_t>( ( uintptr_t* ) dw_ccs_player_renderablevftable, 13, ( SetupBones_t ) SetupBones_h );

		o_setup_bones;
		HookPlayers( );
		Render::Get( ).CreateFonts( );

		g_InputSystem->EnableInput( true );

		g_CVar->FindVar("viewmodel_fov")->null_callback();
		g_CVar->FindVar( "viewmodel_offset_x" )->null_callback();
		g_CVar->FindVar( "viewmodel_offset_y" )->null_callback();
		g_CVar->FindVar( "viewmodel_offset_z" )->null_callback();
		NetvarHook( );
		aw_hitmarker->initialize( );

	}
	//--------------------------------------------------------------------------------
	/*void Shutdown( )
	{
		direct3d_hook.unhook_all( );
		hlclient_hook.unhook_all( );
		vguipanel_hook.unhook_all( );
		engine_hook.unhook_all( );
		vguisurf_hook.unhook_all( );
		NENEAPEOCHI.unhook_all( );
		mdlrender_hook.unhook_all( );
		clientmode_hook.unhook_all( );
		matsys_hook.unhook_all( );
		entity_hook.unhook_all( );
		sv_cheats.unhook_all( );
		Glow::Get( ).Shutdown( );
		bsp_query_hook.unhook_all( );

		g_InputSystem->EnableInput( true );
	}*/
	//--------------------------------------------------------------------------------



}
