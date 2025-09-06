#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x87_register.h"
#include "x87_register.inl"
#include "x87_instruction.h"
#include "x87_instruction.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x87_instruction::FCMOVcc(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xDA:
    switch (opcode[1] & 0b11111000) {
        case 0xC0:  format.instruction = "FCMOVB";      break;
        case 0xC8:  format.instruction = "FCMOVE";      break;
        case 0xD0:  format.instruction = "FCMOVBE";     break;
        case 0xD8:  format.instruction = "FCMOVU";      break;
        }
        break;
    case 0xDB:
    switch (opcode[1] & 0b11111000) {
        case 0xC0:  format.instruction = "FCMOVNB";     break;
        case 0xC8:  format.instruction = "FCMOVNE";     break;
        case 0xD0:  format.instruction = "FCMOVNBE";    break;
        case 0xD8:  format.instruction = "FCMOVNU";     break;
        }
        break;
    }
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    switch (opcode[0]) {
    case 0xDA:
    switch (opcode[1] & 0b11111000) {
        case 0xC0:  OPERATION() { if ((CF)      == 1) ST(0) = ST(format.operand[1].base); };    break;
        case 0xC8:  OPERATION() { if ((ZF)      == 1) ST(0) = ST(format.operand[1].base); };    break;
        case 0xD0:  OPERATION() { if ((CF | ZF) == 1) ST(0) = ST(format.operand[1].base); };    break;
        case 0xD8:  OPERATION() { if ((PF)      == 1) ST(0) = ST(format.operand[1].base); };    break;
        }
        break;
    case 0xDB:
    switch (opcode[1] & 0b11111000) {
        case 0xC0:  OPERATION() { if ((CF)      == 0) ST(0) = ST(format.operand[1].base); };    break;
        case 0xC8:  OPERATION() { if ((ZF)      == 0) ST(0) = ST(format.operand[1].base); };    break;
        case 0xD0:  OPERATION() { if ((CF & ZF) == 0) ST(0) = ST(format.operand[1].base); };    break;
        case 0xD8:  OPERATION() { if ((PF)      == 0) ST(0) = ST(format.operand[1].base); };    break;
        }
        break;
    }
}
//------------------------------------------------------------------------------
void x87_instruction::FCOM(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FCOM", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    switch (opcode[1] & 0xF8) {
    case 0xD0:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        C0 = ST(0) < SRC;
        C2 = 0;
        C3 = ST(0) == SRC;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FCOMP(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "FCOMP", 1, 0, DIRECTION | X87_REGISTER);
    switch (opcode[0]) {
    case 0xD8:  format.width = 32;  break;
    case 0xDC:  format.width = 64;  break;
    }
    switch (opcode[1] & 0xF8) {
    case 0xD8:  format.width = 64;  break;
    }
    format.operand[0].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        C0 = ST(0) < SRC;
        C2 = 0;
        C3 = ST(0) == SRC;
        TOP = TOP + 1;
    } END_OPERATION_RANGE_FLOAT(32, 64);
}
//------------------------------------------------------------------------------
void x87_instruction::FCOMPP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCOMPP";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        C0 = ST(0) < SRC;
        C2 = 0;
        C3 = ST(0) == SRC;
        TOP = TOP + 2;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOMI(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCOMI";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        CF = ST(0) < SRC;
        PF = 0;
        ZF = ST(0) == SRC;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FCOMIP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCOMIP";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        CF = ST(0) < SRC;
        PF = 0;
        ZF = ST(0) == SRC;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOM(Format& format, const uint8_t* opcode)
{
    format.instruction = "FUCOM";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        C0 = isunordered(ST(0), SRC) || ST(0) < SRC;
        C2 = isunordered(ST(0), SRC);
        C3 = isunordered(ST(0), SRC) || ST(0) == SRC;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FUCOMP";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        C0 = isunordered(ST(0), SRC) || ST(0) < SRC;
        C2 = isunordered(ST(0), SRC);
        C3 = isunordered(ST(0), SRC) || ST(0) == SRC;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMPP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FUCOMPP";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        C0 = isunordered(ST(0), SRC) || ST(0) < SRC;
        C2 = isunordered(ST(0), SRC);
        C3 = isunordered(ST(0), SRC) || ST(0) == SRC;
        TOP = TOP + 2;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMI(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCOMI";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        CF = isunordered(ST(0), SRC) || ST(0) < SRC;
        PF = isunordered(ST(0), SRC);
        ZF = isunordered(ST(0), SRC) || ST(0) == SRC;
    };
}
//------------------------------------------------------------------------------
void x87_instruction::FUCOMIP(Format& format, const uint8_t* opcode)
{
    format.instruction = "FCOMIP";
    format.operand[1].type = Format::Operand::X87;
    format.operand[1].base = opcode[1] & 0b111;

    OPERATION() {
        auto SRC = ST(format.operand[1].base);
        CF = isunordered(ST(0), SRC) || ST(0) < SRC;
        PF = isunordered(ST(0), SRC);
        ZF = isunordered(ST(0), SRC) || ST(0) == SRC;
        TOP = TOP + 1;
    };
}
//------------------------------------------------------------------------------
