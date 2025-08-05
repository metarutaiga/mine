#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::Sxx(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:
        switch (opcode[1] >> 3 & 7) {
        case 4: Decode(format, opcode, "SHL", 1, 8, opcode[0] & 0b01);  break;
        case 5: Decode(format, opcode, "SHR", 1, 8, opcode[0] & 0b01);  break;
        case 6: Decode(format, opcode, "SAL", 1, 8, opcode[0] & 0b01);  break;
        case 7: Decode(format, opcode, "SAR", 1, 8, opcode[0] & 0b01);  break;
        }
        break;
    case 0xD0:
    case 0xD1:
        switch (opcode[1] >> 3 & 7) {
        case 4: Decode(format, opcode, "SHL", 1, 0, opcode[0] & 0b01);  break;
        case 5: Decode(format, opcode, "SHR", 1, 0, opcode[0] & 0b01);  break;
        case 6: Decode(format, opcode, "SAL", 1, 0, opcode[0] & 0b01);  break;
        case 7: Decode(format, opcode, "SAR", 1, 0, opcode[0] & 0b01);  break;
        }
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        switch (opcode[1] >> 3 & 7) {
        case 4: Decode(format, opcode, "SHL", 1, 0, opcode[0] & 0b01);  break;
        case 5: Decode(format, opcode, "SHR", 1, 0, opcode[0] & 0b01);  break;
        case 6: Decode(format, opcode, "SAL", 1, 0, opcode[0] & 0b01);  break;
        case 7: Decode(format, opcode, "SAR", 1, 0, opcode[0] & 0b01);  break;
        }
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = REG(ECX);
        break;
    }

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
void x86_instruction::SHxD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xA4:
        Decode(format, opcode, "SHLD", 2, 8, OPERAND_SIZE | THREE_OPERAND);
        break;
    case 0xA5:
        Decode(format, opcode, "SHLD", 2, 0, OPERAND_SIZE);
        format.operand[2].type = Format::Operand::REG;
        format.operand[2].base = REG(ECX);
        break;
    case 0xAC:
        Decode(format, opcode, "SHRD", 2, 8, OPERAND_SIZE | THREE_OPERAND);
        break;
    case 0xAD:
        Decode(format, opcode, "SHRD", 2, 0, OPERAND_SIZE);
        format.operand[2].type = Format::Operand::REG;
        format.operand[2].base = REG(ECX);
        break;
    }

    BEGIN_OPERATION() {
        // TODO
    } END_OPERATION;
}
//------------------------------------------------------------------------------
