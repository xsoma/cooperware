#include "ragebot.hpp"

#include "../globals/globals.hpp"
#include "lagcomp.h"
#include "autowall.hpp"

#include "resolver.hpp"

bool HeadHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_head;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_head;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_head;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_head;
	}
	else {
		return Variables.rageaimbot.head;
	}
}
bool NeckHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_neck;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_neck;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_neck;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_neck;
	}
	else {
		return Variables.rageaimbot.neck;
	}
}
bool StomachHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_stomach;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_stomach;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_stomach;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_stomach;
	}
	else {
		return Variables.rageaimbot.stomach;
	}
}
bool BodyHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_body;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_body;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_body;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_body;
	}
	else {
		return Variables.rageaimbot.body;
	}
}
bool PelvisHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_pelvis;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_pelvis;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_pelvis;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_pelvis;
	}
	else {
		return Variables.rageaimbot.pelvis;
	}
}
bool ChestHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_chest;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_chest;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_chest;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_chest;
	}
	else {
		return Variables.rageaimbot.chest;
	}
}
bool ArmsHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_arms;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_arms;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_arms;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_arms;
	}
	else {
		return Variables.rageaimbot.arms;
	}
}
bool LegsHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_legs;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_legs;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_legs;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_legs;
	}
	else {
		return Variables.rageaimbot.legs;
	}
}
bool ToesHitbox( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_toes;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_toes;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_toes;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_toes;
	}
	else {
		return Variables.rageaimbot.toes;
	}
}

bool Multipoint( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return false;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_multipoint;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_multipoint;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_multipoint;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_multipoint;
	}
	else {
		return Variables.rageaimbot.multipoint;
	}
}

float SynchronizeBodyScale( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return 0;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_bodyscale;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_bodyscale;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_bodyscale;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_bodyscale;
	}
	else {
		return Variables.rageaimbot.bodyscale;
	}
}
float SynchronizeHeadScale( )
{
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return 0;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_headscale;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_headscale;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_headscale;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_headscale;
	}
	else {
		return Variables.rageaimbot.headscale;
	}
}

float SynchronizeMinDamage( ) {
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return 0;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_mindamage;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_mindamage;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_mindamage;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_mindamage;
	}
	else {
		return Variables.rageaimbot.minimumdmage;
	}
}

float SynchronizeMinDamageVisible( ) {
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return 0;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_mindamagevisible;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_mindamagevisible;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_mindamagevisible;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_mindamagevisible;
	}
	else {
		return Variables.rageaimbot.minimumdmagevisible;
	}
}


bool visible( Vector& start, Vector& end, C_BasePlayer* player, C_BasePlayer* local )
{
	Ray_t ray;
	trace_t tr;
	ray.Init( start, end );

	CTraceFilter filter;
	filter.pSkip = local;

	g_EngineTrace->TraceRay( ray, MASK_SHOT, &filter, &tr );

	if ( tr.hit_entity == player || tr.fraction > 0.97f )
	{
		return true;
	}

	return false;
}

std::vector<Vector> GetMultiplePoi43ntsForHitbox( C_BasePlayer* local, C_BasePlayer* entity, int iHitbox, matrix3x4_t BoneMatrix [ 128 ] )
{
	auto VectorTransform_Wrapper = [ ] ( const Vector& in1, const matrix3x4_t& in2, Vector& out )
	{
		auto VectorTransform = [ ] ( const float* in1, const matrix3x4_t& in2, float* out )
		{
			auto DotProducts = [ ] ( const float* v1, const float* v2 )
			{
				return v1 [ 0 ] * v2 [ 0 ] + v1 [ 1 ] * v2 [ 1 ] + v1 [ 2 ] * v2 [ 2 ];
			};
			out [ 0 ] = DotProducts( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
			out [ 1 ] = DotProducts( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
			out [ 2 ] = DotProducts( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
		};
		VectorTransform( &in1.x, in2, &out.x );
	};

	studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel( entity->GetModel( ) );
	mstudiohitboxset_t* set = pStudioModel->GetHitboxSet( 0 );
	mstudiobbox_t* hitbox = set->GetHitbox( iHitbox );

	std::vector<Vector> vecArray;

	Vector max;
	Vector min;
	VectorTransform_Wrapper( hitbox->bbmax, BoneMatrix [ hitbox->bone ], max );
	VectorTransform_Wrapper( hitbox->bbmin, BoneMatrix [ hitbox->bone ], min );

	auto center = (min + max) * 0.5f;

	Vector CurrentAngles = Math::CalcAngle( center, local->GetEyePos( ) );

	Vector Forward;
	Math::AngleVectors( CurrentAngles, Forward );

	Vector Right = Forward.Cross( Vector( 0, 0, 1 ) );
	Vector Left = Vector( -Right.x, -Right.y, Right.z );

	Vector Top = Vector( 0, 0, 1 );
	Vector Bot = Vector( 0, 0, -1 );
	Vector v1 = Vector( Right.x / 2, Right.y, 0.6 );
	Vector v2 = Vector( Right.x, Right.y / 2, 0.6 );
	Vector v3 = Vector( Right.x / 2, Right.y / 2, 0.6 );
	Vector v4 = Vector( -Right.x / 2, -Right.y / 2, 0.6 );





	if ( Multipoint() )
	{
		switch ( iHitbox ) {
		case 0:
			for ( auto i = 0; i < 8; ++i )
			{
				vecArray.emplace_back( center );
			}
			vecArray.at( 1 ) += Top * (hitbox->m_flRadius * SynchronizeHeadScale( ));
			vecArray.at( 2 ) += Right * (hitbox->m_flRadius * SynchronizeHeadScale( ));
			vecArray.at( 3 ) += Left * (hitbox->m_flRadius * SynchronizeHeadScale( ));
			vecArray.at( 4 ) += v1 * (hitbox->m_flRadius * SynchronizeHeadScale( ));
			vecArray.at( 5 ) += v2 * (hitbox->m_flRadius * SynchronizeHeadScale( ));
			vecArray.at( 6 ) += v3 * (hitbox->m_flRadius * SynchronizeHeadScale( ));
			vecArray.at( 7 ) += v4 * (hitbox->m_flRadius * SynchronizeHeadScale( ));
			break;

		default:

			for ( auto i = 0; i < 3; ++i )
			{
				vecArray.emplace_back( center );
			}
			vecArray [ 1 ] += Right * (hitbox->m_flRadius * SynchronizeBodyScale( ));
			vecArray [ 2 ] += Left * (hitbox->m_flRadius * SynchronizeBodyScale( ));
			break;
		}

	}
	else
		vecArray.emplace_back( center );





	return vecArray;
}



void C_BasePlayer::FixSetupBones( matrix3x4_t* Matrix ) { // i took out alot of stuff from here because b1g secrets

	auto animstate = GetPlayerAnimState( );
	if ( !animstate )
		return;



	auto v5 = *( int* ) ((( uintptr_t ) this) + 0xF0);
	const auto Backup = *( int* ) (uintptr_t( this ) + ptrdiff_t( 0x272 ));
	auto v6 = *( uint8_t* ) ((( uintptr_t ) this) + 0x274);
	*( int* ) (uintptr_t( this ) + ptrdiff_t( 0x272 )) = -1;

	this->m_vecVelocity( ) = this->m_vecAbsVelocity( ) * animstate->m_flUnknownVelocityLean;

	this->SetupBones( Matrix, 126, 0x00000100 | 0x200, g_pGlobalVars->curtime );


	this->m_vecVelocity( ) = this->m_vecAbsVelocity( ); // restore velocity

	*( int* ) (uintptr_t( this ) + ptrdiff_t( 0x272 )) = Backup;
	*( int* ) ((( uintptr_t ) this) + 0xF0) = v5;
	*( uint8_t* ) ((( uintptr_t ) this) + 0x274) = v6;


}
bool is_visible( C_BasePlayer* ent ) {

	trace_t tr;
	CTraceFilter traceFilter;
	Ray_t ray;
	ray.Init( g_LocalPlayer->GetEyePos( ), ent->GetEyePos( ) );

	if ( g_LocalPlayer )
	{
		traceFilter.pSkip = g_LocalPlayer;
		g_EngineTrace->TraceRay( ray, MASK_VISIBLE, &traceFilter, &tr );
		if ( tr.hit_entity == ent || tr.fraction == 1.f )
			return true;
	}
	return false;
}
Vector hitscan( C_BasePlayer* entity )
{

	if ( !entity )
		return Vector( 0, 0, 0 );
	if ( !g_LocalPlayer )
		return Vector( 0, 0, 0 );
	Vector vector_best_point = Vector( 0, 0, 0 );

	std::vector<int> HitBoxesToScan;

	int maindamage;
	maindamage = is_visible( entity ) ? SynchronizeMinDamageVisible( ) : SynchronizeMinDamage( );

	shots_missed [ entity->EntIndex( ) ] = shots_fire [ entity->EntIndex( ) ] - shots_hit [ entity->EntIndex( ) ];



	if ( Variables.rageaimbot.baimafter3misses && missed3shots == true )
	{
		if ( ChestHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_CHEST );

		}

		if ( BodyHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_LOWER_CHEST );

		}

		if ( StomachHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_STOMACH );

		}
		if ( PelvisHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_PELVIS );
		}
	}
	else if ( GetAsyncKeyState( Variables.rageaimbot.baimkey ) )
	{
		if ( ChestHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_CHEST );

		}

		if ( BodyHitbox() )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_LOWER_CHEST );

		}

		if ( StomachHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_STOMACH );

		}
		if ( PelvisHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_PELVIS );
		}
	}
	else if ( Variables.rageaimbot.baimiflethal && entity->m_iHealth( ) <= maindamage )
	{
		if ( ChestHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_CHEST );

		}

		if ( BodyHitbox( ) )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_LOWER_CHEST );

		}

		if ( StomachHitbox() )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_STOMACH );

		}
		if ( PelvisHitbox() )
		{
			HitBoxesToScan.push_back( ( int ) HITBOX_PELVIS );
		}
	}
	else
	{
		if ( Variables.rageaimbot.safepoint )
		{
			if ( missed2shots == true )
			{

			}
			else
			{
				if ( HeadHitbox() )
					HitBoxesToScan.push_back( ( int ) HITBOX_NECK );
			}

			if ( NeckHitbox )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_NECK );

			}

			if ( ChestHitbox( ) )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_CHEST );

			}

			if ( BodyHitbox( ) )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_LOWER_CHEST );

			}

			if ( StomachHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_STOMACH );

			}
			if ( PelvisHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_PELVIS );
			}

			if ( LegsHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_LEFT_THIGH );
				HitBoxesToScan.push_back( ( int ) HITBOX_RIGHT_THIGH );
			}


		}
		else
		{

			if ( NeckHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_NECK );
			}
			if ( HeadHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_HEAD );
			}

			if ( ChestHitbox( ) )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_UPPER_CHEST );
				HitBoxesToScan.push_back( ( int ) HITBOX_CHEST );
				HitBoxesToScan.push_back( ( int ) HITBOX_LOWER_CHEST );

			}
			if ( StomachHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_STOMACH );
			}
			if ( PelvisHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_PELVIS );
			}
			if ( ArmsHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_LEFT_UPPER_ARM );
				HitBoxesToScan.push_back( ( int ) HITBOX_RIGHT_UPPER_ARM );

			}
			if ( LegsHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_LEFT_THIGH );
				HitBoxesToScan.push_back( ( int ) HITBOX_RIGHT_THIGH );
				HitBoxesToScan.push_back( ( int ) HITBOX_LEFT_CALF );
				HitBoxesToScan.push_back( ( int ) HITBOX_RIGHT_CALF );

			}
			if ( ToesHitbox() )
			{
				HitBoxesToScan.push_back( ( int ) HITBOX_LEFT_FOOT );
				HitBoxesToScan.push_back( ( int ) HITBOX_RIGHT_FOOT );
			}

		}

	}




	matrix3x4_t bones [ 128 ];
	AnimFix::setup_bones( entity, 256, g_pGlobalVars->curtime, bones );
	for ( auto hitbox_id : HitBoxesToScan )
	{
		auto point = GetMultiplePoi43ntsForHitbox( g_LocalPlayer, entity, hitbox_id, bones );

		for ( int i = 0; i < point.size( ); i++ )
		{
			auto hit_vector = point.at( i );


			float damage = AutoWall::CanHit( hit_vector );

			if ( damage > maindamage )
			{

				maindamage = damage;

				vector_best_point = hit_vector;


				if ( maindamage >= entity->m_iHealth( ) )
					return vector_best_point;
			}



		}

		//}

	}


	return vector_best_point;

}

namespace Aimbot_R
{
	float GetCurtime346( )
	{
		if ( !g_LocalPlayer )
			return -1;

		return static_cast< float >(g_LocalPlayer->m_nTickBase( )) * g_pGlobalVars->interval_per_tick;
	}

	bool  can_shoot( CUserCmd* cmd )
	{

		if ( !g_LocalPlayer ) return false;

		if ( !g_LocalPlayer->m_hActiveWeapon( ).Get( ) ) return false;



		return (g_LocalPlayer->m_hActiveWeapon( ).Get( )->m_flNextPrimaryAttack( ) < GetCurtime346( )) && (g_LocalPlayer->m_flNextAttack( ) < GetCurtime346( ));

	}

	float hitchance( C_BaseCombatWeapon* weapon )
	{

		if ( !weapon )
			return 0;
		float hitchance = 101;
		float inaccuracy = weapon->GetInaccuracy( );

		if ( inaccuracy == 0 )
			inaccuracy = 0.0000001;

		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;

		return hitchance;
	}
	inline float FastSqrt222( float x )
	{
		unsigned int i = *( unsigned int* ) &x;
		i += 127 << 23;
		i >>= 1;
		return *( float* ) &i;
	}
#define square( x ) ( x * x )
	void ClampMov2363526263ement( CUserCmd* pCommand, float fMaxSpeed )
	{
		if ( fMaxSpeed <= 0.f )
			return;
		float fSpeed = ( float ) (FastSqrt222( square( pCommand->forwardmove ) + square( pCommand->sidemove ) + square( pCommand->upmove ) ));
		if ( fSpeed <= 0.f )
			return;
		if ( pCommand->buttons & IN_DUCK )
			fMaxSpeed *= 2.94117647f; // TO DO: Maybe look trough the leaked sdk for an exact value since this is straight out of my ass...
		if ( fSpeed <= fMaxSpeed )
			return;
		float fRatio = fMaxSpeed / fSpeed;
		pCommand->forwardmove *= fRatio;
		pCommand->sidemove *= fRatio;
		pCommand->upmove *= fRatio;
	}



}




























































