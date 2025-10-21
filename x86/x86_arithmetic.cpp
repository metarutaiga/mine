#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::ADC(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:  Decode(format, opcode, "ADC", 1, opcode[0] & 0b11);                 break;
    case 0x14:
    case 0x15:  Decode(format, opcode, "ADC", 0, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "ADC", 1, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x83:  Decode(format, opcode, "ADC", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<OSZAPC, CARRY>(x86, DEST, DEST + (SRC + CF), DEST, SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::ADD(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:  Decode(format, opcode, "ADD", 1, opcode[0] & 0b11);                 break;
    case 0x04:
    case 0x05:  Decode(format, opcode, "ADD", 0, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "ADD", 1, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x83:  Decode(format, opcode, "ADD", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<OSZAPC, CARRY>(x86, DEST, DEST + SRC, DEST, SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CMP(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x38:
    case 0x39:
    case 0x3A:
    case 0x3B:  Decode(format, opcode, "CMP", 1, opcode[0] & 0b11);                 break;
    case 0x3C:
    case 0x3D:  Decode(format, opcode, "CMP", 0, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "CMP", 1, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x83:  Decode(format, opcode, "CMP", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    }

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::DEC(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x48:
    case 0x49:
    case 0x4A:
    case 0x4B:
    case 0x4C:
    case 0x4D:
    case 0x4E:
    case 0x4F:
        format.instruction = "DEC";
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = opcode[0] & 0b111;
        break;
    case 0xFE:
    case 0xFF:  Decode(format, opcode, "DEC", 1, opcode[0] & 0b01);  break;
    }
    format.operand_count = 1;

    BEGIN_OPERATION() {
        UpdateFlags<OSZAP_, BORROW>(x86, DEST, DEST - 1, DEST, 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::DIV(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:
        Decode(format, opcode, "DIV", 1, opcode[0] & 0b11);
        format.operand_count = 3;
        format.operand[2] = format.operand[1];
        format.operand[1].type = Format::Operand::REG;
        format.operand[0].type = Format::Operand::REG;
        format.operand[1].flags = Format::Operand::HIDE;
        format.operand[0].flags = Format::Operand::HIDE;
        format.operand[1].base = (format.width == 8) ? IndexREG(ESP) : IndexREG(EDX);
        format.operand[0].base = IndexREG(EAX);
        break;
    }

    BEGIN_OPERATION() {
        const auto& SRC = SRC2;
        switch (sizeof(SRC)) {
        case sizeof(uint8_t):  { auto D = Tuple8(DEST2, DEST1);  DEST1 = uint8_t(D / SRC);  DEST2 = uint8_t(D % SRC);  break; }
        case sizeof(uint16_t): { auto D = Tuple16(DEST2, DEST1); DEST1 = uint16_t(D / SRC); DEST2 = uint16_t(D % SRC); break; }
        case sizeof(uint32_t): { auto D = Tuple32(DEST2, DEST1); DEST1 = uint32_t(D / SRC); DEST2 = uint32_t(D % SRC); break; }
#if HAVE_X64
        case sizeof(uint64_t): { auto D = Tuple64(DEST2, DEST1); DEST1 = uint64_t(D / SRC); DEST2 = uint64_t(D % SRC); break; }
#endif
        default: break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::IDIV(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:
        Decode(format, opcode, "IDIV", 1, opcode[0] & 0b11);
        format.operand_count = 3;
        format.operand[2] = format.operand[1];
        format.operand[1].type = Format::Operand::REG;
        format.operand[0].type = Format::Operand::REG;
        format.operand[1].flags = Format::Operand::HIDE;
        format.operand[0].flags = Format::Operand::HIDE;
        format.operand[1].base = (format.width == 8) ? IndexREG(ESP) : IndexREG(EDX);
        format.operand[0].base = IndexREG(EAX);
        break;
    }

    BEGIN_OPERATION() {
        const auto& SRC = SRC2;
        switch (sizeof(SRC)) {
        case sizeof(int8_t):  { auto D = int16_t(Tuple8(DEST2, DEST1));   DEST1 = int8_t(D / SRC);  DEST2 = int8_t(D % SRC);  break; }
        case sizeof(int16_t): { auto D = int32_t(Tuple16(DEST2, DEST1));  DEST1 = int16_t(D / SRC); DEST2 = int16_t(D % SRC); break; }
        case sizeof(int32_t): { auto D = int64_t(Tuple32(DEST2, DEST1));  DEST1 = int32_t(D / SRC); DEST2 = int32_t(D % SRC); break; }
#if HAVE_X64
        case sizeof(int64_t): { auto D = int128_t(Tuple64(DEST2, DEST1)); DEST1 = int64_t(D / SRC); DEST2 = int64_t(D % SRC); break; }
#endif
        default: break;
        }
    } END_OPERATION_SIGNED;
}
//------------------------------------------------------------------------------
void x86_instruction::IMUL(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x0F:
        Decode(format, opcode, "IMUL", 2, OPERAND_SIZE | DIRECTION);
        format.operand_count = 3;
        format.operand[2] = format.operand[1];
        format.operand[1] = format.operand[0];
        format.operand[1].flags = Format::Operand::HIDE;
        break;
    case 0x69:  Decode(format, opcode, "IMUL", 1, OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_SIZE); break;
    case 0x6B:  Decode(format, opcode, "IMUL", 1, OPERAND_SIZE | DIRECTION | THREE_OPERAND | IMM_8BIT); break;
    case 0xF6:
    case 0xF7:
        Decode(format, opcode, "IMUL", 1, opcode[0] & 0b11);
        format.operand_count = 3;
        format.operand[2] = format.operand[1];
        format.operand[1].type = Format::Operand::REG;
        format.operand[0].type = Format::Operand::REG;
        format.operand[1].flags = Format::Operand::HIDE;
        format.operand[0].flags = Format::Operand::HIDE;
        format.operand[1].base = (format.width == 8) ? IndexREG(ESP) : IndexREG(EDX);
        format.operand[0].base = IndexREG(EAX);
        break;
    }

    if (format.operand[0].flags == Format::Operand::HIDE) {
        BEGIN_OPERATION() {
            const auto& SRC = SRC2;
            switch (sizeof(SRC)) {
            case sizeof(int8_t):  { auto M = int16_t(SRC) * int8_t(DEST1);   DEST1 = int8_t(M);  DEST2 = (M >> 8);  CF = OF = (DEST2 != 0 && DEST2 != -1); break; }
            case sizeof(int16_t): { auto M = int32_t(SRC) * int16_t(DEST1);  DEST1 = int16_t(M); DEST2 = (M >> 16); CF = OF = (DEST2 != 0 && DEST2 != -1); break; }
            case sizeof(int32_t): { auto M = int64_t(SRC) * int32_t(DEST1);  DEST1 = int32_t(M); DEST2 = (M >> 32); CF = OF = (DEST2 != 0 && DEST2 != -1); break; }
#if HAVE_X64
            case sizeof(int64_t): { auto M = int128_t(SRC) * int64_t(DEST1); DEST1 = int64_t(M); DEST2 = (M >> 64); CF = OF = (DEST2 != 0 && DEST2 != -1); break; }
#endif
            default: break;
            }
        } END_OPERATION_SIGNED;
    }
    else {
        BEGIN_OPERATION() {
            switch (sizeof(DEST)) {
            case sizeof(int8_t):  { auto M = int16_t(SRC1) * int8_t(SRC2);   DEST = int8_t(M);  CF = OF = (int8_t(M) != M);  break; }
            case sizeof(int16_t): { auto M = int32_t(SRC1) * int16_t(SRC2);  DEST = int16_t(M); CF = OF = (int16_t(M) != M); break; }
            case sizeof(int32_t): { auto M = int64_t(SRC1) * int32_t(SRC2);  DEST = int32_t(M); CF = OF = (int32_t(M) != M); break; }
#if HAVE_X64
            case sizeof(int64_t): { auto M = int128_t(SRC1) * int64_t(SRC2); DEST = int64_t(M); CF = OF = (int64_t(M) != M); break; }
#endif
            default: break;
            }
        } END_OPERATION_SIGNED;
    }
}
//------------------------------------------------------------------------------
void x86_instruction::INC(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
        format.instruction = "INC";
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = opcode[0] & 0b111;
        break;
    case 0xFE:
    case 0xFF:  Decode(format, opcode, "INC", 1, opcode[0] & 0b01);  break;
    }
    format.operand_count = 1;

    BEGIN_OPERATION() {
        UpdateFlags<OSZAP_, CARRY>(x86, DEST, DEST + 1, DEST, 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MUL(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:
        Decode(format, opcode, "MUL", 1, opcode[0] & 0b11);
        format.operand_count = 3;
        format.operand[2] = format.operand[1];
        format.operand[1].type = Format::Operand::REG;
        format.operand[0].type = Format::Operand::REG;
        format.operand[1].flags = Format::Operand::HIDE;
        format.operand[0].flags = Format::Operand::HIDE;
        format.operand[1].base = (format.width == 8) ? IndexREG(ESP) : IndexREG(EDX);
        format.operand[0].base = IndexREG(EAX);
        break;
    }

    BEGIN_OPERATION() {
        const auto& SRC = SRC2;
        switch (sizeof(SRC)) {
        case sizeof(uint8_t):  { auto M = uint16_t(SRC) * DEST1;  DEST1 = uint8_t(M);  DEST2 = (M >> 8);  CF = OF = (DEST2 != 0); break; }
        case sizeof(uint16_t): { auto M = uint32_t(SRC) * DEST1;  DEST1 = uint16_t(M); DEST2 = (M >> 16); CF = OF = (DEST2 != 0); break; }
        case sizeof(uint32_t): { auto M = uint64_t(SRC) * DEST1;  DEST1 = uint32_t(M); DEST2 = (M >> 32); CF = OF = (DEST2 != 0); break; }
#if HAVE_X64
        case sizeof(uint64_t): { auto M = uint128_t(SRC) * DEST1; DEST1 = uint64_t(M); DEST2 = (M >> 64); CF = OF = (DEST2 != 0); break; }
#endif
        default: break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::NEG(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, opcode, "NEG", 1, opcode[0] & 0b01);  break;
    }
    format.operand_count = 1;

    BEGIN_OPERATION() {
        CF = DEST ? 1 : 0;
        UpdateFlags<_SZ_P_>(x86, DEST, -DEST);

        // Special case
        auto bc = DEST | -DEST;
        auto one = (DEST & 0) | 1;
        auto bits = sizeof(DEST) * 8;
        auto sign2 = one << (bits - 2);
        AF = bc &                   8 ? 1 : 0;
        OF = (bc ^ (bc >> 1)) & sign2 ? 1 : 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SBB(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:  Decode(format, opcode, "SBB", 1, opcode[0] & 0b11);                 break;
    case 0x1C:
    case 0x1D:  Decode(format, opcode, "SBB", 0, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "SBB", 1, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x83:  Decode(format, opcode, "SBB", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<OSZAPC, BORROW>(x86, DEST, DEST - (SRC + CF), DEST, SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SUB(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:  Decode(format, opcode, "SUB", 1, opcode[0] & 0b11);                 break;
    case 0x2C:
    case 0x2D:  Decode(format, opcode, "SUB", 0, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "SUB", 1, (opcode[0] & 0b01) | IMM_SIZE);    break;
    case 0x83:  Decode(format, opcode, "SUB", 1, (opcode[0] & 0b01) | IMM_8BIT);    break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<OSZAPC, BORROW>(x86, DEST, DEST - SRC, DEST, SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
