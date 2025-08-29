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
#if HAVE_X64
        auto value = std::abs((int64_t)imm);
        if (value > 0xFFFFFFFF)  snprintf(temp, 64, "%s%0*llX",                 "", 16, imm);
        else if (value > 0xFFFF) snprintf(temp, 64, "%s%0*llX",                 "",  8, imm & 0xFFFFFFFF);
        else if (value > 0xFF)   snprintf(temp, 64, "%s%0*llX",                 "",  4, imm & 0xFFFF);
        else if (value > 0x9)    snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  2, value);
        else                     snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  1, value);
#else
        auto value = std::abs((int32_t)imm);
        if (value > 0xFFFF)    snprintf(temp, 64, "%s%0*X",                 "", 8, imm);
        else if (value > 0xFF) snprintf(temp, 64, "%s%0*X",                 "", 4, imm & 0xFFFF);
        else if (value > 0x9)  snprintf(temp, 64, "%s%0*X", imm < 0 ? "-" : "", 2, value);
        else                   snprintf(temp, 64, "%s%0*X", imm < 0 ? "-" : "", 1, value);
#endif
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
            case 8:  disasm += "BYTE PTR";  break;
            case 16: disasm += "WORD PTR";  break;
            case 32: disasm += "DWORD PTR"; break;
            case 64: disasm += "QWORD PTR"; break;
            case 80: disasm += "TBYTE PTR"; break;
            }
            disasm += ' ';
            if (format.segment[0]) {
                disasm += format.segment;
                disasm += ':';
            }
            disasm += '[';
            if (format.operand[i].scale > 0) {
                switch (format.address) {
                case 16: disasm += REG16[format.operand[i].index]; break;
                case 32: disasm += REG32[format.operand[i].index]; break;
#if HAVE_X64
                case 64: disasm += REG64[format.operand[i].index]; break;
#endif
                }
                if (format.operand[i].scale > 1) {
                    disasm += '*';
                    disasm += std::to_string(format.operand[i].scale);
                }
            }
            if (format.operand[i].base >= 0) {
                if (disasm.back() != '[')
                    disasm += '+';
                switch (format.address) {
                case 16: disasm += REG16[format.operand[i].base]; break;
                case 32: disasm += REG32[format.operand[i].base]; break;
#if HAVE_X64
                case 64: disasm += REG64[format.operand[i].base]; break;
#endif
                }
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
            if (format.floating) {
                disasm += "ST";
                disasm += '(';
                disasm += '0' + format.operand[i].base;
                disasm += ')';
                break;
            }
            switch (format.width) {
            case 8:  disasm += REG8[format.operand[i].base];   break;
            case 16: disasm += REG16[format.operand[i].base];  break;
            case 32: disasm += REG32[format.operand[i].base];  break;
#if HAVE_X64
            case 64: disasm += REG64[format.operand[i].base];  break;
#endif
            }
            break;
        case Format::Operand::REL:
#if HAVE_X64
            disasm += hex(x86.ip.q + format.operand[i].displacement);
#else
            disasm += hex(x86.ip.d + format.operand[i].displacement);
#endif
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
            switch (format.address) {
            case 16: format.operand[i].address = uint16_t(format.operand[i].address); break;
#if HAVE_X64
            case 32: format.operand[i].address = uint32_t(format.operand[i].address); break;
#endif
            }
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
#if HAVE_X64
            format.operand[i].address = x86.ip.q;
#else
            format.operand[i].address = x86.ip.d;
#endif
            format.operand[i].address += format.operand[i].displacement;
            switch (format.address) {
            case 16: format.operand[i].address = uint16_t(format.operand[i].address); break;
#if HAVE_X64
            case 32: format.operand[i].address = uint32_t(format.operand[i].address); break;
#endif
            }
            format.operand[i].memory = (uint8_t*)&format.operand[i].address;
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

    OPERATION() {};
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
void x86_instruction::REP(Format& format, const uint8_t* opcode)
{
    format.repeat = true;
}
//------------------------------------------------------------------------------
void x86_instruction::WAIT(Format& format, const uint8_t* opcode)
{
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

    OPERATION() {
        Push32(EIP);
        EIP = *(uint32_t*)format.operand[0].memory;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CDQ(Format& format, const uint8_t* opcode)
{
    switch (format.width) {
    case 16:
        format.instruction = "CWD";

        OPERATION() {
            DX = (int16_t)AX < 0 ? 0xFFFF : 0x0000;
        };
        break;
    case 32:
        format.instruction = "CDQ";

        OPERATION() {
            EDX = (int32_t)EAX < 0 ? 0xFFFFFFFF : 0x00000000;
        };
        break;
#if HAVE_X64
    case 64:
        format.instruction = "CQO";

        OPERATION() {
            RDX = (int64_t)RAX < 0 ? 0xFFFFFFFFFFFFFFFFull : 0x0000000000000000ull;
        };
        break;
#endif
    }
}
//------------------------------------------------------------------------------
void x86_instruction::CLC(Format& format, const uint8_t* opcode)
{
    format.instruction = "CLC";

    OPERATION() {
        CF = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CLD(Format& format, const uint8_t* opcode)
{
    format.instruction = "CLD";

    OPERATION() {
        DF = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CMC(Format& format, const uint8_t* opcode)
{
    format.instruction = "CMC";

    OPERATION() {
        CF = !CF;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CWDE(Format& format, const uint8_t* opcode)
{
    switch (format.width) {
    case 16:
        format.instruction = "CBW";

        OPERATION() {
            AX = (int8_t)AL;
        };
        break;
    case 32:
        format.instruction = "CWDE";

        OPERATION() {
            EAX = (int16_t)AX;
        };
        break;
#if HAVE_X64
    case 64:
        format.instruction = "CDQE";

        OPERATION() {
            RAX = (int32_t)EAX;
        };
        break;
#endif
    }
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

    OPERATION() {
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
    case 0x70:      Decode(format, opcode, "JO",  1, 8, RELATIVE);  break;
    case 0x71:      Decode(format, opcode, "JNO", 1, 8, RELATIVE);  break;
    case 0x72:      Decode(format, opcode, "JC",  1, 8, RELATIVE);  break;
    case 0x73:      Decode(format, opcode, "JNC", 1, 8, RELATIVE);  break;
    case 0x74:      Decode(format, opcode, "JZ",  1, 8, RELATIVE);  break;
    case 0x75:      Decode(format, opcode, "JNZ", 1, 8, RELATIVE);  break;
    case 0x76:      Decode(format, opcode, "JBE", 1, 8, RELATIVE);  break;
    case 0x77:      Decode(format, opcode, "JA",  1, 8, RELATIVE);  break;
    case 0x78:      Decode(format, opcode, "JS",  1, 8, RELATIVE);  break;
    case 0x79:      Decode(format, opcode, "JNS", 1, 8, RELATIVE);  break;
    case 0x7A:      Decode(format, opcode, "JPE", 1, 8, RELATIVE);  break;
    case 0x7B:      Decode(format, opcode, "JPO", 1, 8, RELATIVE);  break;
    case 0x7C:      Decode(format, opcode, "JL",  1, 8, RELATIVE);  break;
    case 0x7D:      Decode(format, opcode, "JGE", 1, 8, RELATIVE);  break;
    case 0x7E:      Decode(format, opcode, "JLE", 1, 8, RELATIVE);  break;
    case 0x7F:      Decode(format, opcode, "JG",  1, 8, RELATIVE);  break;
    case 0xE3:      Decode(format, opcode, (format.address == 16) ? "JCXZ" : "JECXZ", 1, 8, RELATIVE); break;
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

    switch (opcode[0]) {
    case 0x70:      OPERATION() { if (((OF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JO
    case 0x71:      OPERATION() { if (((OF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNO
    case 0x72:      OPERATION() { if (((CF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JC
    case 0x73:      OPERATION() { if (((CF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNC
    case 0x74:      OPERATION() { if (((ZF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JZ
    case 0x75:      OPERATION() { if (((ZF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNZ
    case 0x76:      OPERATION() { if (((CF | ZF)     ) == 1) EIP += format.operand[0].displacement; };  break;  // JBE
    case 0x77:      OPERATION() { if (((CF | ZF)     ) == 0) EIP += format.operand[0].displacement; };  break;  // JA
    case 0x78:      OPERATION() { if (((SF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JS
    case 0x79:      OPERATION() { if (((SF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNS
    case 0x7A:      OPERATION() { if (((PF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JPE
    case 0x7B:      OPERATION() { if (((PF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JPO
    case 0x7C:      OPERATION() { if (((SF ^ OF)     ) == 1) EIP += format.operand[0].displacement; };  break;  // JL
    case 0x7D:      OPERATION() { if (((SF ^ OF)     ) == 0) EIP += format.operand[0].displacement; };  break;  // JGE
    case 0x7E:      OPERATION() { if (((SF ^ OF) | ZF) == 1) EIP += format.operand[0].displacement; };  break;  // JLE
    case 0x7F:      OPERATION() { if (((SF ^ OF) | ZF) == 0) EIP += format.operand[0].displacement; };  break;  // JG
    case 0xE3:      (format.address == 16) ?
                    OPERATION() { if (((CX)          ) == 0) EIP += format.operand[0].displacement; } :         // JCXZ
                    OPERATION() { if (((ECX)         ) == 0) EIP += format.operand[0].displacement; };  break;  // JECXZ
    case 0x0F:
        switch (opcode[1]) {
        case 0x80:  OPERATION() { if (((OF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JO
        case 0x81:  OPERATION() { if (((OF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNO
        case 0x82:  OPERATION() { if (((CF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JC
        case 0x83:  OPERATION() { if (((CF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNC
        case 0x84:  OPERATION() { if (((ZF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JZ
        case 0x85:  OPERATION() { if (((ZF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNZ
        case 0x86:  OPERATION() { if (((CF | ZF)     ) == 1) EIP += format.operand[0].displacement; };  break;  // JBE
        case 0x87:  OPERATION() { if (((CF | ZF)     ) == 0) EIP += format.operand[0].displacement; };  break;  // JA
        case 0x88:  OPERATION() { if (((SF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JS
        case 0x89:  OPERATION() { if (((SF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JNS
        case 0x8A:  OPERATION() { if (((PF)          ) == 1) EIP += format.operand[0].displacement; };  break;  // JPE
        case 0x8B:  OPERATION() { if (((PF)          ) == 0) EIP += format.operand[0].displacement; };  break;  // JPO
        case 0x8C:  OPERATION() { if (((SF ^ OF)     ) == 1) EIP += format.operand[0].displacement; };  break;  // JL
        case 0x8D:  OPERATION() { if (((SF ^ OF)     ) == 0) EIP += format.operand[0].displacement; };  break;  // JGE
        case 0x8E:  OPERATION() { if (((SF ^ OF) | ZF) == 1) EIP += format.operand[0].displacement; };  break;  // JLE
        case 0x8F:  OPERATION() { if (((SF ^ OF) | ZF) == 0) EIP += format.operand[0].displacement; };  break;  // JG
        }
        break;
    }
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

    OPERATION() {
        EIP = *(uint32_t*)format.operand[0].memory;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::LAHF(Format& format, const uint8_t* opcode)
{
    format.instruction = "LAHF";

    OPERATION() {
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

    OPERATION() {
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

    OPERATION() {
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
    Decode(format, opcode, "MOVSX", 2, 0, DIRECTION | OPERAND_SIZE);

    switch (opcode[1]) {
    case 0xBE:  BEGIN_OPERATION() { DEST = (int8_t)SRC;  } END_OPERATION;   break;
    case 0xBF:  BEGIN_OPERATION() { DEST = (int16_t)SRC; } END_OPERATION;   break;
    }
}
//------------------------------------------------------------------------------
void x86_instruction::MOVZX(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVZX", 2, 0, DIRECTION | OPERAND_SIZE);

    switch (opcode[1]) {
    case 0xB6:  BEGIN_OPERATION() { DEST = (uint8_t)SRC;  } END_OPERATION;  break;
    case 0xB7:  BEGIN_OPERATION() { DEST = (uint16_t)SRC; } END_OPERATION;  break;
    }
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

    OPERATION() {};
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

    OPERATION() {
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

    OPERATION() {
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

    OPERATION() {
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

    OPERATION() {
        Push32(EFLAGS);
    };
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

    OPERATION() {
        EIP = Pop32();
        ESP = uint32_t(ESP + format.operand[0].displacement);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::SAHF(Format& format, const uint8_t* opcode)
{
    format.instruction = "SAHF";

    OPERATION() {
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

    switch (opcode[1]) {
    case 0x90:  BEGIN_OPERATION() { DEST = ((OF)          ) == 1; } END_OPERATION;  break;  // SETO
    case 0x91:  BEGIN_OPERATION() { DEST = ((OF)          ) == 0; } END_OPERATION;  break;  // SETNO
    case 0x92:  BEGIN_OPERATION() { DEST = ((CF)          ) == 1; } END_OPERATION;  break;  // SETC
    case 0x93:  BEGIN_OPERATION() { DEST = ((CF)          ) == 0; } END_OPERATION;  break;  // SETNC
    case 0x94:  BEGIN_OPERATION() { DEST = ((ZF)          ) == 1; } END_OPERATION;  break;  // SETZ
    case 0x95:  BEGIN_OPERATION() { DEST = ((ZF)          ) == 0; } END_OPERATION;  break;  // SETNZ
    case 0x96:  BEGIN_OPERATION() { DEST = ((CF | ZF)     ) == 1; } END_OPERATION;  break;  // SETBE
    case 0x97:  BEGIN_OPERATION() { DEST = ((CF | ZF)     ) == 0; } END_OPERATION;  break;  // SETA
    case 0x98:  BEGIN_OPERATION() { DEST = ((SF)          ) == 1; } END_OPERATION;  break;  // SETS
    case 0x99:  BEGIN_OPERATION() { DEST = ((SF)          ) == 0; } END_OPERATION;  break;  // SETNS
    case 0x9A:  BEGIN_OPERATION() { DEST = ((PF)          ) == 1; } END_OPERATION;  break;  // SETPE
    case 0x9B:  BEGIN_OPERATION() { DEST = ((PF)          ) == 0; } END_OPERATION;  break;  // SETPO
    case 0x9C:  BEGIN_OPERATION() { DEST = ((SF ^ OF)     ) == 1; } END_OPERATION;  break;  // SETL
    case 0x9D:  BEGIN_OPERATION() { DEST = ((SF ^ OF)     ) == 0; } END_OPERATION;  break;  // SETGE
    case 0x9E:  BEGIN_OPERATION() { DEST = ((SF ^ OF) | ZF) == 1; } END_OPERATION;  break;  // SETLE
    case 0x9F:  BEGIN_OPERATION() { DEST = ((SF ^ OF) | ZF) == 0; } END_OPERATION;  break;  // SETG
    }
}
//------------------------------------------------------------------------------
void x86_instruction::STC(Format& format, const uint8_t* opcode)
{
    format.instruction = "STC";

    OPERATION() {
        CF = 1;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::STD(Format& format, const uint8_t* opcode)
{
    format.instruction = "STD";

    OPERATION() {
        DF = 1;
    };
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
        auto& SRC = *(std::remove_reference_t<decltype(DEST)>*)format.operand[1].memory;
        auto TEMP = DEST;
        DEST = SRC;
        SRC = TEMP;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::XLAT(Format& format, const uint8_t* opcode)
{
    format.instruction = "XLAT";

    OPERATION() {
        AL = *(x86.memory_address + EBX + AL);
    };
}
//------------------------------------------------------------------------------
