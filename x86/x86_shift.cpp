#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
x86_format::Format x86_instruction::Sxx()
{
    Format format;
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:
        switch (opcode[1] >> 3 & 7) {
        case 4: Decode(format, 1, "SHL", 0, opcode[0] & 0b01, 8);  break;
        case 5: Decode(format, 1, "SHR", 0, opcode[0] & 0b01, 8);  break;
        case 6: Decode(format, 1, "SAL", 0, opcode[0] & 0b01, 8);  break;
        case 7: Decode(format, 1, "SAR", 0, opcode[0] & 0b01, 8);  break;
        }
        break;
    case 0xD0:
    case 0xD1:
        switch (opcode[1] >> 3 & 7) {
        case 4: Decode(format, 1, "SHL", 0, opcode[0] & 0b01, 0);  break;
        case 5: Decode(format, 1, "SHR", 0, opcode[0] & 0b01, 0);  break;
        case 6: Decode(format, 1, "SAL", 0, opcode[0] & 0b01, 0);  break;
        case 7: Decode(format, 1, "SAR", 0, opcode[0] & 0b01, 0);  break;
        }
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        switch (opcode[1] >> 3 & 7) {
        case 4: Decode(format, 1, "SHL", 0, opcode[0] & 0b01, 0);  break;
        case 5: Decode(format, 1, "SHR", 0, opcode[0] & 0b01, 0);  break;
        case 6: Decode(format, 1, "SAL", 0, opcode[0] & 0b01, 0);  break;
        case 7: Decode(format, 1, "SAR", 0, opcode[0] & 0b01, 0);  break;
        }
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = REG(ECX);
        break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
        switch (x86.opcode[1] >> 3 & 7) {
        case 4: DEST = DEST << SRC; break;
        case 5: DEST = DEST >> SRC; break;
        case 6: DEST = dest << SRC; break;
        case 7: DEST = dest >> SRC; break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SHxD()
{
    Format format;
    switch (opcode[1]) {
    case 0xA4:
        Decode(format, 2, "SHLD", 0, 1, 0);
        format.length += 1;
        format.operand[2].type = Format::Operand::IMM;
        format.operand[2].displacement = IMM8(opcode, 3);
        break;
    case 0xA5:
        Decode(format, 2, "SHLD", 0, 1, 0);
        format.operand[2].type = Format::Operand::REG;
        format.operand[2].base = REG(ECX);
        break;
    case 0xAC:
        Decode(format, 2, "SHRD", 0, 1, 0);
        format.length += 1;
        format.operand[2].type = Format::Operand::IMM;
        format.operand[2].displacement = IMM8(opcode, 3);
        break;
    case 0xAD:
        Decode(format, 2, "SHRD", 0, 1, 0);
        format.operand[2].type = Format::Operand::REG;
        format.operand[2].base = REG(ECX);
        break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        // TODO
    } END_OPERATION;
}
//------------------------------------------------------------------------------
