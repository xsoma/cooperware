#pragma once

#include "../../framework.h"

class PanicUtils {

public:

    //Used to separate runtime from internal processes.
    enum Layers {
        INIT = 0x0,
        ROUTINE = 0x1
    };

    /**
     * Set important value to variable. (Throws "exception" if value or variable is null)
     *
     * @param variable - pointer to variable.
     * @param value - value.
     * @param reason - cause reason.
     **/

    static void SetImportant (DWORD *variable, DWORD value, const char* reason = "Variable or value is null.");

    /**
     * Require value to non-null. (Throws "exception" if value or variable is null)
     *
     * @param layer - call level.
     * @param value - required value.
     * @param reason - cause reason.
     **/

    static void RequireNonNull (Layers layer, DWORD value, const char* reason = "Require value is null.");

    /**
     * Call panic.
     *
     * @param layer - call level.
     * @param reason - call reason.
     **/

    static void Release (Layers layer, const char* reason);

};