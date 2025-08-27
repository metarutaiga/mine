#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::Decode(Format& format, const uint8_t* opcode, const char* instruction, int offset, int immediate_size, int flags)
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
            format.operand[MODRM].base = BASE;
            format.operand[MODRM].displacement = 0;
            if (BASE == 0b101) {
                format.length += 4;
                format.operand[MODRM].base = -1;
                format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
            }
            break;
        case 0b00000101:
            format.length += 4;
            format.operand[MODRM].type = Format::Operand::ADR;
            format.operand[MODRM].scale = 0;
            format.operand[MODRM].index = -1;
            format.operand[MODRM].base = -1;
            format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
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
std::string x86_instruction::Disasm(const Format& format, x86_instruction& x86)
{
    auto hex = [](auto imm) {
        char temp[64];
        unsigned long long value = std::abs((int64_t)imm);
        if (value > 0xFFFFFFFF)     snprintf(temp, 64, "%s%0*llX",                 "", 16, imm);
        else if (value > 0xFFFF)    snprintf(temp, 64, "%s%0*llX",                 "",  8, imm & 0xFFFFFFFF);
        else if (value > 0xFF)      snprintf(temp, 64, "%s%0*llX",                 "",  4, imm & 0xFFFF);
        else if (value > 0x9)       snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  2, value);
        else                        snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  1, value);
        return std::string(temp);
    };

    std::string disasm;
    if (format.repeat) {
        disasm += "REP";
        disasm += ' ';
    }
    disasm += format.instruction;

    size_t offset = disasm.size();
    for (int i = 0; i < 3; ++i) {
        if (disasm.size() > offset)
            disasm += ',';
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
            if (format.segment[0]) {
                disasm += format.segment;
                disasm += ':';
            }
            disasm += '[';
            if (format.operand[i].scale > 0) {
                disasm += REG32[format.operand[i].index];
                if (format.operand[i].scale > 1) {
                    disasm += '*';
                    disasm += std::to_string(format.operand[i].scale);
                }
            }
            if (format.operand[i].base >= 0) {
                if (disasm.back() != '[')
                    disasm += '+';
                disasm += REG32[format.operand[i].base];
            }
            if (format.operand[i].displacement) {
                if (disasm.back() != '[' && format.operand[i].displacement >= 0)
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
            if (disasm.size() > offset)
                disasm.pop_back();
            if (disasm.size() > offset)
                disasm.pop_back();
            break;
        }
    }

    return disasm;
}
//------------------------------------------------------------------------------
void x86_instruction::Fixup(Format& format, x86_instruction& x86)
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
                format.operand[i].address += x86.regs[format.operand[i].index].d * format.operand[i].scale;
            }
            if (format.operand[i].base >= 0) {
                format.operand[i].address += x86.regs[format.operand[i].base].d;
            }
            format.operand[i].address += format.operand[i].displacement;
            format.operand[i].address = uint32_t(format.operand[i].address);
            format.operand[i].memory = x86.memory_address + format.operand[i].address;
            break;
        case Format::Operand::IMM:
            format.operand[i].address = format.operand[i].displacement;
            format.operand[i].memory = (uint8_t*)&format.operand[i].address;
            break;
        case Format::Operand::REG:
            if (format.width != 8 || format.operand[i].base < 4) {
                format.operand[i].memory = &x86.regs[format.operand[i].base].l;
            }
            else {
                format.operand[i].memory = &x86.regs[format.operand[i].base - 4].h;
            }
            break;
        case Format::Operand::REL:
            format.operand[i].address = x86.ip.q;
            format.operand[i].address += format.operand[i].displacement;
            format.operand[i].address = uint32_t(format.operand[i].address);
            format.operand[i].memory = x86.memory_address + format.operand[i].address;
            break;
        default:
            break;
        }
    }
}
//------------------------------------------------------------------------------
void x86_instruction::_(Format& format, const uint8_t* opcode)
{
    format.instruction = "UNKNOWN";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {};
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::CS(Format& format, const uint8_t* opcode)
{
    format.segment = "CS";
}
//------------------------------------------------------------------------------
void x86_instruction::SS(Format& format, const uint8_t* opcode)
{
    format.segment = "SS";
}
//------------------------------------------------------------------------------
void x86_instruction::DS(Format& format, const uint8_t* opcode)
{
    format.segment = "DS";
}
//------------------------------------------------------------------------------
void x86_instruction::ES(Format& format, const uint8_t* opcode)
{
    format.segment = "ES";
}
//------------------------------------------------------------------------------
void x86_instruction::FS(Format& format, const uint8_t* opcode)
{
    format.segment = "FS";
}
//------------------------------------------------------------------------------
void x86_instruction::GS(Format& format, const uint8_t* opcode)
{
    format.segment = "GS";
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::CALL(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xE8:  Decode(format, opcode, "CALL", 1, -1, OPERAND_SIZE | RELATIVE);             break;
    case 0xFF:
        switch (opcode[1] & 0b00111000) {
        case 0b00010000:    Decode(format, opcode, "CALL", 1,  0, OPERAND_SIZE);            break;
//      case 0b00011000:    Decode(format, opcode, "CALL", 2, -1, OPERAND_SIZE | INDIRECT); break;
        }
    }
    format.operand[1].type = Format::Operand::NOP;
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format& format, void*, const void*, const void*) {
        Push32(EIP);
        if (format.operand[0].type == Format::Operand::REL)
            EIP += format.operand[0].displacement;
        else
            EIP = *(uint32_t*)format.operand[0].memory;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CDQ(Format& format, const uint8_t* opcode)
{
    format.instruction = (format.width == 16) ? "CWD" : "CDQ";

    BEGIN_OPERATION() {
        if (sizeof(DEST) == sizeof(int16_t))
            DX = (int16_t)AX < 0 ? 0xFFFF : 0x0000;
        else
            EDX = (int32_t)EAX < 0 ? 0xFFFFFFFF : 0x00000000;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CLC(Format& format, const uint8_t* opcode)
{
    format.instruction = "CLC";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        CF = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CLD(Format& format, const uint8_t* opcode)
{
    format.instruction = "CLD";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        DF = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CMC(Format& format, const uint8_t* opcode)
{
    format.instruction = "CMC";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        CF = !CF;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CWDE(Format& format, const uint8_t* opcode)
{
    format.instruction = (format.width == 16) ? "CBW" : "CWDE";

    BEGIN_OPERATION() {
        if (sizeof(DEST) == sizeof(int16_t))
            AX = (int8_t)AL;
        else
            EAX = (int16_t)AX;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::ENTER(Format& format, const uint8_t* opcode)
{
    format.length = 4;
    format.instruction = "ENTER";
    format.operand[0].type = Format::Operand::IMM;
    format.operand[1].type = Format::Operand::IMM;
    format.operand[0].displacement = IMM16(opcode, 1);
    format.operand[1].displacement = IMM8(opcode, 3);
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format& format, void*, const void*, const void*) {
        int level = format.operand[1].displacement % 32;
        Push32(ESP);
        if (level > 0) {
            uint32_t frame_ptr = ESP;
            for (int i = 1; i < level; ++i) {
                EBP = EBP - 4;
                Push32(EBP);
            }
            Push32(frame_ptr);
        }
        ESP -= (uint16_t)format.operand[1].displacement;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::Jcc(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x70:  Decode(format, opcode, "JO",  1, 8, RELATIVE);  break;
    case 0x71:  Decode(format, opcode, "JNO", 1, 8, RELATIVE);  break;
    case 0x72:  Decode(format, opcode, "JC",  1, 8, RELATIVE);  break;
    case 0x73:  Decode(format, opcode, "JNC", 1, 8, RELATIVE);  break;
    case 0x74:  Decode(format, opcode, "JZ",  1, 8, RELATIVE);  break;
    case 0x75:  Decode(format, opcode, "JNZ", 1, 8, RELATIVE);  break;
    case 0x76:  Decode(format, opcode, "JBE", 1, 8, RELATIVE);  break;
    case 0x77:  Decode(format, opcode, "JA",  1, 8, RELATIVE);  break;
    case 0x78:  Decode(format, opcode, "JS",  1, 8, RELATIVE);  break;
    case 0x79:  Decode(format, opcode, "JNS", 1, 8, RELATIVE);  break;
    case 0x7A:  Decode(format, opcode, "JPE", 1, 8, RELATIVE);  break;
    case 0x7B:  Decode(format, opcode, "JPO", 1, 8, RELATIVE);  break;
    case 0x7C:  Decode(format, opcode, "JL",  1, 8, RELATIVE);  break;
    case 0x7D:  Decode(format, opcode, "JGE", 1, 8, RELATIVE);  break;
    case 0x7E:  Decode(format, opcode, "JLE", 1, 8, RELATIVE);  break;
    case 0x7F:  Decode(format, opcode, "JG",  1, 8, RELATIVE);  break;
    case 0xE3:  Decode(format, opcode, (format.address == 16) ? "JCXZ" : "JECXZ", 1, 8, RELATIVE); break;
    case 0x0F:
        switch (opcode[1]) {
        case 0x80:  Decode(format, opcode, "JO",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x81:  Decode(format, opcode, "JNO", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x82:  Decode(format, opcode, "JC",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x83:  Decode(format, opcode, "JNC", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x84:  Decode(format, opcode, "JZ",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x85:  Decode(format, opcode, "JNZ", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x86:  Decode(format, opcode, "JBE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x87:  Decode(format, opcode, "JA",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x88:  Decode(format, opcode, "JS",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x89:  Decode(format, opcode, "JNS", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8A:  Decode(format, opcode, "JPE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8B:  Decode(format, opcode, "JPO", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8C:  Decode(format, opcode, "JL",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8D:  Decode(format, opcode, "JGE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8E:  Decode(format, opcode, "JLE", 2, -1, OPERAND_SIZE | RELATIVE);  break;
        case 0x8F:  Decode(format, opcode, "JG",  2, -1, OPERAND_SIZE | RELATIVE);  break;
        }
        break;
    }
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format& format, void*, const void*, const void*) {
        bool condition = false;
        switch (x86.opcode[0]) {
        case 0x70:  condition = ((OF)          ) == 1;  break;  // JO
        case 0x71:  condition = ((OF)          ) == 0;  break;  // JNO
        case 0x72:  condition = ((CF)          ) == 1;  break;  // JC
        case 0x73:  condition = ((CF)          ) == 0;  break;  // JNC
        case 0x74:  condition = ((ZF)          ) == 1;  break;  // JZ
        case 0x75:  condition = ((ZF)          ) == 0;  break;  // JNZ
        case 0x76:  condition = ((CF | ZF)     ) == 1;  break;  // JBE
        case 0x77:  condition = ((CF | ZF)     ) == 0;  break;  // JA
        case 0x78:  condition = ((SF)          ) == 1;  break;  // JS
        case 0x79:  condition = ((SF)          ) == 0;  break;  // JNS
        case 0x7A:  condition = ((PF)          ) == 1;  break;  // JPE
        case 0x7B:  condition = ((PF)          ) == 0;  break;  // JPO
        case 0x7C:  condition = ((SF ^ OF)     ) == 1;  break;  // JL
        case 0x7D:  condition = ((SF ^ OF)     ) == 0;  break;  // JGE
        case 0x7E:  condition = ((SF ^ OF) | ZF) == 1;  break;  // JLE
        case 0x7F:  condition = ((SF ^ OF) | ZF) == 0;  break;  // JG
        case 0xE3:  (format.address == 16) ?
                    condition = ((CX)          ) == 0:          // JCXZ
                    condition = ((ECX)         ) == 0;  break;  // JECXZ
        case 0x0F:
            switch (x86.opcode[1]) {
            case 0x80:  condition = ((OF)          ) == 1;  break;  // JO
            case 0x81:  condition = ((OF)          ) == 0;  break;  // JNO
            case 0x82:  condition = ((CF)          ) == 1;  break;  // JC
            case 0x83:  condition = ((CF)          ) == 0;  break;  // JNC
            case 0x84:  condition = ((ZF)          ) == 1;  break;  // JZ
            case 0x85:  condition = ((ZF)          ) == 0;  break;  // JNZ
            case 0x86:  condition = ((CF | ZF)     ) == 1;  break;  // JBE
            case 0x87:  condition = ((CF | ZF)     ) == 0;  break;  // JA
            case 0x88:  condition = ((SF)          ) == 1;  break;  // JS
            case 0x89:  condition = ((SF)          ) == 0;  break;  // JNS
            case 0x8A:  condition = ((PF)          ) == 1;  break;  // JPE
            case 0x8B:  condition = ((PF)          ) == 0;  break;  // JPO
            case 0x8C:  condition = ((SF ^ OF)     ) == 1;  break;  // JL
            case 0x8D:  condition = ((SF ^ OF)     ) == 0;  break;  // JGE
            case 0x8E:  condition = ((SF ^ OF) | ZF) == 1;  break;  // JLE
            case 0x8F:  condition = ((SF ^ OF) | ZF) == 0;  break;  // JG
            }
            break;
        }
        if (condition) {
            EIP += format.operand[0].displacement;
        }
    };
}
//------------------------------------------------------------------------------
void x86_instruction::JMP(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xE9:  Decode(format, opcode, "JMP", 1, -1, OPERAND_SIZE | RELATIVE);              break;
    case 0xEB:  Decode(format, opcode, "JMP", 1,  8, RELATIVE);                             break;
    case 0xFF:
        switch (opcode[1] & 0b00111000) {
        case 0b00100000:    Decode(format, opcode, "JMP", 1, 0, OPERAND_SIZE);              break;
//      case 0b00101000:    Decode(format, opcode, "JMP", 2, -1, OPERAND_SIZE | INDIRECT);  break;
        }
    }
    format.operand[1].type = Format::Operand::NOP;
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format& format, void*, const void*, const void*) {
        if (format.operand[0].type == Format::Operand::REL)
            EIP += format.operand[0].displacement;
        else
            EIP = *(uint32_t*)format.operand[0].memory;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::LAHF(Format& format, const uint8_t* opcode)
{
    format.instruction = "LAHF";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        AH = FLAGS;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::LEA(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "LEA", 1, 0, OPERAND_SIZE | DIRECTION);

    BEGIN_OPERATION() {
        DEST = decltype(DEST)(format.operand[1].address);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::LEAVE(Format& format, const uint8_t* opcode)
{
    format.instruction = "LEAVE";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        ESP = EBP;
        EBP = Pop32();
    };
}
//------------------------------------------------------------------------------
void x86_instruction::LOOP(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xE0:  Decode(format, opcode, "LOOPNZ", 1, 8, OPERAND_SIZE | RELATIVE);    break;
    case 0xE1:  Decode(format, opcode, "LOOPZ", 1, 8, OPERAND_SIZE | RELATIVE);     break;
    case 0xE2:  Decode(format, opcode, "LOOP", 1, 8, OPERAND_SIZE | RELATIVE);      break;
    }
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format& format, void*, const void*, const void*) {
        uint32_t CountReg = (format.width == 16) ? CX : ECX;
        if (CountReg) {
            (format.width == 16) ? CX = CX - 1 : ECX = ECX - 1;
            bool BranchCond = true;
            switch (x86.opcode[0]) {
            case 0xE0:  BranchCond = (ZF == 0); break;
            case 0xE1:  BranchCond = (ZF != 0); break;
            }
            if (BranchCond) {
                EIP += format.operand[1].displacement;
            }
        }
    };
}
//------------------------------------------------------------------------------
void x86_instruction::MOV(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:  Decode(format, opcode, "MOV", 1, 0, opcode[0] & 0b11);  break;
    case 0xA0:
    case 0xA1:  Decode(format, opcode, "MOV", 1, 32, (opcode[0] & 0b01) | INDIRECT);
                format.operand[1] = format.operand[0];
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = REG(EAX);                      break;
    case 0xA2:
    case 0xA3:  Decode(format, opcode, "MOV", 1, 32, (opcode[0] & 0b01) | INDIRECT);
                format.operand[1].type = Format::Operand::REG;
                format.operand[1].base = REG(EAX);                      break;
    case 0xB0:
    case 0xB1:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB7:  Decode(format, opcode, "MOV", 0, 8);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111;             break;
    case 0xB8:
    case 0xB9:
    case 0xBA:
    case 0xBB:
    case 0xBC:
    case 0xBD:
    case 0xBE:
    case 0xBF:  Decode(format, opcode, "MOV", 0, -1, OPERAND_SIZE);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111;             break;
    case 0xC6:  Decode(format, opcode, "MOV", 1, 8);                    break;
    case 0xC7:  Decode(format, opcode, "MOV", 1, -1, OPERAND_SIZE);     break;
    }

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MOVSX(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xBE:
    case 0xBF:  Decode(format, opcode, "MOVSX", 2, 0, 1);   break;
    }

    BEGIN_OPERATION() {
        switch (x86.opcode[1]) {
        case 0xBE:
            switch (format.width) {
            case 16:    (int16_t&)DEST = (int8_t)SRC;   break;
            case 32:    (int32_t&)DEST = (int8_t)SRC;   break;
            }
            break;
        case 0xBF:      (int32_t&)DEST = (int16_t)SRC;  break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MOVZX(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0xB6:
    case 0xB7:  Decode(format, opcode, "MOVZX", 2, 0, 1);   break;
    }

    BEGIN_OPERATION() {
        switch (x86.opcode[1]) {
        case 0xB6:
            switch (format.width) {
            case 16:    (uint16_t&)DEST = (uint8_t)SRC;     break;
            case 32:    (uint32_t&)DEST = (uint8_t)SRC;     break;
            }
            break;
        case 0xB7:      (uint32_t&)DEST = (uint16_t)SRC;    break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::NOP(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x90:  Decode(format, opcode, "NOP");                          break;
    case 0x0F:  Decode(format, opcode, "NOP", 2, 0, opcode[1] & 0b11);  break;
    }
    format.operand[0].type = Format::Operand::NOP;
    format.operand[1].type = Format::Operand::NOP;
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {};
}
//------------------------------------------------------------------------------
void x86_instruction::POP(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x5E:
    case 0x5F:  Decode(format, opcode, "POP", 0, 0, OPERAND_SIZE);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111;         break;
    case 0x8F:  Decode(format, opcode, "POP", 1, 0, OPERAND_SIZE);  break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        DEST = Pop32();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::POPAD(Format& format, const uint8_t* opcode)
{
    format.instruction = "POPAD";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        EDI = Pop32();
        ESI = Pop32();
        EBP = Pop32();
        Pop32();
        EBX = Pop32();
        EDX = Pop32();
        ECX = Pop32();
        EAX = Pop32();
    };
}
//------------------------------------------------------------------------------
void x86_instruction::POPFD(Format& format, const uint8_t* opcode)
{
    format.instruction = "POPFD";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        EFLAGS = Pop32();
    };
}
//------------------------------------------------------------------------------
void x86_instruction::PUSH(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:  Decode(format, opcode, "PUSH", 0,  0, OPERAND_SIZE);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111;                         break;
    case 0x68:  Decode(format, opcode, "PUSH", 1, -1, OPERAND_SIZE | IMMEDIATE);    break;
    case 0x6A:  Decode(format, opcode, "PUSH", 1,  8, OPERAND_SIZE | IMMEDIATE);    break;
    case 0xFF:  Decode(format, opcode, "PUSH", 1,  0, OPERAND_SIZE);                break;
    }
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        Push32(DEST);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::PUSHAD(Format& format, const uint8_t* opcode)
{
    format.instruction = "PUSHAD";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        auto Temp = ESP;
        Push32(EAX);
        Push32(ECX);
        Push32(EDX);
        Push32(EBX);
        Push32(Temp);
        Push32(EBP);
        Push32(ESI);
        Push32(EDI);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::PUSHFD(Format& format, const uint8_t* opcode)
{
    format.instruction = "PUSHFD";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        Push32(EFLAGS);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::REP(Format& format, const uint8_t* opcode)
{
    format.repeat = true;
}
//------------------------------------------------------------------------------
void x86_instruction::RET(Format& format, const uint8_t* opcode)
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
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format& format, void*, const void*, const void*) {
        EIP = Pop32();
        ESP = uint32_t(ESP + format.operand[0].displacement);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::SAHF(Format& format, const uint8_t* opcode)
{
    format.instruction = "SAHF";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        FLAGS = AH;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::SETcc(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x90:  Decode(format, opcode, "SETO", 2);  break;
    case 0x91:  Decode(format, opcode, "SETNO", 2); break;
    case 0x92:  Decode(format, opcode, "SETC", 2);  break;
    case 0x93:  Decode(format, opcode, "SETNC", 2); break;
    case 0x94:  Decode(format, opcode, "SETZ", 2);  break;
    case 0x95:  Decode(format, opcode, "SETNZ", 2); break;
    case 0x96:  Decode(format, opcode, "SETBE", 2); break;
    case 0x97:  Decode(format, opcode, "SETA", 2);  break;
    case 0x98:  Decode(format, opcode, "SETS", 2);  break;
    case 0x99:  Decode(format, opcode, "SETNS", 2); break;
    case 0x9A:  Decode(format, opcode, "SETPE", 2); break;
    case 0x9B:  Decode(format, opcode, "SETPO", 2); break;
    case 0x9C:  Decode(format, opcode, "SETL", 2);  break;
    case 0x9D:  Decode(format, opcode, "SETGE", 2); break;
    case 0x9E:  Decode(format, opcode, "SETLE", 2); break;
    case 0x9F:  Decode(format, opcode, "SETG", 2);  break;
    }
    format.width = 8;
    format.operand[1].type = Format::Operand::NOP;

    BEGIN_OPERATION() {
        switch (x86.opcode[1]) {
        case 0x90:  DEST = ((OF)          ) == 1;   break;  // SETO
        case 0x91:  DEST = ((OF)          ) == 0;   break;  // SETNO
        case 0x92:  DEST = ((CF)          ) == 1;   break;  // SETC
        case 0x93:  DEST = ((CF)          ) == 0;   break;  // SETNC
        case 0x94:  DEST = ((ZF)          ) == 1;   break;  // SETZ
        case 0x95:  DEST = ((ZF)          ) == 0;   break;  // SETNZ
        case 0x96:  DEST = ((CF | ZF)     ) == 1;   break;  // SETBE
        case 0x97:  DEST = ((CF | ZF)     ) == 0;   break;  // SETA
        case 0x98:  DEST = ((SF)          ) == 1;   break;  // SETS
        case 0x99:  DEST = ((SF)          ) == 0;   break;  // SETNS
        case 0x9A:  DEST = ((PF)          ) == 1;   break;  // SETPE
        case 0x9B:  DEST = ((PF)          ) == 0;   break;  // SETPO
        case 0x9C:  DEST = ((SF ^ OF)     ) == 1;   break;  // SETL
        case 0x9D:  DEST = ((SF ^ OF)     ) == 0;   break;  // SETGE
        case 0x9E:  DEST = ((SF ^ OF) | ZF) == 1;   break;  // SETLE
        case 0x9F:  DEST = ((SF ^ OF) | ZF) == 0;   break;  // SETG
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::STC(Format& format, const uint8_t* opcode)
{
    format.instruction = "STC";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        CF = 1;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::STD(Format& format, const uint8_t* opcode)
{
    format.instruction = "STD";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        DF = 1;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::UD(Format& format, const uint8_t* opcode)
{
    switch (opcode[1]) {
    case 0x0B:  format.instruction = "UD2"; break;
    case 0xB9:  format.instruction = "UD1"; break;
    case 0xFF:  format.instruction = "UD0"; break;
    }
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {};
}
//------------------------------------------------------------------------------
void x86_instruction::XCHG(Format& format, const uint8_t* opcode)
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
        Decode(format, opcode, "XCHG", 1, 0, opcode[0] & 0b01);
        break;
    }

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        DEST = SRC;
        SRC = TEMP;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::XLAT(Format& format, const uint8_t* opcode)
{
    format.instruction = "XLAT";
    format.operation = [](x86_instruction& x86, x87_instruction&, const Format&, void*, const void*, const void*) {
        AL = *(x86.memory_address + EBX + AL);
    };
}
//------------------------------------------------------------------------------
