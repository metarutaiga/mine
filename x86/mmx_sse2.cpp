#include <algorithm>
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "mmx_register.h"
#include "mmx_register.inl"
#include "mmx_instruction.h"
#include "mmx_instruction.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void mmx_instruction::PADDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PADDQ", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64 = DEST.i64 + SRC.i64;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PMULUDQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PMULUDQ", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.u64[0] = DEST.u32[0] * SRC.u32[0];
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
void mmx_instruction::PSUBQ(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "PSUBQ", 2, MMX_REGISTER | OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST.i64 = DEST.i64 - SRC.i64;
    } END_OPERATION_MMX;
}
//------------------------------------------------------------------------------
