#include "animfix.h"
#include "lagcomp.h"
#include "../globals/globals.hpp"
#include "../helpers/math.hpp"
#include "resolver.hpp"
#include "../memalloc.h"

namespace AnimFix
{


	bool fresh_tick()
	{
		static int old_tick_count;


		if (old_tick_count != g_pGlobalVars->tickcount)
		{
			old_tick_count = g_pGlobalVars->tickcount;
			return true;
		}
	}
	enum bone
	{
		bone_used_mask = 0x0007FF00,
		bone_used_by_anything = 0x0007FF00,
		bone_used_by_hitbox = 0x00000100,
		bone_used_by_attachment = 0x00000200,
		bone_used_by_vertex_mask = 0x0003FC00,
		bone_used_by_vertex_lod0 = 0x00000400,
		bone_used_by_vertex_lod1 = 0x00000800,
		bone_used_by_vertex_lod2 = 0x00001000,
		bone_used_by_vertex_lod3 = 0x00002000,
		bone_used_by_vertex_lod4 = 0x00004000,
		bone_used_by_vertex_lod5 = 0x00008000,
		bone_used_by_vertex_lod6 = 0x00010000,
		bone_used_by_vertex_lod7 = 0x00020000,
		bone_used_by_bone_merge = 0x00040000
	};

	static constexpr auto bone_used_by_server = bone_used_by_hitbox | bone_used_by_vertex_lod0 | bone_used_by_vertex_lod1 | bone_used_by_vertex_lod2
		| bone_used_by_vertex_lod3 | bone_used_by_vertex_lod4 | bone_used_by_vertex_lod5 | bone_used_by_vertex_lod6 | bone_used_by_vertex_lod7;

	bool HandleBoneSetup(C_BasePlayer* target, matrix3x4_t* pBoneToWorldOut, int boneMask, float currentTime)
	{



		auto hdr = target->GetModelPtr();
		if (!hdr)
			return false;

		auto oldBones = target->GetBoneAccessor()->GetBoneArrayForWrite();

		matrix3x4_t baseMatrix;
	Math::angle_matrix(target->GetAbsAngles(), target->GetAbsOrigin(), baseMatrix);

		target->get_effects() |= 0x008;

		IKContext* m_pIk = target->get_ik_context();
		if (m_pIk)
		{
			m_pIk->clear_targets();
			m_pIk->init(hdr, target->GetAbsAngles(), target->GetAbsOrigin(), currentTime, g_pGlobalVars->framecount, boneMask);
		}
		Vector* pos = (Vector*)(g_pMemAlloc->Alloc(sizeof(Vector[256])));
		quaternion* q = (quaternion*)(g_pMemAlloc->Alloc(sizeof(quaternion[256])));
		std::memset(pos, 0xFF, sizeof(pos));
		std::memset(q, 0xFF, sizeof(q));

		target->GetBoneAccessor()->SetBoneArrayForWrite(pBoneToWorldOut);

		target->StandardBlendingRules(hdr, pos, q, currentTime, boneMask);

		byte* boneComputed = (byte*)(g_pMemAlloc->Alloc(sizeof(byte[256])));
		std::memset(boneComputed, 0, sizeof(byte[256]));

		if (m_pIk)
		{
			target->update_ik_locks(currentTime);
			m_pIk->update_targets(pos, q, target->GetBoneAccessor()->GetBoneArrayForWrite(), &boneComputed[0]);
			target->calculate_ik_locks(currentTime);
			m_pIk->solve_dependencies(pos, q, target->GetBoneAccessor()->GetBoneArrayForWrite(), &boneComputed[0]);
		}

		target->BuildTransformations(hdr, pos, q, baseMatrix, boneMask, &boneComputed[0]);

		target->get_effects() &= ~0x008;

		target->GetBoneAccessor()->SetBoneArrayForWrite(oldBones);

		return true;
	}

	void build_server_bones(C_BasePlayer* player, matrix3x4_t* pBoneToWorldOut)
	{
		if (!player)
			return;

		// get studio hdr.
		const auto hdr = player->GetModelPtr();
		if (!hdr)
			return;

		// get renderable entity.
		const auto renderable = (C_BasePlayer*)((DWORD)player + 0x4);
		if (!renderable)
			return;

		// initialize variables.
		Vector pos[128]{};
		quaternion q[128]{};
		uint8_t computed[256]{};

		// aligned transform matrix.
		_declspec(align(16)) matrix3x4_t transform = Math::angle_matrix(
			player->GetAbsAngles(), player->m_vecOrigin());

		// stop interpolation
		player->get_effects() |= 8;

		// notify engine we're setting up bones.
		*(int*)((DWORD)renderable + 224) |= 8;

		// get ik_context.
		IKContext* IK_context = player->get_ik_context();
		if (IK_context)
		{
			IK_context->clear_targets();
			IK_context->init(hdr, player->GetAbsAngles(), player->m_vecOrigin(),
				g_pGlobalVars->curtime, g_pGlobalVars->framecount, 256);
		}

		// run blending.
		player->StandardBlendingRules(hdr, pos, q, g_pGlobalVars->curtime, 256);

		// target matrix.
		const auto bone_matrix = player->get_bone_array_for_write();

		// fix arm jitter.
		if (IK_context)
		{
			player->update_ik_locks(g_pGlobalVars->curtime);
			IK_context->update_targets(pos, q, bone_matrix, computed);
			player->calculate_ik_locks(g_pGlobalVars->curtime);
			IK_context->solve_dependencies(pos, q, bone_matrix, computed);
		}

		// build bones.
		player->BuildTransformations(hdr, pos, q, transform, 256, computed);

		// save bones.
		memcpy(pBoneToWorldOut, bone_matrix, player->get_bone_cache_count() * sizeof(float) * 12);

		// start interpolation again
		player->get_effects() &= ~8;

		// cleanup.
		*(int*)((DWORD)renderable + 224) &= ~(8);
	}
	/*
	bool HandleBoneSetup(C_BasePlayer* player, matrix3x4_t* pBoneToWorldOut, int boneMask, float currentTime)
	{





		const auto hdr = player->GetModelPtr();
		if (!hdr)
			return false;

		auto bone_accessor = player->GetBoneAccessor();
		if (!bone_accessor)
			return false;

		const auto backup_matrix = bone_accessor->GetBoneArrayForWrite();
		if (!backup_matrix)
			return false;



		ALIGN16 matrix3x4_t parent_transform;
		Math::angle_matrix(player->GetAbsAngles(), player->m_vecOrigin(), parent_transform);

		Vector pos[128];
		quaternion q[128];
		ZeroMemory(pos, sizeof(Vector[128]));
		ZeroMemory(q, sizeof(quaternion[128]));

		player->get_effects() |= 0x008;

		IKContext* m_pIk = player->get_ik_context();
		if (m_pIk)
		{
			m_pIk->clear_targets();
			m_pIk->init(hdr, player->GetAbsAngles(), player->GetAbsOrigin(), currentTime, g_pGlobalVars->framecount, boneMask);
		}



		// set bone array for write.
		bone_accessor->SetBoneArrayForWrite(pBoneToWorldOut);

		// compute and build bones.
		player->StandardBlendingRules(hdr, pos, q, g_pGlobalVars->curtime, 256);

		byte computed[32];
		ZeroMemory(computed, sizeof(byte[0x20]));


		if (m_pIk)
		{
			player->update_ik_locks(currentTime);
			m_pIk->update_targets(pos, q, player->GetBoneAccessor()->GetBoneArrayForWrite(), &computed[0]);
			player->calculate_ik_locks(currentTime);
			m_pIk->solve_dependencies(pos, q, player->GetBoneAccessor()->GetBoneArrayForWrite(), &computed[0]);
		}

		player->BuildTransformations(hdr, pos, q, parent_transform, 256, &computed[0]);

		// restore old matrix.
		bone_accessor->SetBoneArrayForWrite(backup_matrix);

		// restore original interpolated entity data.
		player->get_effects() &= ~0x008;


		return true;
	}
	*/
	bool setup_bones(C_BasePlayer* target, int boneMask, float currentTime, matrix3x4_t* pBoneToWorldOut)
	{
		alignas(16) matrix3x4_t bone_out[128];
		const auto ret = HandleBoneSetup(target, bone_out, boneMask, g_pGlobalVars->curtime);
		memcpy(pBoneToWorldOut, bone_out, sizeof(matrix3x4_t[128]));
		return ret;
	}
	void build_matrix(C_BasePlayer* pl, matrix3x4_t mat[128]) {
		/* backup necessary information */
		const auto backup_frametime = g_pGlobalVars->frametime;
		const auto backup_index = *(int*)((uintptr_t)pl + 0x64); // const auto backup_index = pl->EntIndex();

		/* most of the stuff is located in animating class */
		const auto animating = reinterpret_cast<void*>(uintptr_t(pl) + 0x4);

		/* skip call to AccumulateLayers */
		*reinterpret_cast<uint8_t*>(uintptr_t(animating) + 0xa24) = 0xa;
		/* previous bone mask && current bone mask (make sure attatchment helper gets called) */
		*reinterpret_cast<int*>(uintptr_t(animating) + 0x26a8) = 0;
		*reinterpret_cast<int*>(uintptr_t(animating) + 0x26ac) = 0;
		/* disable matrix interpolation */
		*reinterpret_cast<int*>(uintptr_t(animating) + 0xe8) |= 8;
		/* don't use prediction seed as seed for SetupBones */
		*reinterpret_cast<bool*>(uintptr_t(animating) + 0x2ea) = false;

		*reinterpret_cast<int*>(uintptr_t(animating) + 0xa64) = g_pGlobalVars->framecount;

		if (g_pGlobalVars->frametime >= 0.0033333334f)
			g_pGlobalVars->frametime = 0.0033333f;

		* (int*)((uintptr_t)pl + 0x64) = 2 - (g_pGlobalVars->framecount % 3); // pl->EntIndex

		/* build new bone matrix and overwrite old */
		pl->InvalidateBoneCache();

		/* build bones */
		pl->SetupBones(mat, -1, 0x7ff00, g_pGlobalVars->curtime);

		g_pGlobalVars->frametime = backup_frametime;
		*(int*)((uintptr_t)pl + 0x64) = backup_index; // pl->EntIndex
	}

	typedef std::array<float, 24> pose_paramater;


	void LocalAnimFix(C_BasePlayer* entity)
	{

		if (!entity || !entity->IsAlive() || !globals::pCmd)
			return;

		static float proper_abs = entity->GetPlayerAnimState()->m_flGoalFeetYaw;
		static std::array<float, 24> sent_pose_params = entity->m_flPoseParameter();
		static AnimationLayer backup_layers[15];

		if (fresh_tick())
		{
			std::memcpy(backup_layers, entity->GetAnimOverlays(), (sizeof(AnimationLayer) * entity->NumOverlays()));
			entity->ClientAnimations() = true;
			entity->UpdateAnimationState(entity->GetPlayerAnimState(), globals::pCmd->viewangles);

			if (entity->GetPlayerAnimState())
				entity->GetPlayerAnimState()->m_iLastClientSideAnimationUpdateFramecount = g_pGlobalVars->framecount - 1;

			entity->UpdateClientSideAnimation();
			if (globals::bSendPacket)
			{
				proper_abs = entity->GetPlayerAnimState()->m_flGoalFeetYaw;
				sent_pose_params = entity->m_flPoseParameter();
			}
		}
		entity->ClientAnimations() = false;
		entity->SetAbsAngles(Vector(0, proper_abs, 0));
		entity->GetPlayerAnimState()->m_flUnknownFraction = 0.f; // Lol.
		std::memcpy(entity->GetAnimOverlays(), backup_layers, (sizeof(AnimationLayer) * entity->NumOverlays()));
		entity->m_flPoseParameter() = sent_pose_params;
	}
}























