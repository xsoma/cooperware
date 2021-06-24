#pragma once

#include "../../framework.h"

class GameFramework {

protected:

    //Legacy. More about this stuff: https://www.unknowncheats.me/forum/counterstrike-global-offensive/190552-internal-printing-console-explanation.html

    #define GAME_VIRTUAL_CONSOLE_INDEX 108 * 4

    typedef void* (*CreateInterfaceEngine) (const char* interfaceName, int* code);
    typedef bool (__thiscall *ConsoleInterface)  (const void* caller, const char* command);
    typedef void (*PrintConsoleInterface) (const char* message, ...);

    PrintConsoleInterface m_PrintInterface = (PrintConsoleInterface) Utils::GetFunction ("tier0.dll", "Msg");
    CreateInterfaceEngine m_CreateInterface = (CreateInterfaceEngine) Utils::GetFunction ("engine.dll", "CreateInterface");
    PVOID m_VClientPointer = m_CreateInterface ("VEngineClient014", nullptr);
    ConsoleInterface m_ConsoleInterface = (ConsoleInterface) * reinterpret_cast<int*> (*reinterpret_cast<int*> (m_VClientPointer) + GAME_VIRTUAL_CONSOLE_INDEX);

public:

    //Check non-null interfaces.
    bool m_IsInterfacesAvailable = m_PrintInterface && m_CreateInterface && m_VClientPointer && m_ConsoleInterface;

    /**
     * Simple send new line to console.
     *
     * @param newLineCount - Lines count.
     **/

    void PrintNewLineToConsole (int newLineCount = 1);

    /**
     * Print message to game console.
     **/

    GameFramework::PrintConsoleInterface AtPrintInterface () {
        //New line for formating.
        PrintNewLineToConsole ();
        //"Timer" stonks.
        Sleep (0x40);
        return m_PrintInterface;
    }

    /**
     * Execute command at console.
     **/

    GameFramework::ConsoleInterface AtConsoleInterface () {
        return m_ConsoleInterface;
    }

    /**
     * Legacy method for create interfaces.
     **/

    GameFramework::CreateInterfaceEngine AtCreateInterfaceEngine () {
        return m_CreateInterface;
    }

};