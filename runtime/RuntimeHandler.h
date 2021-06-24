#pragma once

#include "../../framework.h"

class RuntimeHandler {

public:

    RuntimeHandler(Segment& segment) : m_Segment (segment) {}

    //Extract segment to memory.
    void ExtractSegment ();

    //Patch relocations and imports.
    void ReconstructHotPoints ();

    //Just invoke segment.
    void InvokeOEP ();

private:

    Segment& m_Segment;

};