#include "ragebot.hpp"
#include "../hooks/hooks.hpp"
#include "autowall.hpp"
#include "..\globals\globals.hpp"
#include "misc.hpp"
#include "animfix.h"
#include "lagcomp.h"
#include "resolver.hpp"

#include "predictionsystem.h"
#include "antiaim.h"


float SynchronizeHitchance( ) {
	auto me = g_LocalPlayer;
	auto cmd = globals::pCmd;

	if ( !me->m_hActiveWeapon( ) )
		return 0;

	if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SCAR20 || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_G3SG1 ) {
		return Variables.rageaimbot.auto_hitchance;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_SSG08 ) {
		return Variables.rageaimbot.scout_hitchance;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_AWP ) {
		return Variables.rageaimbot.awp_hitchance;
	}
	else if ( me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_REVOLVER || me->m_hActiveWeapon( )->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_DEAGLE ) {
		return Variables.rageaimbot.heavy_pistol_hitchance;
	}
	else {
		return Variables.rageaimbot.hitchance;
	}
}

float accepted_inaccuracy( C_BaseCombatWeapon* weapon ) //ayyyyyywareeee
{
	auto local_player = g_LocalPlayer;

	if ( !local_player )
		return 0;

	if ( !weapon )
		return 0;

	if ( weapon->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_TASER )
		return 0;

	float hitchance = 101; //lol idk why, its pasted anyway so w/e
	float inaccuracy = weapon->GetInaccuracy( );

	if ( inaccuracy == 0 )
		inaccuracy = 0.0000001;

	inaccuracy = 1 / inaccuracy;
	hitchance = inaccuracy;

	return hitchance;
}

void NormalizeAngles( Vector& angles )
{
	for ( auto i = 0; i < 3; i++ ) {
		while ( angles [ i ] < -180.0f ) angles [ i ] += 360.0f;
		while ( angles [ i ] > 180.0f ) angles [ i ] -= 360.0f;
	}
}
Vector CrossProduct( const Vector& a, const Vector& b )
{
	return Vector( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

void VectorAngles( const Vector& forward, Vector& up, Vector& angles )
{
	Vector left = CrossProduct( up, forward );
	left.NormalizeInPlace( );

	float forwardDist = forward.Length2D( );

	if ( forwardDist > 0.001f )
	{
		angles.x = atan2f( -forward.z, forwardDist ) * 180 / PI_F;
		angles.y = atan2f( forward.y, forward.x ) * 180 / PI_F;

		float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles.z = atan2f( left.z, upZ ) * 180 / PI_F;
	}
	else
	{
		angles.x = atan2f( -forward.z, forwardDist ) * 180 / PI_F;
		angles.y = atan2f( -left.x, left.y ) * 180 / PI_F;
		angles.z = 0;
	}
}
float RandomFloat( float min, float max )
{
	static auto ranFloat = reinterpret_cast< float(*)(float, float) >(GetProcAddress( GetModuleHandle( "vstdlib.dll" ), "RandomFloat" ));
	if ( ranFloat )
		return ranFloat( min, max );
	else
		return 0.f;
}

bool HitChance( Vector angles, C_BasePlayer* ent, float chance )
{
	auto weapon = g_LocalPlayer->m_hActiveWeapon( ).Get( );

	if ( !weapon )
		return false;

	Vector forward, right, up;
	Vector src = g_LocalPlayer->GetEyePos( );
	Math::AngleVectors( angles, forward, right, up );

	int cHits = 0;
	int cNeededHits = static_cast< int >(150.f * (chance / 100.f));

	weapon->UpdateAccuracyPenalty( );
	float weap_spread = weapon->GetSpread( );
	float weap_inaccuracy = weapon->GetInaccuracy( );

	for ( int i = 0; i < 150; i++ )
	{
		float a = RandomFloat( 0.f, 1.f );
		float b = RandomFloat( 0.f, 2.f * PI_F );
		float c = RandomFloat( 0.f, 1.f );
		float d = RandomFloat( 0.f, 2.f * PI_F );

		float inaccuracy = a * weap_inaccuracy;
		float spread = c * weap_spread;

		if ( weapon->m_iItemDefinitionIndex( ) == 64 )
		{
			a = 1.f - a * a;
			a = 1.f - c * c;
		}

		Vector spreadView( (cos( b ) * inaccuracy) + (cos( d ) * spread), (sin( b ) * inaccuracy) + (sin( d ) * spread), 0 ), direction;

		direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
		direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
		direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
		direction.Normalized( );

		Vector viewAnglesSpread;
		VectorAngles( direction, up, viewAnglesSpread );
		NormalizeAngles( viewAnglesSpread );

		Vector viewForward;
		Math::AngleVectors( viewAnglesSpread, viewForward );
		viewForward.NormalizeInPlace( );

		viewForward = src + (viewForward * weapon->GetCSWeaponData( )->flRange);

		trace_t tr;
		Ray_t ray;

		ray.Init( src, viewForward );
		g_EngineTrace->ClipRayToEntity( ray, MASK_SHOT | CONTENTS_GRATE, ent, &tr );

		if ( tr.hit_entity == ent )
			++cHits;

		if ( static_cast< int >((static_cast< float >(cHits) / 150.f) * 100.f) >= chance )
			return true;

		if ( (150 - i + cHits) < cNeededHits )
			return false;
	}
	return false;
}

Vector angnormalization( Vector angle )
{
	while ( angle.x > 89.f )
	{
		angle.x -= 180.f;
	}
	while ( angle.x < -89.f )
	{
		angle.x += 180.f;
	}
	if ( angle.y > 180 )
	{
		angle.y -= (round( angle.y / 360 ) * 360.f);
	}
	else if ( angle.y < -180 )
	{
		angle.y += (round( angle.y / 360 ) * -360.f);
	}
	if ( (angle.z > 50) || (angle.z < 50) )
	{
		angle.z = 0;
	}
	return angle;
}
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
bool LagComp_Hitchanced = false;


namespace Aimbot_R
{
	matrix3x4_t Matrix [ 65 ][ 128 ];

	auto CanShift = [ ] ( ) -> bool
	{

		auto weapon = g_LocalPlayer->m_hActiveWeapon( );
		if ( !weapon )
			return false;
		float m_flPlayerTime = (g_LocalPlayer->m_nTickBase( ) - ((nTickBaseShift > 0) ? 1 + nTickBaseShift : 0)) * g_pGlobalVars->interval_per_tick;

		if ( m_flPlayerTime <= weapon->m_flNextPrimaryAttack( ) )
			return false; // no need to shift ticks

		return true;
	};
	int count = 20;
	bool start;
	auto should_restore = [ ] ( CUserCmd* cmd ) -> bool
	{
		if ( cmd->tick_count == globals::last_doubletap + TIME_TO_TICKS( g_LocalPlayer->FireRate( ) * 1) )
			start = true;

		if ( count == 0 )
		{
			start = false;
			count = 20;
		}
		while ( count != 0 && start )
		{
			count--;
			return true;
		}

		return false;
	};
	auto shoot_again = [ ] ( CUserCmd* cmd ) -> bool
	{
		if ( cmd->tick_count > globals::last_doubletap + TIME_TO_TICKS( g_LocalPlayer->FireRate( ) * 2 ) )
			return true;
		else
			return false;

		return false;
	};
	auto shoot_2 = [ ] ( ) -> bool
	{

		auto weapon = g_LocalPlayer->m_hActiveWeapon( );
		if ( !weapon )
			return false;
		float m_flPlayerTime = (g_LocalPlayer->m_nTickBase( ) - ((nTickBaseShift > 0) ? 1 + nTickBaseShift : 0)) * g_pGlobalVars->interval_per_tick;

		if ( m_flPlayerTime <= weapon->m_flNextPrimaryAttack( ) )
			return false; // no need to shift ticks

		return true;
	};
	bool charged_dt = false;
	void dt( CUserCmd* cmd )
	{
		if ( !g_LocalPlayer || !g_LocalPlayer->IsAlive( ) )
			return;
		auto weapon = g_LocalPlayer->m_hActiveWeapon( );
		if ( !weapon || weapon->m_iClip1( ) == 0 ) return;
		if ( weapon->m_iItemDefinitionIndex( ) == WEAPON_TASER ) return;
		if ( weapon->m_iItemDefinitionIndex( ) == WEAPON_HEGRENADE || weapon->m_iItemDefinitionIndex( ) == WEAPON_INCGRENADE || weapon->m_iItemDefinitionIndex( ) == WEAPON_FLASHBANG || weapon->m_iItemDefinitionIndex( ) == WEAPON_SMOKEGRENADE || weapon->m_iItemDefinitionIndex( ) == WEAPON_MOLOTOV || weapon->m_iItemDefinitionIndex( ) == WEAPON_DECOY || weapon->m_iItemDefinitionIndex( ) == WEAPON_KNIFE || weapon->IsKnife( ) ) return;

		float flServerTime = g_LocalPlayer->m_nTickBase( ) * g_pGlobalVars->interval_per_tick;
		bool canShoot = (weapon->m_flNextPrimaryAttack( ) <= flServerTime);

		static bool charge_dt = false;

		if ( GetKeyState( Variables.rageaimbot.doubletap_key) )
			charge_dt = true;

		if ( Variables.rageaimbot.doubletab == 1 )
		{
			if ( charge_dt && shoot_again( cmd ) )
				charged_dt = true;
			else
				charged_dt = false;
		}
		else
		{
			if ( shoot_again( cmd ) )
				charged_dt = true;
			else
				charged_dt = false;
		}

		if ( should_restore( cmd ) )
		{
			globals::doubletap_delta = 0;
			globals::last_doubletap = 0;
			cmd->tick_count = INT_MAX;
			cmd->buttons &= ~IN_ATTACK;
		}

		/*if (charge_dt && !CanShift())
		{
			if (cmd->buttons & IN_ATTACK)
				cmd->viewangles = angle - g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat();
		}*/


		if ( Variables.rageaimbot.doubletab == 1 )
		{
			if ( charge_dt && CanShift( ) && shoot_again( cmd ) )
			{
				globals::chockepack = 1;
				if ( cmd->buttons & IN_ATTACK )
				{
					globals::last_doubletap = cmd->tick_count;
					nTickBaseShift = TIME_TO_TICKS( g_LocalPlayer->FireRate( ) );
				}
				charge_dt = false;
			}


		}
		else if ( Variables.rageaimbot.doubletab == 2 )
		{
			if ( CanShift( ) && shoot_again( cmd ) )
			{
				globals::chockepack = 1;
				if ( cmd->buttons & IN_ATTACK )
				{
					globals::last_doubletap = cmd->tick_count;
					nTickBaseShift = TIME_TO_TICKS( g_LocalPlayer->FireRate( ) );
				}
			}

		}

	}


	void autor8( )
	{
		if ( globals::pCmd )
		{
			if ( Variables.rageaimbot.autorevolvo && g_LocalPlayer->m_hActiveWeapon( ).Get( )->m_iItemDefinitionIndex( ) == WEAPON_REVOLVER )
			{
				static int delay = 0;
				delay++;
				if ( delay <= 15 )globals::pCmd->buttons |= IN_ATTACK;
				else delay = 0;
			}
		}

	}
	std::vector<target_selection_info>	Entities;
	void create_move( CUserCmd* cmd ) {
		Entities.clear( );
		target_selection( );
		aim_func( cmd );

	}

	void target_selection( )
	{

		for ( int i = 1; i <= 65; i++ )
		{
			auto entity = ( C_BasePlayer* ) g_EntityList->GetClientEntity( i );
			if ( !entity ) continue;

			if ( !entity->IsValidTarget( ) )
				continue;


			target_selection_info data = target_selection_info( entity, i );


			Entities.push_back( data );
		}
	}
	void aim_func( CUserCmd* cmd ) {

		auto local = g_LocalPlayer;
		auto weapon = local->m_hActiveWeapon( ).Get( );
		if ( !local ) return;
		if ( !weapon ) return;
		if ( weapon->m_iClip1( ) == 0 ) return;
		if ( weapon->m_iItemDefinitionIndex( ) == ItemDefinitionIndex::WEAPON_C4 || weapon->IsGrenade( ) || weapon->IsKnife( ) ) return;
		if ( !can_shoot( cmd ) ) { cmd->buttons &= ~IN_ATTACK;	return; }
		if ( GetAsyncKeyState( VK_LBUTTON ) ) return;
		for ( auto players : Entities )
		{
			auto entity = players.entity;

			if ( !entity )
				continue;

			if ( !entity->IsValidTarget( ) )
				continue;

			/*player_info_t pInfo;
			g_EngineClient->GetPlayerInfo(entity->EntIndex(), &pInfo);

			if (strcmp(pInfo.szSteamID, ("STEAM_0:1:440467695")) == 0)
				continue;*/

			Vector scan = hitscan( entity );


			if ( scan == Vector( 0, 0, 0 ) ) continue;
			if ( !local->m_bIsScoped( ) && local->m_hActiveWeapon( ).Get( )->IsScopeable( ) && Variables.rageaimbot.rageautoscope )
			{
				globals::pCmd->buttons |= IN_ATTACK2;
			}
			if ( local && Variables.rageaimbot.rageautostop )
			{
				if ( local->m_fFlags( ) & FL_ONGROUND )
				{
					if ( weapon->IsGrenade( ) || weapon->IsKnife( ) || weapon->m_iItemDefinitionIndex( ) == WEAPON_TASER ) //we dont wanna stop if we holdin a knife, grenade or zeus
						return;
					ClampMov2363526263ement( globals::pCmd, 2 );
				}

			}
			globals::aimangle = angnormalization( Math::CalcAngle( g_LocalPlayer->GetEyePos( ), scan ) );

			if ( globals::aimangle == Vector( 0, 0, 0 ) ) continue;

			if ( !HitChance( globals::aimangle, entity, SynchronizeHitchance( ) ) ) //done as late as possible to reduce stress on aimbot having to re-lock once this returns true
				continue;

			if ( Variables.rageaimbot.autofire )
			{
				globals::pCmd->buttons |= IN_ATTACK;
				shots_fire [ entity->EntIndex( ) ]++;
			}


			cmd->tick_count = TIME_TO_TICKS( entity->m_flSimulationTime( ) + Utils::GetLerpTime( ) );

		}
		if ( cmd->buttons & IN_ATTACK )
		{

			float recoil_scale = g_CVar->FindVar( "weapon_recoil_scale" )->GetFloat( );

				if ( GetAsyncKeyState( Variables.rageaimbot.fakeduckkey ) )
					globals::bSendPacket = false;
				else
					globals::bSendPacket = true;
			

			globals::aimangle -= local->m_aimPunchAngle( ) * recoil_scale;

				if ( GetAsyncKeyState( Variables.rageaimbot.fakeduckkey ) )
				{
					cmd->viewangles = globals::aimangle;
				}
				else
				{
					if ( globals::bSendPacket )
						cmd->viewangles = globals::aimangle;
					else
						cmd->viewangles.y = 90;
				}
			



		}

	}
}





















