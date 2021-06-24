#include "csgostructs.hpp"
#include "../Helpers/Math.hpp"
#include "../Helpers/Utils.hpp"

studiohdr_t* C_BasePlayer::GetModelPtr( )
{
	return *( studiohdr_t** ) (( uintptr_t ) this + 0x294C);
}

void C_BasePlayer::StandardBlendingRules( studiohdr_t* hdr, Vector* pos, quaternion* q, float_t curtime, int32_t boneMask )
{
	typedef void( __thiscall* o_StandardBlendingRules )(void*, studiohdr_t*, Vector*, quaternion*, float_t, int32_t);
	CallVFunction<o_StandardBlendingRules>( this, 205 )(this, hdr, pos, q, curtime, boneMask);
}

void C_BasePlayer::BuildTransformations( studiohdr_t* hdr, Vector* pos, quaternion* q, const matrix3x4_t& cameraTransform, int32_t boneMask, byte* computed )
{
	typedef void( __thiscall* o_BuildTransformations )(void*, studiohdr_t*, Vector*, quaternion*, const matrix3x4_t&, int32_t, byte*);
	CallVFunction<o_BuildTransformations>( this, 189 )(this, hdr, pos, q, cameraTransform, boneMask, computed);
}
CBoneAccessor* C_BasePlayer::GetBoneAccessor( )
{
	return ( CBoneAccessor* ) (( uintptr_t ) this + 0x26A8);
}

bool C_BaseEntity::IsPlayer( )
{
	//index: 152
	//ref: "effects/nightvision"
	//sig: 8B 92 ? ? ? ? FF D2 84 C0 0F 45 F7 85 F6
	return CallVFunction<bool( __thiscall* )(C_BaseEntity*)>( this, 157 )(this);
}

bool C_BaseEntity::IsLoot( ) {
	return GetClientClass( )->m_ClassID == ClassId_CPhysPropAmmoBox ||
		GetClientClass( )->m_ClassID == ClassId_CPhysPropLootCrate ||
		GetClientClass( )->m_ClassID == ClassId_CPhysPropRadarJammer ||
		GetClientClass( )->m_ClassID == ClassId_CPhysPropWeaponUpgrade ||
		GetClientClass( )->m_ClassID == ClassId_CDrone ||
		GetClientClass( )->m_ClassID == ClassId_CDronegun ||
		GetClientClass( )->m_ClassID == ClassId_CItem_Healthshot ||
		GetClientClass( )->m_ClassID == ClassId_CItemCash ||
		GetClientClass( )->m_ClassID == ClassId_CBumpMine;
}

bool C_BaseEntity::IsWeapon( )
{
	//index: 160
	//ref: "CNewParticleEffect::DrawModel"
	//sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
	return CallVFunction<bool( __thiscall* )(C_BaseEntity*)>( this, 165 )(this);
}


bool C_BaseEntity::IsPlantedC4( )
{
	return GetClientClass( )->m_ClassID == ClassId_CPlantedC4;
}

bool C_BaseEntity::IsDefuseKit( )
{
	return GetClientClass( )->m_ClassID == ClassId_CBaseAnimating;
}

CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData( )
{
	return CallVFunction<CCSWeaponInfo* (__thiscall*)(void*)>( this, 460 )(this);
	/*
	static auto fnGetWpnData
	= reinterpret_cast<CCSWeaponInfo*(__thiscall*)(void*)>(
	Utils::PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B")
	);
	return fnGetWpnData(this);*/
}

bool C_BaseCombatWeapon::HasBullets( )
{
	return !IsReloading( ) && m_iClip1( ) > 0;
}
bool C_BaseCombatWeapon::IsWeaponNonAim( )
{
	int idx = m_iItemDefinitionIndex( );

	return (idx == WEAPON_C4 || idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_BAYONET || idx == WEAPON_KNIFE_BUTTERFLY || idx == WEAPON_KNIFE_FALCHION
		|| idx == WEAPON_KNIFE_FLIP || idx == WEAPON_KNIFE_GUT || idx == WEAPON_KNIFE_KARAMBIT || idx == WEAPON_KNIFE_M9_BAYONET || idx == WEAPON_KNIFE_PUSH
		|| idx == WEAPON_KNIFE_SURVIVAL_BOWIE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_TACTICAL || idx == WEAPON_FLASHBANG || idx == WEAPON_HEGRENADE
		|| idx == WEAPON_SMOKEGRENADE || idx == WEAPON_MOLOTOV || idx == WEAPON_DECOY || idx == WEAPON_INCGRENADE);
}
bool C_BaseCombatWeapon::CanFire( )
{
	static decltype(this) stored_weapon = nullptr;
	static auto stored_tick = 0;
	if ( stored_weapon != this || stored_tick >= g_LocalPlayer->m_nTickBase( ) ) {
		stored_weapon = this;
		stored_tick = g_LocalPlayer->m_nTickBase( );
		return false; //cannot shoot first tick after switch
	}

	if ( IsReloading( ) || m_iClip1( ) <= 0 || !g_LocalPlayer )
		return false;

	auto flServerTime = g_LocalPlayer->m_nTickBase( ) * g_pGlobalVars->interval_per_tick;

	return m_flNextPrimaryAttack( ) <= flServerTime;
}

bool C_BaseCombatWeapon::IsGrenade( )
{
	return GetCSWeaponData( )->iWeaponType == WEAPONTYPE_GRENADE;
}

bool C_BaseCombatWeapon::IsZeus( )
{
	return this->m_Item( ).m_iItemDefinitionIndex( ) == WEAPON_TASER;
}

bool C_BaseCombatWeapon::IsGun( )
{
	switch ( GetCSWeaponData( )->iWeaponType )
	{
	case WEAPONTYPE_C4:
		return false;
	case WEAPONTYPE_GRENADE:
		return false;
	case WEAPONTYPE_KNIFE:
		return false;
	case WEAPONTYPE_UNKNOWN:
		return false;
	default:
		return true;
	}
}

bool C_BaseCombatWeapon::IsKnife( )
{
	if ( this->m_Item( ).m_iItemDefinitionIndex( ) == WEAPON_TASER ) return false;
	return GetCSWeaponData( )->iWeaponType == WEAPONTYPE_KNIFE;
}

bool C_BaseCombatWeapon::IsRifle( )
{
	switch ( GetCSWeaponData( )->iWeaponType )
	{
	case WEAPONTYPE_RIFLE:
		return true;
	case WEAPONTYPE_SUBMACHINEGUN:
		return true;
	case WEAPONTYPE_SHOTGUN:
		return true;
	case WEAPONTYPE_MACHINEGUN:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsPistol( )
{
	switch ( GetCSWeaponData( )->iWeaponType )
	{
	case WEAPONTYPE_PISTOL:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsSniper( )
{
	switch ( GetCSWeaponData( )->iWeaponType )
	{
	case WEAPONTYPE_SNIPER_RIFLE:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsReloading( )
{
	static auto inReload = *( uint32_t* ) (Utils::PatternScan( GetModuleHandleW( L"client.dll" ), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90" ) + 2);
	return *( bool* ) (( uintptr_t ) this + inReload);
}

float C_BaseCombatWeapon::GetInaccuracy( )
{
	return CallVFunction<float( __thiscall* )(void*)>( this, 482 )(this);
}

float C_BaseCombatWeapon::GetSpread( )
{
	return CallVFunction<float( __thiscall* )(void*)>( this, 452 )(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty( )
{
	CallVFunction<void( __thiscall* )(void*)>( this, 483 )(this);
}

bool C_BaseCombatWeapon::IsScopeable( )
{
	int iWeaponID = m_iItemDefinitionIndex( );
	return (iWeaponID == 38 || iWeaponID == 11 || iWeaponID == 9 || iWeaponID == 40 || iWeaponID == 8 || iWeaponID == WEAPON_SG553);
}

CUtlVector<IRefCounted*>& C_BaseCombatWeapon::m_CustomMaterials( )
{
	static auto inReload = *( uint32_t* ) (Utils::PatternScan( GetModuleHandleW( L"client.dll" ), "83 BE ? ? ? ? ? 7F 67" ) + 2) - 12;
	return *(CUtlVector<IRefCounted*>*)(( uintptr_t ) this + inReload);
}

bool* C_BaseCombatWeapon::m_bCustomMaterialInitialized( )
{
	static auto currentCommand = *( uint32_t* ) (Utils::PatternScan( GetModuleHandleW( L"client.dll" ), "C6 86 ? ? ? ? ? FF 50 04" ) + 2);
	return ( bool* ) (( uintptr_t ) this + currentCommand);
}
bool C_BaseCombatWeapon::is_non_aim( ) {
	int idx = m_iItemDefinitionIndex( );

	return (idx == WEAPON_C4 || idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_BAYONET || idx == WEAPON_KNIFE_BUTTERFLY || idx == WEAPON_KNIFE_FALCHION
		|| idx == WEAPON_KNIFE_FLIP || idx == WEAPON_KNIFE_GUT || idx == WEAPON_KNIFE_KARAMBIT || idx == WEAPON_KNIFE_M9_BAYONET || idx == WEAPON_KNIFE_PUSH
		|| idx == WEAPON_KNIFE_SURVIVAL_BOWIE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_TACTICAL || idx == WEAPON_FLASHBANG || idx == WEAPON_HEGRENADE
		|| idx == WEAPON_SMOKEGRENADE || idx == WEAPON_MOLOTOV || idx == WEAPON_DECOY || idx == WEAPON_INCGRENADE);
}

bool C_BaseCombatWeapon::is_empty( ) {
	return m_iClip1( ) <= 0;
}

bool C_BaseCombatWeapon::can_fire( ) {
	if ( is_empty( ) ) {
		return false;
	}

	if ( !g_LocalPlayer ) {
		return false;
	}

	return (m_flNextPrimaryAttack( ) < g_pGlobalVars->curtime) && (g_LocalPlayer->m_flNextAttack( ) < g_pGlobalVars->curtime);
}


void IKContext::init( studiohdr_t* hdr, Vector& angles, Vector& origin, float curtime, int framecount, int boneMask ) {
	static const auto ik_init_address = Utils::FindSignature( "client.dll", "55 8B EC 83 EC 08 8B 45 08 56 57 8B F9 8D 8F" );
	reinterpret_cast< void( __thiscall* )(IKContext*, studiohdr_t*, Vector&, Vector&, float, int, int) >(ik_init_address)(this, hdr, angles, origin, curtime, framecount, boneMask);
}

void IKContext::update_targets( Vector* pos, quaternion* q, matrix3x4_t* bone_array, byte* computed ) {
	static const auto update_targets_address = Utils::FindSignature( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 33 D2" );
	reinterpret_cast< void( __thiscall* )(IKContext*, Vector*, quaternion*, matrix3x4_t*, byte*) >(update_targets_address)(this, pos, q, bone_array, computed);
}

void IKContext::solve_dependencies( Vector* pos, quaternion* q, matrix3x4_t* bone_array, byte* computed ) {
	static const auto solve_dependencies_address = Utils::FindSignature( "client.dll", "55 8B EC 83 E4 F0 81 EC ? ? ? ? 8B 81" );
	reinterpret_cast< void( __thiscall* )(IKContext*, Vector*, quaternion*, matrix3x4_t*, byte*) >(solve_dependencies_address)(this, pos, q, bone_array, computed);
}

CCSGOPlayerAnimState* C_BasePlayer::get_animation_state( )
{
	if ( this == nullptr ) return nullptr;

	return *( CCSGOPlayerAnimState** )( ( DWORD )this + 0x3914 );
}

std::string C_BasePlayer::GetName( bool console_safe )
{
	// Cleans player's name so we don't get new line memes. Use this everywhere you get the players name.
	// Also, if you're going to use the console for its command and use the players name, set console_safe.
	player_info_t pinfo = this->GetPlayerInfo( );

	char* pl_name = pinfo.szName;
	char buf [ 128 ];
	int c = 0;

	for ( int i = 0; pl_name [ i ]; ++i )
	{
		if ( c >= sizeof( buf ) - 1 )
			break;

		switch ( pl_name [ i ] )
		{
		case '"': if ( console_safe ) break;
		case '\\':
		case ';': if ( console_safe ) break;
		case '\n':
			break;
		default:
			buf [ c++ ] = pl_name [ i ];
		}
	}

	buf [ c ] = '\0';
	return std::string( buf );
}

CUserCmd*& C_BasePlayer::m_pCurrentCommand( )
{
	static auto currentCommand = *( uint32_t* ) (Utils::PatternScan( GetModuleHandleW( L"client.dll" ), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF" ) + 2);
	return *( CUserCmd** ) (( uintptr_t ) this + currentCommand);
}

bool& C_BasePlayer::ClientAnimations( )
{
	return *reinterpret_cast< bool* >(uintptr_t( this ) + NetvarSys::Get( ).GetOffset( "DT_BaseAnimating", "m_bClientSideAnimation" ));
}

void C_BasePlayer::Animations( bool active )
{
	if ( this->GetPlayerAnimState( )->m_iLastClientSideAnimationUpdateFramecount == g_pGlobalVars->framecount )
		this->GetPlayerAnimState( )->m_iLastClientSideAnimationUpdateFramecount = g_pGlobalVars->framecount - 1;

	ClientAnimations( ) = active;
}

AnimationLayer* C_BasePlayer::GetAnimOverlays( )
{
	return *( AnimationLayer** ) (( DWORD ) this + 0x2980);
}

AnimationLayer* C_BasePlayer::GetAnimOverlay( int i )
{
	if ( i < 15 ) return &GetAnimOverlays()[i];
	return nullptr;
}

bool C_BasePlayer::IsKnifeorNade( )
{
	if ( !this )
		return false;
	if ( !this->IsAlive( ) )
		return false;

	C_BaseCombatWeapon* pWeapon = ( C_BaseCombatWeapon* ) this->m_hActiveWeapon( );

	if ( !pWeapon )
		return false;

	std::string WeaponName = pWeapon->GetName( );

	if ( WeaponName == "weapon_knife" )
		return true;
	else if ( WeaponName == "weapon_incgrenade" )
		return true;
	else if ( WeaponName == "weapon_decoy" )
		return true;
	else if ( WeaponName == "weapon_flashbang" )
		return true;
	else if ( WeaponName == "weapon_hegrenade" )
		return true;
	else if ( WeaponName == "weapon_smokegrenade" )
		return true;
	else if ( WeaponName == "weapon_molotov" )
		return true;

	return false;
}

float C_BasePlayer::FireRate( )
{
	if ( !this )
		return 0.f;
	if ( !this->IsAlive( ) )
		return 0.f;
	if ( this->IsKnifeorNade( ) )
		return 0.f;

	C_BaseCombatWeapon* pWeapon = ( C_BaseCombatWeapon* ) this->m_hActiveWeapon( );

	if ( !pWeapon )
		return false;

	std::string WeaponName = pWeapon->GetName( );

	if ( WeaponName == "weapon_glock" )
		return 0.15f;
	else if ( WeaponName == "weapon_hkp2000" )
		return 0.169f;
	else if ( WeaponName == "weapon_p250" )//the cz and p250 have the same name idky same with other guns
		return 0.15f;
	else if ( WeaponName == "weapon_tec9" )
		return 0.12f;
	else if ( WeaponName == "weapon_elite" )
		return 0.12f;
	else if ( WeaponName == "weapon_fiveseven" )
		return 0.15f;
	else if ( WeaponName == "weapon_deagle" )
		return 0.224f;
	else if ( WeaponName == "weapon_nova" )
		return 0.882f;
	else if ( WeaponName == "weapon_sawedoff" )
		return 0.845f;
	else if ( WeaponName == "weapon_mag7" )
		return 0.845f;
	else if ( WeaponName == "weapon_xm1014" )
		return 0.35f;
	else if ( WeaponName == "weapon_mac10" )
		return 0.075f;
	else if ( WeaponName == "weapon_ump45" )
		return 0.089f;
	else if ( WeaponName == "weapon_mp9" )
		return 0.070f;
	else if ( WeaponName == "weapon_bizon" )
		return 0.08f;
	else if ( WeaponName == "weapon_mp7" )
		return 0.08f;
	else if ( WeaponName == "weapon_p90" )
		return 0.070f;
	else if ( WeaponName == "weapon_galilar" )
		return 0.089f;
	else if ( WeaponName == "weapon_ak47" )
		return 0.1f;
	else if ( WeaponName == "weapon_sg556" )
		return 0.089f;
	else if ( WeaponName == "weapon_m4a1" )
		return 0.089f;
	else if ( WeaponName == "weapon_aug" )
		return 0.089f;
	else if ( WeaponName == "weapon_m249" )
		return 0.08f;
	else if ( WeaponName == "weapon_negev" )
		return 0.0008f;
	else if ( WeaponName == "weapon_ssg08" )
		return 1.25f;
	else if ( WeaponName == "weapon_awp" )
		return 1.463f;
	else if ( WeaponName == "weapon_g3sg1" )
		return 0.25f;
	else if ( WeaponName == "weapon_scar20" )
		return 0.25f;
	else if ( WeaponName == "weapon_mp5sd" )
		return 0.08f;
	else
		return .0f;

}

void C_BasePlayer::SetAbsAngles( Vector angle )
{
	using SetAbsAnglesFn = void( __thiscall* )(void*, const Vector& Angles);
	static SetAbsAnglesFn SetAbsAngles = ( SetAbsAnglesFn ) Utils::PatternScan( GetModuleHandleA( "client.dll" ), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" );
	SetAbsAngles( this, angle );
}

int C_BasePlayer::GetSequenceActivity( int sequence )
{
	auto hdr = g_MdlInfo->GetStudiomodel( this->GetModel( ) );

	if ( !hdr )
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast< int( __fastcall* )(void*, studiohdr_t*, int) >(Utils::PatternScan( GetModuleHandle( "client.dll" ), "55 8B EC 83 7D 08 FF 56 8B F1 74 3D" ));

	return get_sequence_activity( this, hdr, sequence );
}

float C_BasePlayer::MaxDesyncDelta( ) {

	if ( g_LocalPlayer )
		return false;
	auto animstate = uintptr_t( g_LocalPlayer->GetPlayerAnimState( ) );

	float duckammount = *( float* ) (animstate + 0xA4);
	float speedfraction = std::fmax( 0, std::fmin( *reinterpret_cast< float* >(animstate + 0xF8), 1 ) );

	float speedfactor = std::fmax( 0, std::fmin( 1, *reinterpret_cast< float* > (animstate + 0xFC) ) );

	float unk1 = ((*reinterpret_cast< float* > (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if ( duckammount > 0 ) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}

	unk3 = *( float* ) (animstate + 0x334) * unk2;

	return unk3;
}

CBasePlayerAnimState* C_BasePlayer::GetPlayerAnimState( )
{
	return *( CBasePlayerAnimState** ) (( DWORD ) this + 0x3914);
}

void C_BasePlayer::UpdateAnimationState(CBasePlayerAnimState* state, Vector angle)
{
	static auto UpdateAnimState = Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

	if (!UpdateAnimState)
		return;

	__asm {
		push 0
	}

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}


void C_BasePlayer::ResetAnimationState(CBasePlayerAnimState* state)
{
	using ResetAnimState_t = void(__thiscall*)(CBasePlayerAnimState*);
	static auto reset_anim_state = (ResetAnimState_t)Utils::PatternScan(GetModuleHandleA("client.dll"), "56 6A 01 68 ? ? ? ? 8B F1");
	if (!reset_anim_state)
		return;

	reset_anim_state(state);
}

void C_BasePlayer::CreateAnimationState(CBasePlayerAnimState* state)
{
	using create_anim_state_t = void(__thiscall*) (CBasePlayerAnimState*, C_BasePlayer*);
	static auto create_anim_state = reinterpret_cast<create_anim_state_t> (Utils::FindSignature("client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));

	if (!create_anim_state)
		return;

	create_anim_state(state, this);
}

Vector C_BasePlayer::GetEyePos( )
{
	return m_vecOrigin( ) + m_vecViewOffset( );
}

player_info_t C_BasePlayer::GetPlayerInfo( )
{
	player_info_t info;
	g_EngineClient->GetPlayerInfo( EntIndex( ), &info );
	return info;
}

bool C_BasePlayer::IsAlive( )
{
	return this->m_iHealth( ) > 0;
}

bool C_BasePlayer::IsFlashed( )
{
	static auto m_flFlashMaxAlpha = NetvarSys::Get( ).GetOffset( "DT_CSPlayer", "m_flFlashMaxAlpha" );
	return *( float* ) (( uintptr_t ) this + m_flFlashMaxAlpha - 0x8) > 200.0;
}

bool C_BasePlayer::HasC4( )
{
	static auto fnHasC4
		= reinterpret_cast< bool( __thiscall* )(void*) >(
			Utils::PatternScan( GetModuleHandleW( L"client.dll" ), "56 8B F1 85 F6 74 31" )
			);

	return fnHasC4( this );
}

Vector C_BasePlayer::GetHitboxPos( int hitbox_id )
{
	matrix3x4_t boneMatrix [ MAXSTUDIOBONES ];

	if ( SetupBones( boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f ) ) {
		auto studio_model = g_MdlInfo->GetStudiomodel( GetModel( ) );
		if ( studio_model ) {
			auto hitbox = studio_model->GetHitboxSet( 0 )->GetHitbox( hitbox_id );
			if ( hitbox ) {
				auto
					min = Vector {},
					max = Vector {};

				Math::VectorTransform( hitbox->bbmin, boneMatrix [ hitbox->bone ], min );
				Math::VectorTransform( hitbox->bbmax, boneMatrix [ hitbox->bone ], max );

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector {};
}
Vector C_BasePlayer::GetHitboxPos( int hitbox_id, matrix3x4_t* boneMatrix )
{
	auto studio_model = g_MdlInfo->GetStudiomodel( GetModel( ) );
	if ( studio_model ) {
		auto hitbox = studio_model->GetHitboxSet( 0 )->GetHitbox( hitbox_id );
		if ( hitbox ) {
			auto
				min = Vector {},
				max = Vector {};

			Math::VectorTransform( hitbox->bbmin, boneMatrix [ hitbox->bone ], min );
			Math::VectorTransform( hitbox->bbmax, boneMatrix [ hitbox->bone ], max );

			return (min + max) / 2.0f;
		}
	}
	return Vector {};
}

mstudiobbox_t* C_BasePlayer::GetHitbox( int hitbox_id )
{
	matrix3x4_t boneMatrix [ MAXSTUDIOBONES ];

	if ( SetupBones( boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f ) ) {
		auto studio_model = g_MdlInfo->GetStudiomodel( GetModel( ) );
		if ( studio_model ) {
			auto hitbox = studio_model->GetHitboxSet( 0 )->GetHitbox( hitbox_id );
			if ( hitbox ) {
				return hitbox;
			}
		}
	}
	return nullptr;
}

bool C_BasePlayer::GetHitboxPos( int hitbox, Vector& output )
{
	if ( hitbox >= HITBOX_MAX )
		return false;

	const model_t* model = this->GetModel( );
	if ( !model )
		return false;

	studiohdr_t* studioHdr = g_MdlInfo->GetStudiomodel( model );
	if ( !studioHdr )
		return false;

	matrix3x4_t matrix [ MAXSTUDIOBONES ];
	if ( !this->SetupBones( matrix, MAXSTUDIOBONES, 0x100, 0 ) )
		return false;

	mstudiobbox_t* studioBox = studioHdr->GetHitboxSet( 0 )->GetHitbox( hitbox );
	if ( !studioBox )
		return false;

	Vector min, max;

	Math::VectorTransform( studioBox->bbmin, matrix [ studioBox->bone ], min );
	Math::VectorTransform( studioBox->bbmax, matrix [ studioBox->bone ], max );

	output = (min + max) * 0.5f;

	return true;
}

Vector C_BasePlayer::GetBonePos( int bone )
{
	matrix3x4_t boneMatrix [ MAXSTUDIOBONES ];
	if ( SetupBones( boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f ) ) {
		return boneMatrix [ bone ].at( 3 );
	}
	return Vector {};
}

bool C_BasePlayer::CanSeePlayer( C_BasePlayer* player, int hitbox )
{
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto endpos = player->GetHitboxPos( hitbox );

	ray.Init( GetEyePos( ), endpos );
	g_EngineTrace->TraceRay( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

bool C_BasePlayer::CanSeePlayer( C_BasePlayer* player, Vector& pos )
{
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	ray.Init( GetEyePos( ), pos );
	g_EngineTrace->TraceRay( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

void C_BasePlayer::UpdateClientSideAnimation( )
{
	return CallVFunction<void( __thiscall* )(void*)>( this, 223 )(this);
}

void C_BasePlayer::InvalidateBoneCache( )
{
	static auto invalidate_bone_bache = Utils::FindSignature( "client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ) + 10;

	*(std::uint32_t*) ((std::uintptr_t) this + 0x2924) = 0xFF7FFFFF;
	*(std::uint32_t*) ((std::uintptr_t) this + 0x2690) = **(std::uintptr_t**) invalidate_bone_bache - 1;
}

int C_BasePlayer::m_nMoveType( )
{
	return *( int* ) (( uintptr_t ) this + 0x25C);
}

Vector* C_BasePlayer::GetVAngles( )
{
	static auto deadflag = NetvarSys::Get( ).GetOffset( "DT_BasePlayer", "deadflag" );
	return ( Vector* ) (( uintptr_t ) this + deadflag + 0x4);
}

void C_BaseAttributableItem::SetGloveModelIndex( int modelIndex )
{
	return CallVFunction<void( __thiscall* )(void*, int)>( this, 75 )(this, modelIndex);
}

void C_BaseViewModel::SendViewModelMatchingSequence( int sequence )
{
	return CallVFunction<void( __thiscall* )(void*, int)>( this, 245 )(this, sequence);
}

float_t C_BasePlayer::m_flSpawnTime( )
{
	return *( float_t* ) (( uintptr_t ) this + 0xA360);
}


bool C_BasePlayer::IsValidTarget( ) {

	if ( !this || this == nullptr )
		return false;

	ClientClass* pClass = ( ClientClass* ) this->GetClientClass( ); // Needed to check clientclass after nullptr check that was causing a crash

	if ( this == g_LocalPlayer )
		return false;

	if ( pClass->m_ClassID != ClassId_CCSPlayer)
		return false;

	if ( this->m_iTeamNum( ) == g_LocalPlayer->m_iTeamNum( ) )
		return false;

	if ( this->IsDormant( ) )
		return false;

	if ( !this->IsAlive( ) )
		return false;

	if ( this->m_bGunGameImmunity( ) )
		return false;

	return true;
}

















































