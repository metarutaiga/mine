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
    case 0x13:  Decode(format, opcode, "ADC", 1,  0, opcode[0] & 0b11); break;
    case 0x14:
    case 0x15:  Decode(format, opcode, "ADC", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "ADC", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, opcode, "ADC", 1,  8, opcode[0] & 0b01); break;
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
    case 0x03:  Decode(format, opcode, "ADD", 1,  0, opcode[0] & 0b11); break;
    case 0x04:
    case 0x05:  Decode(format, opcode, "ADD", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "ADD", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, opcode, "ADD", 1,  8, opcode[0] & 0b01); break;
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
    case 0x3B:  Decode(format, opcode, "CMP", 1,  0, opcode[0] & 0b11); break;
    case 0x3C:
    case 0x3D:  Decode(format, opcode, "CMP", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "CMP", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, opcode, "CMP", 1,  8, opcode[0] & 0b01); break;
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
        format.operand[0].base = opcode[0] - 0x48;
        break;
    case 0xFE:
    case 0xFF:  Decode(format, opcode, "DEC", 1, 0, opcode[0] & 0b01);  break;
    }

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
        Decode(format, opcode, "DIV", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::NOP;
        break;
    }

    BEGIN_OPERATION() {
        auto Q = SRC;
        auto R = SRC;
        switch (sizeof(DEST)) {
        case sizeof(uint8_t):  Q = AL / SRC;  R = AL % SRC;  AL = Q;  AH = R;  break;
        case sizeof(uint16_t): Q = AX / SRC;  R = AX % SRC;  AX = Q;  DX = R;  break;
        case sizeof(uint32_t): Q = EAX / SRC; R = EAX % SRC; EAX = Q; EDX = R; break;
#if HAVE_X64
        case sizeof(uint64_t): Q = RAX / SRC; R = RAX % SRC; RAX = Q; RDX = R; break;
#endif
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::IDIV(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:
        Decode(format, opcode, "IDIV", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::NOP;
        break;
    }

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
        auto Q = src;
        auto R = src;
        switch (sizeof(DEST)) {
        case sizeof(int8_t):  Q = AL / src;  R = AL % src;  AL = Q;  AH = R;  break;
        case sizeof(int16_t): Q = AX / src;  R = AX % src;  AX = Q;  DX = R;  break;
        case sizeof(int32_t): Q = EAX / src; R = EAX % src; EAX = Q; EDX = R; break;
#if HAVE_X64
        case sizeof(int64_t): Q = RAX / src; R = RAX % src; RAX = Q; RDX = R; break;
#endif
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::IMUL(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x0F:  Decode(format, opcode, "IMUL", 2,  0, OPERAND_SIZE | DIRECTION);                    break;
    case 0x69:  Decode(format, opcode, "IMUL", 1, -1, OPERAND_SIZE | DIRECTION | THREE_OPERAND);    break;
    case 0x6B:  Decode(format, opcode, "IMUL", 1,  8, OPERAND_SIZE | DIRECTION | THREE_OPERAND);    break;
    case 0xF6:
    case 0xF7:
        Decode(format, opcode, "IMUL", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::NOP;
        break;
    }

    if (format.operand[0].type == Format::Operand::NOP) {
        BEGIN_OPERATION() {
            typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
            auto M = std::make_widened_t<decltype(src)>(src);
            switch (sizeof(SRC)) {
            case sizeof(int8_t):  M = M * AL;  AL = int8_t(M);   AH = int8_t(M >> 8);    CF = OF = (AL != M);  break;
            case sizeof(int16_t): M = M * AX;  AX = int16_t(M);  DX = int16_t(M >> 16);  CF = OF = (AX != M);  break;
            case sizeof(int32_t): M = M * EAX; EAX = int32_t(M); EDX = int32_t(M >> 32); CF = OF = (EAX != M); break;
#if HAVE_X64
            case sizeof(int64_t): M = M * RAX; RAX = int64_t(M); RDX = int64_t(M >> 64); CF = OF = (RAX != M); break;
#endif
            }
        } END_OPERATION;
    }
    else if (format.operand[2].type == Format::Operand::NOP) {
        BEGIN_OPERATION() {
            typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
            typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
            auto M = std::make_widened_t<decltype(dest)>(dest);
            M = M * src;
            DEST = decltype(dest)(M);
            CF = OF = (dest != M);
        } END_OPERATION;
    }
    else {
        BEGIN_OPERATION() {
            typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
            typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src1 = SRC1;
            typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src2 = SRC2;
            auto M = std::make_widened_t<decltype(dest)>(src1);
            M = M * src2;
            DEST = decltype(dest)(M);
            CF = OF = (dest != M);
        } END_OPERATION;
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
        format.operand[0].base = opcode[0] - 0x40;
        break;
    case 0xFE:
    case 0xFF:  Decode(format, opcode, "INC", 1, 0, opcode[0] & 0b01);  break;
    }

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
        Decode(format, opcode, "MUL", 1, 0, opcode[0] & 0b11);
        format.operand[0].type = Format::Operand::NOP;
        break;
    }

    BEGIN_OPERATION() {
        auto M = std::make_widened_t<decltype(SRC)>(SRC);
        switch (sizeof(SRC)) {
        case sizeof(int8_t):  M = M * AL;  AL = uint8_t(M);   AH = uint8_t(M >> 8);    CF = OF = (AH != 0);  break;
        case sizeof(int16_t): M = M * AX;  AX = uint16_t(M);  DX = uint16_t(M >> 16);  CF = OF = (DX != 0);  break;
        case sizeof(int32_t): M = M * EAX; EAX = uint32_t(M); EDX = uint32_t(M >> 32); CF = OF = (EDX != 0); break;
#if HAVE_X64
        case sizeof(int64_t): M = M * RAX; RAX = uint64_t(M); RDX = uint64_t(M >> 64); CF = OF = (RDX != 0); break;
#endif
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::NEG(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, opcode, "NEG", 1, 0, opcode[0] & 0b01);  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        CF = DEST ? 1 : 0;
        UpdateFlags<_SZ_P_>(x86, DEST, -DEST);

        // Special case
        auto bc = DEST | -DEST;
        auto one = 1;
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
    case 0x1B:  Decode(format, opcode, "SBB", 1,  0, opcode[0] & 0b11); break;
    case 0x1C:
    case 0x1D:  Decode(format, opcode, "SBB", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "SBB", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, opcode, "SBB", 1,  8, opcode[0] & 0b01); break;
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
    case 0x2B:  Decode(format, opcode, "SUB", 1,  0, opcode[0] & 0b11); break;
    case 0x2C:
    case 0x2D:  Decode(format, opcode, "SUB", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, opcode, "SUB", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, opcode, "SUB", 1,  8, opcode[0] & 0b01); break;
    }

    BEGIN_OPERATION() {
        UpdateFlags<OSZAPC, BORROW>(x86, DEST, DEST - SRC, DEST, SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
