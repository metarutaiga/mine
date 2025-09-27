#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::CMPXCHG(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPXCHG", 2, 0, opcode[1] & 0b01);
    format.operand_count = 3;
    format.operand[2].type = Format::Operand::REG;
    format.operand[2].flags = Format::Operand::HIDE;
    format.operand[2].base = IndexREG(EAX);

    BEGIN_OPERATION() {
        auto& accumulator = SRC2;
        if (accumulator == DEST) {
            ZF = 1;
            DEST = SRC;
        }
        else {
            ZF = 0;
            accumulator = DEST;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CMPXCHG8B(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMPXCHG8B", 2);
    format.operand_count = 1;

    OPERATION() {
        auto& DEST = *(uint64_t*)format.operand[0].memory;
        if ((uint64_t(EDX) << 32 | EAX) == DEST) {
            ZF = 1;
            DEST = (uint64_t(ECX) << 32 | EBX);
        }
        else {
            ZF = 0;
            EDX = uint32_t(DEST >> 32);
            EAX = uint32_t(DEST);
        }
    };
}
//------------------------------------------------------------------------------
void x86_instruction::XADD(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "XADD", 2, 0, opcode[1] & 0b01);

    BEGIN_OPERATION() {
        auto TEMP = SRC + DEST;
        SRC = DEST;
        DEST = TEMP;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
