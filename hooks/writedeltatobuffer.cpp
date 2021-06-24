
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

	void WriteUsercmd(bf_write* buf, CUserCmd* pin, CUserCmd* pout)
	{
		//using WriteUsercmd_t = void(__fastcall*)(bf_write*, CUserCmd*, CUserCmd*);
		static DWORD WriteUsercmdF = (DWORD)Utils::PatternScan(GetModuleHandle("client.dll"), ("55 8B EC 83 E4 F8 51 53 56 8B D9"));

		__asm
		{
			mov     ecx, buf
			mov     edx, pin
			push	pout
			call    WriteUsercmdF
			add     esp, 4
		}
	};
	//--------------------------------------------------------------------------------


	bool __fastcall hkWriteUsercmdDeltaToBuffer(IBaseClientDLL* ECX, void* EDX, /*void* unk,*/ int nSlot, bf_write* buf, int from, int to, bool isNewCmd)
	{
		static DWORD WriteUsercmdDeltaToBufferReturn = (DWORD)Utils::PatternScan(GetModuleHandleA("engine.dll"), ("84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84"));
		static auto sendmovecall = Utils::PatternScan(GetModuleHandleA("engine.dll"), ("84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84"));

		if (nTickBaseShift <= 0 /*|| (DWORD)_ReturnAddress() != ((DWORD)GetModuleHandleA("engine.dll") + 0xCCCA6)*/)
			return hlclient_hook.get_original<WriteUsercmdDeltaToBufferFn>(24)(ECX, /*unk,*/ nSlot, buf, from, to, isNewCmd);

		if (!g_LocalPlayer->IsAlive() || !g_EngineClient->IsConnected() || !g_EngineClient->IsInGame())
		{
			nTickBaseShift = 0;
			return hlclient_hook.get_original<WriteUsercmdDeltaToBufferFn>(24)(ECX, /*unk,*/ nSlot, buf, from, to, isNewCmd);
		}


		if (from != -1)
			return true;

		// CL_SendMove function
		auto CL_SendMove = []()
		{
			using CL_SendMove_t = void(__fastcall*)(void);
			static CL_SendMove_t CL_SendMoveF = (CL_SendMove_t)Utils::PatternScan(GetModuleHandleA("engine.dll"), ("55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98"));

			CL_SendMoveF();
		};

		int* pNumBackupCommands = (int*)((uintptr_t)buf - 0x30);
		int* pNumNewCommands = (int*)((uintptr_t)buf - 0x2C);
		auto net_channel = *reinterpret_cast<NetChannel**>(reinterpret_cast<uintptr_t>(g_ClientState) + 0x9C);
		int32_t new_commands = *pNumNewCommands;


		// Manipulate CLC_Move
		auto nextcmdnumber = g_ClientState->nLastOutgoingCommand + g_ClientState->iChokedCommands + 1;
		auto totalnewcommands = std::min(nTickBaseShift, 60);
		nTickBaseShift -= totalnewcommands;

		from = -1;
		*pNumNewCommands = totalnewcommands;
		*pNumBackupCommands = 0;

		for (to = nextcmdnumber - new_commands + 1; to <= nextcmdnumber; to++)
		{
			if (!hlclient_hook.get_original<WriteUsercmdDeltaToBufferFn>(24)(ECX, /*unk,*/ nSlot, buf, from, to, true))
				return false;

			from = to;
		}

		auto lastrealcmd = g_Input->GetUserCmd(nSlot, from);
		CUserCmd fromcmd;

		if (lastrealcmd)
			fromcmd = *lastrealcmd;

		CUserCmd tocmd = fromcmd;
		tocmd.command_number = nextcmdnumber++;
		tocmd.tick_count += 100;

		for (int i = new_commands; i <= totalnewcommands; i++)
		{
			WriteUsercmd(buf, &tocmd, &fromcmd);
			fromcmd = tocmd;
			tocmd.command_number++;
			tocmd.tick_count++;
		}

		return true;
	}

}



















