//==============================================================================
// MIPS(R) Architecture For Programmers Volume II-A: The MIPS64(R) Instruction Set Reference Manual
//
// Document Number: MD00087
// Revision 6.06
// December 15, 2016
//
// Copyright (C) Wave Computing, Inc. All rights reserved.
//==============================================================================

#include "platform.h"
#include "cpu.h"

CPU::SYSTEMCALLFUNCTION CPU::SystemCallFunction;
CPU::NATIVEFUNCTION CPU::NativeFunction;
//------------------------------------------------------------------------------
CPU::CPU()
    :cop0(*this)
    ,cop1(*this)
    ,cop2(*this)
{
    // Instruction
    Encode = 0;

    // CPU
    PC = 0;
    for (unsigned int i = 0; i < 32; ++i)
    {
        GPR[i] = 0;
    }

    // Stack
    Stack = new intptr_t[65536];
    GPR[29] = (intptr_t)&Stack[65504];
}
//------------------------------------------------------------------------------
CPU::~CPU()
{
    delete[] Stack;
}
//------------------------------------------------------------------------------
void CPU::Execute(const void* code)
{
    PC = (intptr_t)code;
    GPR[25] = PC;
    for (;;)
    {
        intptr_t pc = PC;

        // Instruction
        Encode = *(int*)pc;
        FINSTRUCTION OPCODE = tableOPCODE[opcode];
        (this->*OPCODE)();

#if defined(_DEBUG)
        // Debug
        if (GPR[0] != 0)
            __builtin_trap();
#endif

        // Not Branch and Jump
        if (PC == pc)
        {
            PC += 4;
        }
        // Native Function
        else if (NativeFunction)
        {
            NativeFunction(*this);
        }

        // Exit
        if (PC == 0)
            break;
    }
}
//------------------------------------------------------------------------------
void CPU::BranchDelaySlot()
{
    unsigned int encode = Encode;

    // Instruction
    Encode = *(int*)(PC + 4);
    FINSTRUCTION OPCODE = tableOPCODE[opcode];
    (this->*OPCODE)();

    Encode = encode;
}
//------------------------------------------------------------------------------
void CPU::SetSystemCall(SYSTEMCALLFUNCTION systemCall)
{
    SystemCallFunction = systemCall;
}
//------------------------------------------------------------------------------
void CPU::SetNativeFunction(NATIVEFUNCTION nativeFunction)
{
    NativeFunction = nativeFunction;
}
//------------------------------------------------------------------------------
void CPU::SignalException(int exception, int argument)
{
    switch (exception)
    {
    case SystemCall:
        if (SystemCallFunction)
        {
            SystemCallFunction(*this, argument);
        }
        break;
    default:
        __builtin_trap();
        break;
    }
}
//------------------------------------------------------------------------------
