#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::Decode(Format& format, const char* instruction, int offset, int immediate_size, int flags)
{
    format.width = (flags & OPERAND_SIZE) ? format.width : 8;
    format.length = offset;
    format.instruction = instruction;

    if (flags & (IMMEDIATE | INDIRECT | RELATIVE)) {
        if (immediate_size < 0)
            immediate_size = format.width;
        if (flags & IMMEDIATE)  format.operand[0].type = Format::Operand::IMM;
        if (flags & INDIRECT)   format.operand[0].type = Format::Operand::ADR;
        if (flags & RELATIVE)   format.operand[0].type = Format::Operand::REL;
        format.operand[0].scale = 0;
        format.operand[0].index = 0;
        format.operand[0].base = -1;
        switch (immediate_size) {
        case 8:
            format.length += 1;
            format.operand[0].displacement = IMM8(opcode, format.length - 1);
            break;
        case 16:
            format.length += 2;
            format.operand[0].displacement = IMM16(opcode, format.length - 2);
            break;
        case 32:
            format.length += 4;
            format.operand[0].displacement = IMM32(opcode, format.length - 4);
            break;
        }
        return;
    }

    format.length += 1;
    int OPREG = (flags & DIRECTION) ? 0 : 1;
    int MODRM = (flags & DIRECTION) ? 1 : 0;
    if (offset) {
        int MOD = (opcode[offset] >> 6) & 0b11;
        int REG = (opcode[offset] >> 3) & 0b111;
        int RM = (opcode[offset] >> 0) & 0b111;

        int SS = (opcode[offset + 1] >> 6) & 0b11;
        int INDEX = (opcode[offset + 1] >> 3) & 0b111;
        int BASE = (opcode[offset + 1] >> 0) & 0b111;

        switch (opcode[offset] & 0b11000111) {
        case 0b00000100:
            format.length += 1;
            format.operand[MODRM].type = Format::Operand::ADR;
            format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.operand[MODRM].base = (BASE == 0b101) ? -1 : BASE;
            format.operand[MODRM].displacement = 0;
            break;
        case 0b01000100:
            format.length += 2;
            format.operand[MODRM].type = Format::Operand::ADR;
            format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.operand[MODRM].base = BASE;
            format.operand[MODRM].displacement = IMM8(opcode, format.length - 1);
            break;
        case 0b10000100:
            format.length += 5;
            format.operand[MODRM].type = Format::Operand::ADR;
            format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.operand[MODRM].base = BASE;
            format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
            break;
        default:
            switch (MOD) {
            case 0b00:
                format.operand[MODRM].type = Format::Operand::ADR;
                format.operand[MODRM].scale = 0;
                format.operand[MODRM].index = -1;
                format.operand[MODRM].base = RM;
                format.operand[MODRM].displacement = 0;
                if (RM == 0b101) {
                    format.length += 4;
                    format.operand[MODRM].base = -1;
                    format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
                }
                break;
            case 0b01:
                format.length += 1;
                format.operand[MODRM].type = Format::Operand::ADR;
                format.operand[MODRM].scale = 0;
                format.operand[MODRM].index = -1;
                format.operand[MODRM].base = RM;
                format.operand[MODRM].displacement = IMM8(opcode, format.length - 1);
                break;
            case 0b10:
                format.length += 4;
                format.operand[MODRM].type = Format::Operand::ADR;
                format.operand[MODRM].scale = 0;
                format.operand[MODRM].index = -1;
                format.operand[MODRM].base = RM;
                format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
                break;
            case 0b11:
                format.operand[MODRM].type = Format::Operand::REG;
                format.operand[MODRM].base = RM;
                break;
            }
            break;
        }
        format.operand[OPREG].type = Format::Operand::REG;
        format.operand[OPREG].base = REG;
        if (flags & THREE_OPERAND) {
            OPREG = 2;
        }
    }
    else {
        format.operand[MODRM].type = Format::Operand::REG;
        format.operand[OPREG].type = Format::Operand::REG;
        format.operand[MODRM].base = 0;
        format.operand[OPREG].base = 0;
    }

    if (immediate_size) {
        if (immediate_size < 0)
            immediate_size = format.width;
        switch (immediate_size) {
        case 8:
            format.length += 1;
            format.operand[OPREG].type = Format::Operand::IMM;
            format.operand[OPREG].displacement = IMM8(opcode, format.length - 1);
            break;
        case 16:
            format.length += 2;
            format.operand[OPREG].type = Format::Operand::IMM;
            format.operand[OPREG].displacement = IMM16(opcode, format.length - 2);
            break;
        case 32:
            format.length += 4;
            format.operand[OPREG].type = Format::Operand::IMM;
            format.operand[OPREG].displacement = IMM32(opcode, format.length - 4);
            break;
        }
    }
}
//------------------------------------------------------------------------------
std::string x86_instruction::Disasm(const Format& format)
{
    auto hex = [](auto imm) {
        char temp[64];
        int precision = 0;
        unsigned long long value = std::abs((int64_t)imm);
        if (value > 0xFFFFFFFF)     precision = 16;
        else if (value > 0xFFFF)    precision = 8;
        else if (value > 0xFF)      precision = 4;
        else                        precision = 2;
        snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "", precision, value);
        return std::string(temp);
    };

    std::string disasm;
    disasm += format.instruction;
    for (int i = 0; i < 3; ++i) {
        if (i) disasm += ',';
        disasm += ' ';
        switch (format.operand[i].type) {
        case Format::Operand::ADR:
            switch (format.width) {
            case 8:     disasm += "BYTE PTR";   break;
            case 16:    disasm += "WORD PTR";   break;
            case 32:    disasm += "DWORD PTR";  break;
            case 64:    disasm += "QWORD PTR";  break;
            }
            disasm += ' ';
            disasm += '[';
            if (format.operand[i].scale > 0) {
                disasm += REG32[format.operand[i].index];
                disasm += '*';
                disasm += std::to_string(format.operand[i].scale);
            }
            if (format.operand[i].base >= 0) {
                if (disasm.back() != '[')
                    disasm += '+';
                disasm += REG32[format.operand[i].base];
            }
            if (format.operand[i].displacement) {
                if (disasm.back() != '[')
                    disasm += '+';
                disasm += hex(format.operand[i].displacement);
            }
            if (disasm.back() == '[')
                disasm += '0';
            disasm += ']';
            break;
        case Format::Operand::IMM:
            disasm += hex(format.operand[i].displacement);
            break;
        case Format::Operand::REG:
            switch (format.width) {
            case 8:     disasm += REG8[format.operand[i].base];   break;
            case 16:    disasm += REG16[format.operand[i].base];  break;
            case 32:    disasm += REG32[format.operand[i].base];  break;
            case 64:    disasm += REG64[format.operand[i].base];  break;
            }
            break;
        case Format::Operand::REL:
            disasm += hex(x86.ip.q + format.operand[i].displacement);
            break;
        default:
            if (i) disasm.pop_back();
            disasm.pop_back();
            break;
        }
    }

    return disasm;
}
//------------------------------------------------------------------------------
void x86_instruction::Fixup(Format& format)
{
    for (int i = 0; i < 3; ++i) {
        switch (format.operand[i].type) {
        case Format::Operand::NOP:
            format.operand[i].address = 0;
            format.operand[i].memory = (uint8_t*)&format.operand[i].address;
            break;
        case Format::Operand::ADR:
            format.operand[i].address = 0;
            if (format.operand[i].scale > 0) {
                format.operand[i].address += regs[format.operand[i].index].d * format.operand[i].scale;
            }
            if (format.operand[i].base >= 0) {
                format.operand[i].address += regs[format.operand[i].base].d;
            }
            format.operand[i].address += format.operand[i].displacement;
            format.operand[i].memory = memory + format.operand[i].address;
            break;
        case Format::Operand::IMM:
            format.operand[i].address = format.operand[i].displacement;
            format.operand[i].memory = (uint8_t*)&format.operand[i].address;
            break;
        case Format::Operand::REG:
            if (format.width != 8 || format.operand[i].base < 4) {
                format.operand[i].memory = &regs[format.operand[i].base].l;
            }
            else {
                format.operand[i].memory = &regs[format.operand[i].base - 4].h;
            }
            break;
        case Format::Operand::REL:
            format.operand[i].memory = memory;
            format.operand[i].memory += x86.ip.q;
            format.operand[i].memory += format.operand[i].displacement;
            break;
        default:
            break;
        }
    }
}
//------------------------------------------------------------------------------
template<typename A, typename B>
void x86_instruction::UpdateFlags(x86_instruction& x86, A& DEST, B TEMP)
{
    int bits = sizeof(A) * 8;
    CF = ((DEST ^ TEMP) &  (1 << (bits - 1))) ? 1 : 0;
    PF = ((       TEMP) &  (1              )) ? 1 : 0;
    AF = ((DEST ^ TEMP) &  (1              )) ? 1 : 0;
    ZF = ((       TEMP) == (0              )) ? 1 : 0;
    SF = ((       TEMP) &  (1 << (bits - 1))) ? 1 : 0;
    OF = ((DEST ^ TEMP) &  (1 << (bits - 2))) ? 1 : 0;
    DEST = TEMP;
}
//------------------------------------------------------------------------------
void x86_instruction::_(Format& format)
{
    format.instruction = "UNKNOWN";

    BEGIN_OPERATION() {
    } END_OPERATION;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::CS(Format& format)
{
}
//------------------------------------------------------------------------------
void x86_instruction::SS(Format& format)
{
}
//------------------------------------------------------------------------------
void x86_instruction::DS(Format& format)
{
}
//------------------------------------------------------------------------------
void x86_instruction::ES(Format& format)
{
}
//------------------------------------------------------------------------------
void x86_instruction::FS(Format& format)
{
}
//------------------------------------------------------------------------------
void x86_instruction::GS(Format& format)
{
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::BSF(Format& format)
{
    Decode(format, "BSF", 2, 0, OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = __builtin_ctz(SRC);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::BSR(Format& format)
{
    Decode(format, "BSR", 2, 0, OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = __builtin_clz(SRC) ^ (sizeof(SRC) * 8 - 1);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CALL(Format& format)
{
    switch (opcode[0]) {
    case 0xE8:  Decode(format, "CALL", 1, -1, OPERAND_SIZE | RELATIVE);             break;
    case 0xFF:
        switch (opcode[1] & 0b00111000) {
        case 0b00010000:    Decode(format, "CALL", 1,  0, OPERAND_SIZE);            break;
//      case 0b00011000:    Decode(format, "CALL", 2, -1, OPERAND_SIZE | INDIRECT); break;
        }
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        Push(EIP);
        if (format.operand[0].type == Format::Operand::REL)
            EIP += format.operand[0].displacement;
        else
            EIP = *(uint32_t*)format.operand[0].memory;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CDQ(Format& format)
{
    format.instruction = (format.width == 16) ? "CWD" : "CDQ";

    BEGIN_OPERATION() {
        if (format.width == 16)
            DX = (int16_t)AX < 0 ? 0xFFFF : 0x0000;
        else
            EDX = (int32_t)EAX < 0 ? 0xFFFFFFFF : 0x00000000;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CLC(Format& format)
{
    format.instruction = "CLC";

    BEGIN_OPERATION() {
        CF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CLD(Format& format)
{
    format.instruction = "CLD";

    BEGIN_OPERATION() {
        DF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CMC(Format& format)
{
    format.instruction = "CMC";

    BEGIN_OPERATION() {
        CF = !CF;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CMP(Format& format)
{
    switch (opcode[0]) {
    case 0x38:
    case 0x39:
    case 0x3A:
    case 0x3B:  Decode(format, "CMP", 1, 0, opcode[0] & 0b11);  break;
    case 0x3C:
    case 0x3D:  Decode(format, "CMP", 0, -1, opcode[0] & 0b01); break;
    case 0x80:
    case 0x81:  Decode(format, "CMP", 1, -1, opcode[0] & 0b01); break;
    case 0x83:  Decode(format, "CMP", 1,  8, opcode[0] & 0b01); break;
    }

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags(x86, TEMP, TEMP - SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CWDE(Format& format)
{
    format.instruction = (format.width == 16) ? "CBW" : "CWDE";

    BEGIN_OPERATION() {
        if (format.width == 16)
            AX = (int8_t)AL;
        else
            EAX = (int16_t)AX;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::ENTER(Format& format)
{
    format.length = 4;
    format.instruction = "ENTER";
    format.operand[0].type = Format::Operand::IMM;
    format.operand[1].type = Format::Operand::IMM;
    format.operand[0].displacement = IMM16(opcode, 1);
    format.operand[1].displacement = IMM8(opcode, 3);

    BEGIN_OPERATION() {
        int level = format.operand[1].displacement % 32;
        Push(ESP);
        if (level > 0) {
            uint32_t frame_ptr = ESP;
            for (int i = 1; i < level; ++i) {
                EBP = EBP - 4;
                Push(EBP);
            }
            Push(frame_ptr);
        }
        ESP -= (uint16_t)format.operand[1].displacement;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::Jcc(Format& format)
{
    switch (opcode[0]) {
    case 0x70:  Decode(format, "JO",  1, 8, RELATIVE);  break;
    case 0x71:  Decode(format, "JNO", 1, 8, RELATIVE);  break;
    case 0x72:  Decode(format, "JC",  1, 8, RELATIVE);  break;
    case 0x73:  Decode(format, "JNC", 1, 8, RELATIVE);  break;
    case 0x74:  Decode(format, "JZ",  1, 8, RELATIVE);  break;
    case 0x75:  Decode(format, "JNZ", 1, 8, RELATIVE);  break;
    case 0x76:  Decode(format, "JBE", 1, 8, RELATIVE);  break;
    case 0x77:  Decode(format, "JA",  1, 8, RELATIVE);  break;
    case 0x78:  Decode(format, "JS",  1, 8, RELATIVE);  break;
    case 0x79:  Decode(format, "JNS", 1, 8, RELATIVE);  break;
    case 0x7A:  Decode(format, "JPE", 1, 8, RELATIVE);  break;
    case 0x7B:  Decode(format, "JPO", 1, 8, RELATIVE);  break;
    case 0x7C:  Decode(format, "JL",  1, 8, RELATIVE);  break;
    case 0x7D:  Decode(format, "JGE", 1, 8, RELATIVE);  break;
    case 0x7E:  Decode(format, "JLE", 1, 8, RELATIVE);  break;
    case 0x7F:  Decode(format, "JG",  1, 8, RELATIVE);  break;
    case 0xE3:  Decode(format, (format.address == 16) ? "JCXZ" : "JECXZ", 1, 8, RELATIVE); break;
    case 0x0F:
        switch (opcode[1]) {
        case 0x80:  Decode(format, "JO",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x81:  Decode(format, "JNO", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x82:  Decode(format, "JC",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x83:  Decode(format, "JNC", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x84:  Decode(format, "JZ",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x85:  Decode(format, "JNZ", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x86:  Decode(format, "JBE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x87:  Decode(format, "JA",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x88:  Decode(format, "JS",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x89:  Decode(format, "JNS", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8A:  Decode(format, "JPE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8B:  Decode(format, "JPO", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8C:  Decode(format, "JL",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8D:  Decode(format, "JGE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8E:  Decode(format, "JLE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8F:  Decode(format, "JG",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        }
        break;
    }

    BEGIN_OPERATION() {
        // TODO
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::JMP(Format& format)
{
    switch (opcode[0]) {
    case 0xE9:  Decode(format, "JMP", 1, -1, OPERAND_SIZE | RELATIVE);              break;
    case 0xEB:  Decode(format, "JMP", 1,  8, RELATIVE);                             break;
    case 0xFF:
        switch (opcode[1] & 0b00111000) {
        case 0b00100000:    Decode(format, "JMP", 1, 0, OPERAND_SIZE);              break;
//      case 0b00101000:    Decode(format, "JMP", 2, -1, OPERAND_SIZE | INDIRECT);  break;
        }
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::REL)
            EIP += format.operand[0].displacement;
        else
            EIP = *(uint32_t*)format.operand[0].memory;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::LAHF(Format& format)
{
    format.instruction = "LAHF";

    BEGIN_OPERATION() {
        AH = FLAGS;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::LEA(Format& format)
{
    Decode(format, "LEA", 1, 0, OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = (decltype(DEST))format.operand[0].address;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::LEAVE(Format& format)
{
    format.instruction = "LEAVE";

    BEGIN_OPERATION() {
        ESP = EBP;
        EBP = Pop();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::LOOP(Format& format)
{
    switch (opcode[0]) {
    case 0xE0:  Decode(format, "LOOPNZ", 1, 8, RELATIVE);   break;
    case 0xE1:  Decode(format, "LOOPZ", 1, 8, RELATIVE);    break;
    case 0xE2:  Decode(format, "LOOP", 1, 8, RELATIVE);     break;
    }

    BEGIN_OPERATION() {
        uint32_t CountReg = (format.width == 16) ? CX : ECX;
        if (CountReg) {
            ECX = ECX - 1;
            bool BranchCond = true;
            switch (x86.opcode[0]) {
            case 0xE0:  BranchCond = (ZF == 0); break;
            case 0xE1:  BranchCond = (ZF != 0); break;
            }
            if (BranchCond) {
                EIP += format.operand[1].displacement;
            }
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MOV(Format& format)
{
    switch (opcode[0]) {
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:  Decode(format, "MOV", 1, 0, opcode[0] & 0b11);  break;
    case 0xB0:
    case 0xB1:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB7:  Decode(format, "MOV", 0, 8);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111;     break;
    case 0xB8:
    case 0xB9:
    case 0xBA:
    case 0xBB:
    case 0xBC:
    case 0xBD:
    case 0xBE:
    case 0xBF:  Decode(format, "MOV", 0, -1, OPERAND_SIZE);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111;     break;
    case 0xC6:  Decode(format, "MOV", 1, 8);                    break;
    case 0xC7:  Decode(format, "MOV", 1, -1, OPERAND_SIZE);     break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MOVSX(Format& format)
{
    switch (opcode[1]) {
    case 0xBE:
    case 0xBF:  Decode(format, "MOVSX", 2, 0, opcode[1] & 0b11);    break;
    }

    BEGIN_OPERATION() {
        switch (format.width) {
        case 8:
            (uint16_t&)DEST = SRC;
            break;
        case 16:
            (uint32_t&)DEST = SRC;
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MOVZX(Format& format)
{
    switch (opcode[1]) {
    case 0xB6:
    case 0xB7:  Decode(format, "MOVZX", 2, 0, opcode[1] & 0b11);    break;
    }

    BEGIN_OPERATION() {
        switch (format.width) {
        case 8:
            (uint16_t&)DEST = SRC;
            break;
        case 16:
            (uint32_t&)DEST = SRC;
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::NOP(Format& format)
{
    switch (opcode[0]) {
    case 0x90:  Decode(format, "NOP");                          break;
    case 0x0F:  Decode(format, "NOP", 2, 0, opcode[1] & 0b11);  break;
    }
    format.operand[0].type = Format::Operand::NOP;
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::POP(Format& format)
{
    switch (opcode[0]) {
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F:  Decode(format, "POP", 0, 0, OPERAND_SIZE);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111; break;
    case 0x8F:  Decode(format, "POP", 1, 0, OPERAND_SIZE);  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        DEST = Pop();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::POPAD(Format& format)
{
    format.instruction = "POPAD";

    BEGIN_OPERATION() {
        EDI = Pop();
        ESI = Pop();
        EBP = Pop();
        Pop();
        EBX = Pop();
        EDX = Pop();
        ECX = Pop();
        EAX = Pop();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::POPFD(Format& format)
{
    format.instruction = "POPFD";

    BEGIN_OPERATION() {
        EFLAGS = Pop();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::PUSH(Format& format)
{
    switch (opcode[0]) {
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:  Decode(format, "PUSH", 0,  0, OPERAND_SIZE);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111;                 break;
    case 0x68:  Decode(format, "PUSH", 1, -1, OPERAND_SIZE | IMMEDIATE);    break;
    case 0x6A:  Decode(format, "PUSH", 1,  8, OPERAND_SIZE | IMMEDIATE);    break;
    case 0xFF:  Decode(format, "PUSH", 1,  0, OPERAND_SIZE);                break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        Push(DEST);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::PUSHAD(Format& format)
{
    format.instruction = "PUSHAD";

    BEGIN_OPERATION() {
        auto Temp = ESP;
        Push(EAX);
        Push(ECX);
        Push(EDX);
        Push(EBX);
        Push(Temp);
        Push(EBP);
        Push(ESI);
        Push(EDI);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::PUSHFD(Format& format)
{
    format.instruction = "PUSHFD";

    BEGIN_OPERATION() {
        Push(EFLAGS);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::REP(Format& format)
{
    format.repeat = true;
}
//------------------------------------------------------------------------------
void x86_instruction::RET(Format& format)
{
    format.instruction = "RET";
    switch (opcode[0]) {
    case 0xC2:
        format.length += 2;
        format.operand[0].type = Format::Operand::IMM;
        format.operand[0].displacement = IMM16(opcode, 1);
        break;
    case 0xC3:
        break;
    }

    BEGIN_OPERATION() {
        EIP = Pop();
        ESP = ESP + format.operand[0].displacement;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SAHF(Format& format)
{
    format.instruction = "SAHF";

    BEGIN_OPERATION() {
        FLAGS = AH;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SETcc(Format& format)
{
    switch (opcode[1]) {
    case 0x90:  Decode(format, "SETO", 2);    break;
    case 0x91:  Decode(format, "SETNO", 2);   break;
    case 0x92:  Decode(format, "SETC", 2);    break;
    case 0x93:  Decode(format, "SETNC", 2);   break;
    case 0x94:  Decode(format, "SETZ", 2);    break;
    case 0x95:  Decode(format, "SETNZ", 2);   break;
    case 0x96:  Decode(format, "SETBE", 2);   break;
    case 0x97:  Decode(format, "SETA", 2);    break;
    case 0x98:  Decode(format, "SETS", 2);    break;
    case 0x99:  Decode(format, "SETNS", 2);   break;
    case 0x9A:  Decode(format, "SETPE", 2);   break;
    case 0x9B:  Decode(format, "SETPO", 2);   break;
    case 0x9C:  Decode(format, "SETL", 2);    break;
    case 0x9D:  Decode(format, "SETGE", 2);   break;
    case 0x9E:  Decode(format, "SETLE", 2);   break;
    case 0x9F:  Decode(format, "SETG", 2);    break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        // TODO
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::STC(Format& format)
{
    format.instruction = "STC";

    BEGIN_OPERATION() {
        CF = 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::STD(Format& format)
{
    format.instruction = "STD";

    BEGIN_OPERATION() {
        DF = 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::TEST(Format& format)
{
    switch (opcode[0]) {
    case 0x84:
    case 0x85:  Decode(format, "TEST", 1, 0, opcode[0] & 0b01);     break;
    case 0xA8:
    case 0xA9:  Decode(format, "TEST", 0, -1, opcode[0] & 0b01);    break;
    case 0xF6:
    case 0xF7:  Decode(format, "TEST", 1, -1, opcode[0] & 0b01);    break;
    }

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags(x86, TEMP, TEMP & SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::XCHG(Format& format)
{
    switch (opcode[0]) {
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
        format.instruction = "XCHG";
        format.operand[0].type = Format::Operand::REG;
        format.operand[1].type = Format::Operand::REG;
        format.operand[0].base = REG(EAX);
        format.operand[1].base = (opcode[0] & 0b111);
        break;
    case 0x86:
    case 0x87:
        Decode(format, "XCHG", 1, 0, opcode[0] & 0b01);
        break;
    }

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        DEST = SRC;
        SRC = TEMP;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::XLAT(Format& format)
{
    format.instruction = "XLAT";

    BEGIN_OPERATION() {
        AL = *(x86.memory + EBX + AL);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
