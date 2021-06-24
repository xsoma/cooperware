#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/vfunc_hook.hpp"
#include <d3d9.h>

namespace index
{
	constexpr auto is_playing_demo = 82;
	constexpr auto EndScene = 42;
	constexpr auto Reset = 16;
	constexpr auto PaintTraverse = 41;
	constexpr auto CreateMove = 24;
	constexpr auto FrameStageNotify = 37;
	constexpr auto DrawModelExecute = 21;
	constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto SvCheatsGetBool = 13;
	constexpr auto OverrideView = 18;
	constexpr auto list_leaves_in_box = 6;
	constexpr auto LockCursor = 67;
	constexpr auto WriteUsercmd = 24;
}
extern int32_t nTickBaseShift;
extern int32_t nSinceUse;
extern bool bInSendMove, bFirstSendMovePack;
namespace Hooks
{
	extern Vector RealAngle;
	extern float FakeAngle;
	extern float Pitch;

	void Initialize( );
	

	//vfunctions
	inline vfunc_hook clientstatehook;
	inline vfunc_hook firebullethook;
	inline vfunc_hook entity_hook;
	inline vfunc_hook direct3d_hook;
	inline vfunc_hook hlclient_hook;
	inline vfunc_hook vguipanel_hook;
	inline vfunc_hook vguisurf_hook;
	inline vfunc_hook mdlrender_hook;
	inline vfunc_hook viewrender_hook;
	inline vfunc_hook clientmode_hook;
	inline vfunc_hook matsys_hook;
	inline vfunc_hook bsp_query_hook;
	inline vfunc_hook sv_cheats;
	inline vfunc_hook NENEAPEOCHI;
	inline vfunc_hook engine_hook;
	inline vfunc_hook setup_bones_hook;


	//using defs
	using CreateMove = bool( __thiscall* )( PVOID _this, float flInputSampleTime, CUserCmd * cmd );
	using WriteUsercmdDeltaToBufferFn = bool( __thiscall* )( IBaseClientDLL*, /*void* unk,*/ int, bf_write*, int, int, bool );
	using doextraboneprocessing_fn = void( __thiscall* )( void*, int, int, int, int, int, int );
	using IsHltv_t = bool( __thiscall* )( uintptr_t );

	//type defs
	typedef int( __thiscall* ListLeavesInBox_t )( void*, const Vector&, const Vector&, unsigned short*, int );
	typedef bool( __thiscall* SetupBones_t )( void*, matrix3x4_t*, int, int, float );

	//functions
	extern 	SetupBones_t o_setup_bones;

	//hooks
	bool __stdcall is_playing_demo();
	bool __fastcall IsHltv( uintptr_t ecx, uintptr_t edx );
	int  __fastcall hkListLeavesInBox( void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax );
	long __stdcall hkEndScene( IDirect3DDevice9* device );
	long __stdcall hkReset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters );
	bool __fastcall hkCreateMove(PVOID _this, DWORD edx, float flInputSampleTime, CUserCmd* cmd );
	void __fastcall hkPaintTraverse( void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce );
	void __fastcall hkDrawModelExecute( void* _this, int, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld );
	void __fastcall hkFrameStageNotify( void* _this, int, ClientFrameStage_t stage );
	void __fastcall hkOverrideView( void* _this, int, CViewSetup* vsView );
	void __fastcall hkLockCursor( void* _this );
	int  __fastcall hkDoPostScreenEffects( void* _this, int, int a1 );
	float __fastcall Hook_GetScreenAspectRatio( void* pEcx, void* pEdx, int32_t iWidth, int32_t iHeight );
	bool __fastcall hkSvCheatsGetBool( PVOID pConVar, void* edx );
	bool __fastcall hkWriteUsercmdDeltaToBuffer( IBaseClientDLL* ECX, void* EDX, /*void* unk,*/ int nSlot, bf_write* buf, int from, int to, bool isNewCmd );
	bool __fastcall SetupBones_h( void* ECX, void* EDX, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );
	bool __fastcall TempEntities_h( void* ECX, void* EDX, void* msg );
	void __stdcall TEFireBulletsPostDataUpdate_h( DataUpdateType_t updateType );
	void HookPlayers( );
}
