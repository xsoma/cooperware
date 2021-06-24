#pragma once

#include "sdk.hpp"
#include <array>
#include "../helpers/utils.hpp"
#include "../features/Skinchanger.hpp"
#include <minwindef.h>
#include <specstrings.h>
#include <winnt.h>
#include <winapifamily.h>
#include <xutility>
#include <fstream>
#define TICK_INTERVAL g_pGlobalVars->interval_per_tick
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define NETVAR(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
    }

#define PNETVAR(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
    }
#define NETVARADDOFFS(type, name, table, netvar, offs)                           \
    type& name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar) + offs;     \
        return *(type*)((uintptr_t)this + _##name);                 \
	}
#define NETPROP(name, table, netvar) static RecvProp* name() \
{ \
	static auto prop_ptr = NetvarSys::Get().GetNetvarProp(table,netvar); \
	return prop_ptr; \
}
#define OFFSET( type, offset )		( *( type* ) ( ( std::uintptr_t ) this + ( offset ) ) )
#define OFFSETPTR( type, offset )    ( ( type ) ( ( std::uintptr_t ) this + ( offset ) ) )
#define FOFFSET( type, ptr, offset ) ( ( type ) ( ( std::uintptr_t ) (ptr) + ( offset ) ) )
#define Assert( _exp ) ((void)0)
#define offset(func, type, offset) type& func { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }

struct varmapentry_t {
	std::uint16_t m_type;
	std::uint16_t m_needstointerpolate;
	void* data;
	void* watcher;
};
struct varmapping_t {
	varmapentry_t* m_entries;
	int m_interpolatedentries;
	float m_lastinterptime;
};
struct datamap_t;
class AnimationLayer;
class CBasePlayerAnimState;
class CCSGOPlayerAnimState;
class C_BaseEntity;
class CBoneAccessor;
class CBoneCache;
class CCSGOPlayerAnimState
{
public:
	void* ent; //0x0060 
	float feet_speed; //0x00F8 
	float feet_shit; //0x00FC 
	float stop_to_full_run_frac; //0x011C 
	float max_yaw; //0x0334 
	float duck_amt; //0x00A4 

	Vector get_vec_velocity()
	{
		// Only on ground velocity
		return *reinterpret_cast<Vector*> (reinterpret_cast<uintptr_t> (this) + 0xC8);
	}

	float get_velocity()
	{
		return *reinterpret_cast<float*> (reinterpret_cast<uintptr_t> (this) + 0xEC);
	}

	char pad_0x0000[0x344]; //0x0000
}; //Size=0x344
class AnimationLayer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void* m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038
class CBasePlayerAnimState
{
public:
	char pad[4];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float& m_flAbsRotation() {
		return *(float*)((uintptr_t)this + 0x80);
	}
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float& m_flMagicFraction() {
		return *(float*)((uintptr_t)this + 0x124);
	}
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float& m_flTimeSinceInAir() {
		return *(float*)((uintptr_t)this + 0x110);
	}
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];

};
enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

class C_BaseEntity;


class C_EconItemView
{
private:
	using str_32 = char[32];
public:
	NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
	NETVAR(int32_t, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, m_iEntityLevel, "DT_BaseAttributableItem", "m_iEntityLevel");
	NETVAR(int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(str_32, m_iCustomName, "DT_BaseAttributableItem", "m_szCustomName");
};

class C_BaseEntity : public IClientEntity
{
public:
	static constexpr auto ef_nointerp = 8;
	datamap_t* GetDataDescMap() {
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return CallVFunction<o_GetPredDescMap>(this, 15)(this);
	}

	datamap_t* GetPredDescMap() {
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return CallVFunction<o_GetPredDescMap>(this, 17)(this);
	}
	static __forceinline C_BaseEntity* GetEntityByIndex(int index) {
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
	}
	static __forceinline C_BaseEntity* get_entity_from_handle(CBaseHandle h) {
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
	}

	NETVAR(int32_t, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(int32_t, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");

	NETVAR(Vector, m_angRotation, "DT_BaseEntity", "m_angRotation");
	NETVAR(Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecAngles, "DT_BaseEntity", "m_vecAngles");
	NETVAR(bool, m_bShouldGlow, "DT_DynamicProp", "m_bShouldGlow");
	NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(bool, m_bSpotted, "DT_BaseEntity", "m_bSpotted");
	NETVAR(float_t, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");


	const matrix3x4_t& m_rgflCoordinateFrame()
	{
		static auto _m_rgflCoordinateFrame = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
		return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
	}

	bool IsPlayer();
	bool IsLoot();
	bool IsWeapon();
	bool IsPlantedC4();
	bool IsDefuseKit();
	void InvalidateBoneCache();

	//bool isSpotted();
};

class C_PlantedC4
{
public:
	NETVAR(bool, m_bBombTicking, "DT_PlantedC4", "m_bBombTicking");
	NETVAR(bool, m_bBombDefused, "DT_PlantedC4", "m_bBombDefused");
	NETVAR(float, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(float, m_flTimerLength, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(float, m_flDefuseLength, "DT_PlantedC4", "m_flDefuseLength");
	NETVAR(float, m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
	NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(uint64_t, m_OriginalOwnerXuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");
	NETVAR(int32_t, m_iEntityQuality, "CBaseAttributableItem", "m_iEntityQuality");
	NETVAR(C_EconItemView, m_Item2, "DT_BaseAttributableItem", "m_Item");
	int* fixItemIDHigh()
	{
		return (int*)((uintptr_t)this + 0x2FC0);
	}
	C_EconItemView& m_Item()
	{
		// Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
		// A real fix for this requires a rewrite of the netvar manager
		return *(C_EconItemView*)this;
	}
	void SetGloveModelIndex(int modelIndex);

};

class C_BaseWeaponWorldModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseWeaponWorldModel", "m_nModelIndex");
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	NETVAR(float_t, m_flNextPrimaryAttack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(int32_t, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(int32_t, m_iPrimaryReserveAmmoCount, "DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
	NETVAR(float_t, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(int32_t, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, m_iWorldDroppedModelIndex, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(bool, m_bPinPulled, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(float_t, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, m_flPostponeFireReadyTime, "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(CHandle<C_BaseWeaponWorldModel>, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(float_t, m_fLastShotTime, "CWeaponCSBase", "m_fLastShotTime");

	short m_iItemDefinitionIndex()
	{
		if (!this)
			return 0;
		return *reinterpret_cast<short*>(uintptr_t(this) + 0x2FAA);
	}
	short* m_iItemDefinitionIndex2()
	{
		if (!this)
			return 0;
		return reinterpret_cast<short*>(uintptr_t(this) + 0x2FAA);
	}
	c_base_handle m_hWeaponWorldModel()
	{
		return *(c_base_handle*)((uintptr_t)this + 0x3224);
	}
	CCSWeaponInfo* GetCSWeaponData();
	bool HasBullets();
	bool IsWeaponNonAim();
	bool CanFire();
	bool IsZeus();
	bool IsGrenade();
	bool IsKnife();
	bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
	bool IsGun();
	float GetInaccuracy();
	float GetSpread();
	void UpdateAccuracyPenalty();
	CUtlVector<IRefCounted*>& m_CustomMaterials();
	bool* m_bCustomMaterialInitialized();
	bool is_non_aim();
	bool is_empty();
	bool can_fire();
	bool IsScopeable();
	std::string GetName()
	{
		///TODO: Test if szWeaponName returns proper value for m4a4 / m4a1-s or it doesnt recognize them.
		return std::string(this->GetCSWeaponData()->szWeaponName);
	}
};

#define member_func_args(...) (this, __VA_ARGS__ ); }
#define vfunc(index, func, sig) auto func { return reinterpret_cast<sig>((*(uint32_t**)this)[index]) member_func_args
class IKContext
{
public:
	void init(studiohdr_t* hdr, Vector& angles, Vector& origin, float curtime, int framecount, int boneMask);
	void update_targets(Vector* pos, quaternion* q, matrix3x4_t* bone_array, byte* computed);
	void solve_dependencies(Vector* pos, quaternion* q, matrix3x4_t* bone_array, byte* computed);

	void clear_targets()
	{
		/*
		08.10.2019
		if ( (unsigned __int8)sub_101C9820(v53) || (*(_DWORD *)(v7 + 236) >> 3) & 1 )
		{
			  v56 = 0;
			  if ( *(_DWORD *)(v105 + 4080) > 0 )
			  {
				   v57 = (_DWORD *)(v105 + 208);
				   do
				   {
					   *v57 = -9999;
						v57 += 85;
						++v56;
					}
					while ( v56 < *(_DWORD *)(v105 + 4080) );
				  }
				}
		}
		*/
		auto v56 = 0;
		if (*(int*)((DWORD)this + 4080) > 0)
		{
			auto v57 = (int*)((DWORD)this + 208);
			do
			{
				*v57 = -9999;
				v57 += 85;
				++v56;
			} while (v56 < *(int*)((DWORD)this + 4080));
		}
	}
};


class C_BasePlayer : public C_BaseEntity
{
public:
	static __forceinline C_BasePlayer* GetPlayerByUserId(int id)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(g_EngineClient->GetPlayerForUserID(id)));
	}
	static __forceinline C_BasePlayer* GetPlayerByIndex(int i)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(i));
	}
	varmapping_t* varmap(void) {
		return OFFSETPTR(varmapping_t*, 0x24);
	}

	int GetIndex()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x64);
	}
	void HealthShotBoostExpirationTime(float value)
	{
		static int m_flHealthShotBoostExpirationTime = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flHealthShotBoostExpirationTime");
		*reinterpret_cast<float*>(uintptr_t(this) + m_flHealthShotBoostExpirationTime) = value;
	}
	float FireRate();
	bool IsTeamMate()
	{
		return this->m_iTeamNum() == g_LocalPlayer->m_iTeamNum();
	}
	matrix3x4_t* get_bone_array_for_write() {
		return *(matrix3x4_t**)((uintptr_t)this + 0x26A8);
	}
	offset(get_ik_context(), IKContext*, 9836 + 0x4);
	matrix3x4_t GetBoneMatrix(int BoneID)
	{
		matrix3x4_t matrix;

		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x26A8);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}
	void should_interp(bool interp) {
		auto map = this->varmap();

		if (!map)
			return;

		for (auto i = 0; i < map->m_interpolatedentries; i++)
			map->m_entries[i].m_needstointerpolate = interp;
	}

	Vector& GetAbsOrigin()
	{
		if (!this)
			return Vector();
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return CallVFunction<OriginalFn>(this, 10)(this);
	}
	void SetAbsOrigin(const Vector& origin)
	{
		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector & origin);
		static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		SetAbsOrigin(this, origin);
	}
	Vector& GetAbsAngles()
	{
		if (!this)
			return Vector();
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return CallVFunction<OriginalFn>(this, 11)(this);
	}
	NETVAR(bool, m_bHasDefuser, "DT_CSPlayer", "m_bHasDefuser");

	float get_bomb_defuse_timer()
	{
		if (!this)
			return 0.f;

		float returnValue = m_flDefuseCountDown() - g_pGlobalVars->curtime;
		return (returnValue < 0) ? 0.f : returnValue;
	}

	float& m_flDefuseCountDown()
	{
		static int offset = NetvarSys::Get().GetOffset("DT_PlantedC4", "m_flDefuseCountDown");
		return *(float*)((DWORD)this + offset);
	}
	float& newsim()
	{
		auto gay = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return *(float*)((DWORD)this + gay);
	}
	NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser2, "DT_PlantedC4", "m_hBombDefuser");
	NETVAR(bool, m_bGunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(int32_t, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(float, duck, "DT_BasePlayer", "m_flDuckAmount");
	NETVAR(Vector, m_angEyeAngles, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(int, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(bool, m_bHasHeavyArmor, "DT_CSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(bool, m_bIsScoped, "DT_CSPlayer", "m_bIsScoped");;
	NETVAR(float, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int32_t, m_iHealth, "DT_BasePlayer", "m_iHealth");
	NETVAR(int32_t, m_lifeState, "DT_BasePlayer", "m_lifeState");
	NETVAR(int32_t, m_fFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(int32_t, m_nTickBase, "DT_BasePlayer", "m_nTickBase");
	NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(Vector, m_viewPunchAngle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(Vector, m_aimPunchAngle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(CHandle<C_BaseViewModel>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(float, m_flMaxspeed, "DT_BasePlayer", "m_flMaxspeed");
	NETVAR(CHandle<C_BasePlayer>, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(float, m_flFlashMaxAlpha, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(int32_t, m_nHitboxSet, "DT_BaseAnimating", "m_nHitboxSet");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(int32_t, m_iAccount, "DT_CSPlayer", "m_iAccount");
	NETVAR(float, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(float, m_flCycle, "DT_ServerAnimationData", "m_flCycle");
	NETVAR(int, m_nSequence, "DT_BaseViewModel", "m_nSequence");
	PNETVAR(char, m_szLastPlaceName, "DT_BasePlayer", "m_szLastPlaceName");
	NETPROP(m_flLowerBodyYawTargetProp, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVARADDOFFS(float_t, m_flOldSimulationTime, "DT_BaseEntity", "m_flSimulationTime", 0x4);
	NETVAR(float_t, m_flNextAttack, "CBaseCombatCharacter", "m_flNextAttack");

	char* GetArmorName()
	{
		if (this->m_ArmorValue() > 0)
		{
			if (this->m_bHasHelmet())
				return "H+K";
			else
				return "K";
		}
	}
	uint32_t& GetMostRecentModelBoneCounter() // for fake matrix
	{
		return *reinterpret_cast<uint32_t*>(uintptr_t(this) + 0x2690);
	}
	float& GetLastBoneSetupTime() // for fake matrix
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + 0x2924);
	}



	vfunc(191, update_ik_locks(float curtime), void(__thiscall*)(void*, float))(curtime)
		vfunc(192, calculate_ik_locks(float curtime), void(__thiscall*)(void*, float))(curtime)

		CBoneCache* bone_cache() // for fake matrix
	{
		return reinterpret_cast<CBoneCache*>(uintptr_t(this) + 0x290C);
	}
	offset(get_bone_count(), int, 0x291C);
	matrix3x4_t* bone_cache2() {
		return FOFFSET(matrix3x4_t*, GetClientRenderable(), 0x290C);
	}
	offset(get_bone_cache_count(), int, 0x2910 + 0xC)
		NETVAR(Vector, m_angAbsAngles, "DT_BaseEntity", "m_angAbsAngles");
	NETVAR(Vector, m_angAbsOrigin, "DT_BaseEntity", "m_angAbsOrigin");
	NETVAR(float, m_flDuckSpeed, "DT_BaseEntity", "m_flDuckSpeed");
	NETVAR(float, m_flDuckAmount, "DT_BaseEntity", "m_flDuckAmount");

	std::array< float, 24 >& m_flPoseParameter()
	{
		static int _m_flPoseParameter = NetvarSys::Get().GetOffset("DT_BaseAnimating", "m_flPoseParameter");
		return *reinterpret_cast<std::array<float, 24>*>(reinterpret_cast<uintptr_t>(this) + _m_flPoseParameter);
	}


	void SetCurrentCommand(CUserCmd* cmd)
	{
		static int m_hConstraintEntity = NetvarSys::Get().GetOffset("DT_BasePlayer", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(reinterpret_cast<uintptr_t>(this) + m_hConstraintEntity - 0xC) = cmd;
	}

	PNETVAR(CHandle<C_BaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables, "DT_BaseCombatCharacter", "m_hMyWearables");
	c_base_handle* m_hMyWeapons()
	{
		return (c_base_handle*)((uintptr_t)this + 0x2DF8);
	}
	std::string GetName(bool console_safe);

	CUserCmd*& m_pCurrentCommand();
	/*gladiator v2*/
	void InvalidateBoneCache();

	bool& ClientAnimations();
	void Animations(bool active);
	AnimationLayer* GetAnimOverlays();
	AnimationLayer* GetAnimOverlay(int i);

	bool IsKnifeorNade();

	void SetAbsAngles(Vector angle);
	int GetSequenceActivity(int sequence);
	float get_max_desync_delta()
	{
		uintptr_t animstate = uintptr_t(g_LocalPlayer->GetPlayerAnimState());


		float duckammount = *(float*)(animstate + 0xA4);
		float speedfraction = std::fmax(0, std::fmin(*reinterpret_cast<float*>(animstate + 0xF8), 1));

		float speedfactor = std::fmax(0, std::fmin(1, *reinterpret_cast<float*> (animstate + 0xFC)));

		float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999)* speedfraction;
		float unk2 = unk1 + 1.1f;
		float unk3;

		if (duckammount > 0) {

			unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

		}
		else
			unk2 += ((duckammount * speedfactor) * (0.5f - 0.58f));

		unk3 = *(float*)(animstate + 0x334) * unk2;

		return unk3;
	}

	float MaxDesyncDelta();
	CBasePlayerAnimState* GetPlayerAnimState();
	void UpdateAnimationState(CBasePlayerAnimState* state, Vector angle);
	void ResetAnimationState(CBasePlayerAnimState* state);
	void CreateAnimationState(CBasePlayerAnimState* state);
	CCSGOPlayerAnimState* get_animation_state();

	float_t& m_surfaceFriction()
	{
		static unsigned int _m_surfaceFriction = Utils::FindInDataMap(GetPredDescMap(), "m_surfaceFriction");
		return *(float_t*)((uintptr_t)this + _m_surfaceFriction);
	}
	Vector& m_vecBaseVelocity()
	{
		static unsigned int _m_vecBaseVelocity = Utils::FindInDataMap(GetPredDescMap(), "m_vecBaseVelocity");
		return *(Vector*)((uintptr_t)this + _m_vecBaseVelocity);
	}
	bool IsEnemy()
	{
		return (this->m_iTeamNum() != g_LocalPlayer->m_iTeamNum());
	}
	Vector& m_vecAbsVelocity()
	{
		static unsigned int _m_vecAbsVelocity = Utils::FindInDataMap(GetPredDescMap(), "m_vecAbsVelocity");
		return *(Vector*)((uintptr_t)this + _m_vecAbsVelocity);
	}
	float_t& m_flMaxspeed()
	{
		static unsigned int _m_flMaxspeed = Utils::FindInDataMap(GetPredDescMap(), "m_flMaxspeed");
		return *(float_t*)((uintptr_t)this + _m_flMaxspeed);
	}

	int& eflags() {
		return OFFSET(int, 0xE4);
	}

	uint32_t& m_iEFlags()
	{
		static unsigned int _m_iEFlags = Utils::FindInDataMap(GetPredDescMap(), "m_iEFlags");
		return *(uint32_t*)((uintptr_t)this + _m_iEFlags);
	}
	int& get_effects()
	{
		static unsigned int _m_iEFlags = Utils::FindInDataMap(GetPredDescMap(), "m_fEffects");
		return *(int*)((uintptr_t)this + _m_iEFlags);
	}
	int NumOverlays()
	{
		return 15;
	}


	Vector        GetEyePos();
	player_info_t GetPlayerInfo();
	bool          IsAlive();
	bool		  IsFlashed();
	bool          HasC4();
	Vector        GetHitboxPos(int hitbox_id);
	Vector		  GetHitboxPos(int hitbox_id, matrix3x4_t* boneMatrix);
	mstudiobbox_t* GetHitbox(int hitbox_id);
	bool          GetHitboxPos(int hitbox, Vector& output);
	Vector        GetBonePos(int bone);
	bool          CanSeePlayer(C_BasePlayer* player, int hitbox);
	bool          CanSeePlayer(C_BasePlayer* player, Vector& pos);
	void UpdateClientSideAnimation();

	int m_nMoveType();
	Vector* GetVAngles();
	float_t m_flSpawnTime();

	bool IsValidTarget();

	studiohdr_t* GetModelPtr();

	void StandardBlendingRules(studiohdr_t* hdr, Vector* pos, quaternion* q, float_t curtime, int32_t boneMask);

	void BuildTransformations(studiohdr_t* hdr, Vector* pos, quaternion* q, const matrix3x4_t& cameraTransform, int32_t boneMask, byte* computed);

	CBoneAccessor* GetBoneAccessor();



	bool SetupBones2(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

	void FixSetupBones(matrix3x4_t* Matrix);

};

class C_BaseViewModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
	NETPROP(m_nSequence, "DT_BaseViewModel", "m_nSequence");

	inline DWORD GetOwner() {

		return *(PDWORD)((DWORD)this + NetvarSys::Get().GetOffset("DT_BaseViewModel", "m_hOwner"));
	}
	void SendViewModelMatchingSequence(int sequence);
};

class DT_CSPlayerResource
{
public:
	PNETVAR(int32_t, m_nActiveCoinRank, "DT_CSPlayerResource", "m_nActiveCoinRank");
	PNETVAR(int32_t, m_nMusicID, "DT_CSPlayerResource", "m_nMusicID");
	PNETVAR(int32_t, m_nPersonaDataPublicLevel, "DT_CSPlayerResource", "m_nPersonaDataPublicLevel");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsLeader, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsLeader");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsTeacher, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsTeacher");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsFriendly, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsFriendly");
	PNETVAR(int32_t, m_iCompetitiveRanking, "DT_CSPlayerResource", "m_iCompetitiveRanking");
	PNETVAR(int32_t, m_iCompetitiveWins, "DT_CSPlayerResource", "m_iCompetitiveWins");
	PNETVAR(int32_t, m_iPlayerVIP, "DT_CSPlayerResource", "m_iPlayerVIP");
	PNETVAR(int32_t, m_iMVPs, "DT_CSPlayerResource", "m_iMVPs");
	PNETVAR(int32_t, m_iScore, "DT_CSPlayerResource", "m_iScore");
};



class CBoneAccessor
{

public:

	inline matrix3x4_t* GetBoneArrayForWrite()
	{
		return m_pBones;
	}

	inline void SetBoneArrayForWrite(matrix3x4_t* bone_array)
	{
		m_pBones = bone_array;
	}

	alignas(16) matrix3x4_t* m_pBones;
	int32_t m_ReadableBones; // Which bones can be read.
	int32_t m_WritableBones; // Which bones can be written.
};

class CBoneCache
{
public:
	matrix3x4_t* m_pCachedBones;
	char pad[8];
	unsigned int m_CachedBoneCount;
};



class CCSBomb
{
public:

	template<class T>
	T GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}


	float GetC4BlowTime() {

		return GetValue < float >(0x2990);
	}


};



