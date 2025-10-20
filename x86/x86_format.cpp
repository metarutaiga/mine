#include "x86_format.h"
#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x87_register.h"
#include "x87_register.inl"
#include "mmx_register.h"
#include "mmx_register.inl"
#include "sse_register.h"
#include "sse_register.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
const char* const x86_format::REG8HL[8] = { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH" };
const char* const x86_format::REG8[8] = { "AL", "CL", "DL", "BL", "SPL", "BPL", "SIL", "DIL" };
const char* const x86_format::REG16[8] = { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI" };
const char* const x86_format::REG32[8] = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };
const char* const x86_format::REG64[8] = { "RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI" };
//------------------------------------------------------------------------------
void x86_format::Decode(Format& format, const uint8_t* opcode, const char* instruction, int offset, int flags)
{
    format.instruction = instruction;
    switch (flags & (X87_REGISTER | MMX_REGISTER | SSE_REGISTER)) {
    default:            format.width = (flags & OPERAND_SIZE) ? format.width : 8;   break;
    case X87_REGISTER:  format.width = 64;                                          break;
    case MMX_REGISTER:  format.width = 64;                                          break;
    case SSE_REGISTER:  format.width = 128;                                         break;
    }
    format.length = offset;

    int MODRM = (flags & DIRECTION) ? 1 : 0;
    int OPREG = (flags & DIRECTION) ? 0 : 1;
    switch (flags & (IMMEDIATE | INDIRECT | RELATIVE)) {
    default:
        format.length += 1;
        if (offset) {
//          int MOD     = (opcode[offset + 0] >> 6) & 0b11;
            int REG     = (opcode[offset + 0] >> 3) & 0b111;
            int RM      = (opcode[offset + 0] >> 0) & 0b111;
            int SS      = (opcode[offset + 1] >> 6) & 0b11;
            int INDEX   = (opcode[offset + 1] >> 3) & 0b111;
            int BASE    = (opcode[offset + 1] >> 0) & 0b111;
            switch (opcode[offset] & 0b11000111) {
            case 0b00000100:
                format.length += 1;
                format.operand[MODRM].type = Format::Operand::ADR;
                format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
                format.operand[MODRM].index = (INDEX == 0b100) ? 0 : INDEX;
//              format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
                format.operand[MODRM].base = BASE;
//              format.operand[MODRM].displacement = 0;
                if (BASE == 0b101) {
                    format.length += 4;
                    format.operand[MODRM].base = -1;
                    format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
                }
                break;
            case 0b00000101:
                format.length += 4;
                format.operand[MODRM].type = Format::Operand::ADR;
//              format.operand[MODRM].scale = 0;
//              format.operand[MODRM].index = -1;
                format.operand[MODRM].base = -1;
                format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
                break;
            case 0b01000100:
                format.length += 2;
                format.operand[MODRM].type = Format::Operand::ADR;
                format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
                format.operand[MODRM].index = (INDEX == 0b100) ? 0 : INDEX;
//              format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
                format.operand[MODRM].base = BASE;
                format.operand[MODRM].displacement = IMM8(opcode, format.length - 1);
                break;
            case 0b10000100:
                format.length += 5;
                format.operand[MODRM].type = Format::Operand::ADR;
                format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
                format.operand[MODRM].index = (INDEX == 0b100) ? 0 : INDEX;
//              format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
                format.operand[MODRM].base = BASE;
                format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
                break;
            case 0b00000000:
            case 0b00000001:
            case 0b00000010:
            case 0b00000011:
//          case 0b00000100:
//          case 0b00000101:
            case 0b00000110:
            case 0b00000111:
                format.operand[MODRM].type = Format::Operand::ADR;
//              format.operand[MODRM].scale = 0;
//              format.operand[MODRM].index = -1;
                format.operand[MODRM].base = RM;
//              format.operand[MODRM].displacement = 0;
                break;
            case 0b01000000:
            case 0b01000001:
            case 0b01000010:
            case 0b01000011:
//          case 0b01000100:
            case 0b01000101:
            case 0b01000110:
            case 0b01000111:
                format.length += 1;
                format.operand[MODRM].type = Format::Operand::ADR;
//              format.operand[MODRM].scale = 0;
//              format.operand[MODRM].index = -1;
                format.operand[MODRM].base = RM;
                format.operand[MODRM].displacement = IMM8(opcode, format.length - 1);
                break;
            case 0b10000000:
            case 0b10000001:
            case 0b10000010:
            case 0b10000011:
//          case 0b10000100:
            case 0b10000101:
            case 0b10000110:
            case 0b10000111:
                format.length += 4;
                format.operand[MODRM].type = Format::Operand::ADR;
//              format.operand[MODRM].scale = 0;
//              format.operand[MODRM].index = -1;
                format.operand[MODRM].base = RM;
                format.operand[MODRM].displacement = IMM32(opcode, format.length - 4);
                break;
            case 0b11000000:
            case 0b11000001:
            case 0b11000010:
            case 0b11000011:
            case 0b11000100:
            case 0b11000101:
            case 0b11000110:
            case 0b11000111:
                switch (flags & (X87_REGISTER | MMX_REGISTER | SSE_REGISTER)) {
                default:            format.operand[MODRM].type = Format::Operand::REG;  break;
                case X87_REGISTER:  format.operand[MODRM].type = Format::Operand::X87;  break;
                case MMX_REGISTER:  format.operand[MODRM].type = Format::Operand::MMX;  break;
                case SSE_REGISTER:  format.operand[MODRM].type = Format::Operand::SSE;  break;
                }
                format.operand[MODRM].base = RM;
                break;
            }
            format.operand[OPREG].base = REG;
        }
        else {
            switch (flags & (X87_REGISTER | MMX_REGISTER | SSE_REGISTER)) {
            default:            format.operand[MODRM].type = Format::Operand::REG;  break;
            case X87_REGISTER:  format.operand[MODRM].type = Format::Operand::X87;  break;
            case MMX_REGISTER:  format.operand[MODRM].type = Format::Operand::MMX;  break;
            case SSE_REGISTER:  format.operand[MODRM].type = Format::Operand::SSE;  break;
            }
//          format.operand[MODRM].base = 0;
//          format.operand[OPREG].base = 0;
        }
        switch (flags & (X87_REGISTER | MMX_REGISTER | SSE_REGISTER)) {
        default:            format.operand[OPREG].type = Format::Operand::REG;  break;
        case X87_REGISTER:  format.operand[OPREG].type = Format::Operand::X87;  break;
        case MMX_REGISTER:  format.operand[OPREG].type = Format::Operand::MMX;  break;
        case SSE_REGISTER:  format.operand[OPREG].type = Format::Operand::SSE;  break;
        }
        if (flags & THREE_OPERAND) {
            OPREG = 2;
            format.operand_count = 3;
        }
        break;
    case IMMEDIATE:
    case INDIRECT:
    case RELATIVE:
        OPREG = 0;
//      format.operand[OPREG].scale = 0;
//      format.operand[OPREG].index = 0;
        format.operand[OPREG].base = -1;
        break;
    }

    int immediate_size = 0;
    switch (flags & (IMM_SIZE | IMM_8BIT | IMM_32BIT)) {
    case IMM_SIZE:  immediate_size = format.width;  break;
    case IMM_8BIT:  immediate_size = 8;             break;
    case IMM_32BIT: immediate_size = 32;            break;
    }

    if (immediate_size) {
        switch (flags & (IMMEDIATE | INDIRECT | RELATIVE)) {
        default:
        case IMMEDIATE: format.operand[OPREG].type = Format::Operand::IMM;  break;
        case INDIRECT:  format.operand[OPREG].type = Format::Operand::ADR;  break;
        case RELATIVE:  format.operand[OPREG].type = Format::Operand::REL;  break;
        }
        switch (immediate_size) {
        case 8:
            format.length += 1;
            format.operand[OPREG].displacement = IMM8(opcode, format.length - 1);
            break;
        case 16:
            format.length += 2;
            format.operand[OPREG].displacement = IMM16(opcode, format.length - 2);
            break;
        case 32:
            format.length += 4;
            format.operand[OPREG].displacement = IMM32(opcode, format.length - 4);
            break;
        }
    }
}
//------------------------------------------------------------------------------
std::string x86_format::Disasm(const Format& format, x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse)
{
    auto hex = [](auto imm, bool sign) {
        char temp[64];
        if (sign) {
#if HAVE_X64
            auto value = uint64_t(std::abs(int64_t(imm)));
            if (value > 0x9FFFFFFF)  snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "", 16, value);
#else
            auto value = uint64_t(abs(int32_t(imm)));
            if (0) {}
#endif
            else if (value > 0x9FFF) snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  8, value);
            else if (value > 0x9F)   snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  4, value);
            else if (value > 0x9)    snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  2, value);
            else                     snprintf(temp, 64, "%s%0*llX", imm < 0 ? "-" : "",  1, value);
        }
        else {
#if HAVE_X64
            auto value = uint64_t(uint64_t(imm));
            if (value > 0x9FFFFFFF)  snprintf(temp, 64, "%s%0*llX", "", 16, value);
#else
            auto value = uint64_t(uint32_t(imm));
            if (0) {}
#endif
            else if (value > 0x9FFF) snprintf(temp, 64, "%s%0*llX", "",  8, value);
            else if (value > 0x9F)   snprintf(temp, 64, "%s%0*llX", "",  4, value);
            else if (value > 0x9)    snprintf(temp, 64, "%s%0*llX", "",  2, value);
            else                     snprintf(temp, 64, "%s%0*llX", "",  1, value);
        }
        return std::string(temp);
    };

    std::string disasm;
    if (format.string) {
        switch (format.prefix) {
        case 0xF2:
            disasm += "REPNE";
            disasm += ' ';
            break;
        case 0xF3:
            disasm += "REPE";
            disasm += ' ';
        }
    }
    disasm += format.instruction;

    size_t offset = disasm.size();
    for (int i = 0, count = format.operand_count; i < count; ++i) {
        const auto& operand = format.operand[i];
        if (operand.flags & Format::Operand::HIDE)
            continue;
        if (disasm.size() > offset)
            disasm += ',';
        disasm += ' ';

        int width = format.width;
        switch (operand.flags & (Format::Operand::BIT8 | Format::Operand::BIT16 | Format::Operand::BIT32)) {
        case Format::Operand::BIT8:  width = 8;  break;
        case Format::Operand::BIT16: width = 16; break;
        case Format::Operand::BIT32: width = 32; break;
        }

        switch (operand.type) {
        case Format::Operand::ADR:
            switch (width) {
            case 8:   disasm += "BYTE PTR";    break;
            case 16:  disasm += "WORD PTR";    break;
            case 32:  disasm += "DWORD PTR";   break;
            case 64:  disasm += "QWORD PTR";   break;
            case 80:  disasm += "TBYTE PTR";   break;
            case 128: disasm += "XMMWORD PTR"; break;
            }
            disasm += ' ';
            if (format.segment[0]) {
                disasm += format.segment;
                disasm += ':';
            }
            disasm += '[';
            if (operand.scale > 0) {
                switch (format.address) {
                case 16: disasm += REG16[operand.index]; break;
                case 32: disasm += REG32[operand.index]; break;
#if HAVE_X64
                case 64: disasm += REG64[operand.index]; break;
#endif
                }
                if (operand.scale > 1) {
                    disasm += '*';
                    disasm += std::to_string(operand.scale);
                }
            }
            if (operand.base >= 0) {
                if (disasm.back() != '[')
                    disasm += '+';
                switch (format.address) {
                case 16: disasm += REG16[operand.base]; break;
                case 32: disasm += REG32[operand.base]; break;
#if HAVE_X64
                case 64: disasm += REG64[operand.base]; break;
#endif
                }
            }
            if (operand.displacement) {
                if (disasm.back() != '[' && operand.displacement >= 0)
                    disasm += '+';
                disasm += hex(operand.displacement, true);
            }
            if (disasm.back() == '[')
                disasm += '0';
            disasm += ']';
            continue;
        case Format::Operand::IMM:
            switch (width) {
            case 8:  disasm += hex(uint8_t(operand.displacement), false);  break;
            case 16: disasm += hex(uint16_t(operand.displacement), false); break;
            case 32: disasm += hex(uint32_t(operand.displacement), false); break;
            }
            continue;
        case Format::Operand::REG:
#if HAVE_X64
            if (width == 8 && format.address > 32) width = -8;
#endif
            switch (width) {
            case 8:  disasm += REG8HL[operand.base]; break;
#if HAVE_X64
            case -8: disasm += REG8[operand.base];   break;
#endif
            case 16: disasm += REG16[operand.base];  break;
            case 32: disasm += REG32[operand.base];  break;
#if HAVE_X64
            case 64: disasm += REG64[operand.base];  break;
#endif
            }
            continue;
        case Format::Operand::REL:
#if HAVE_X64
            disasm += hex(x86.ip.q + operand.displacement, false);
#else
            disasm += hex(x86.ip.d + operand.displacement, false);
#endif
            continue;
        case Format::Operand::X87:
            if (operand.base >= 0) {
                disasm += "ST";
                disasm += '(';
                disasm += '0' + operand.base;
                disasm += ')';
                continue;
            }
            break;
        case Format::Operand::MMX:
            disasm += "MM";
            disasm += '0' + operand.base;
            continue;
        case Format::Operand::SSE:
            disasm += "XMM";
            disasm += '0' + operand.base;
            continue;
        default:
            break;
        }
        if (disasm.size() > offset)
            disasm.pop_back();
        if (disasm.size() > offset)
            disasm.pop_back();
    }

    return disasm;
}
//------------------------------------------------------------------------------
void x86_format::Fixup(Format& format, x86_register& x86, x87_register& x87, mmx_register& mmx, sse_register& sse)
{
    for (int i = 0, count = format.operand_count; i < count; ++i) {
        auto& operand = format.operand[i];

        int width = format.width;
        switch (operand.flags & (Format::Operand::BIT8 | Format::Operand::BIT16 | Format::Operand::BIT32)) {
        case Format::Operand::BIT8:  width = 8;  break;
        case Format::Operand::BIT16: width = 16; break;
        case Format::Operand::BIT32: width = 32; break;
        }

        switch (operand.type) {
        case Format::Operand::IMM:
            operand.address = operand.displacement;
            operand.memory = (uint8_t*)&operand.address;
            break;
        case Format::Operand::REL:
            operand.address = operand.displacement;
#if HAVE_X64
            operand.address += x86.ip.q;
            switch (format.address) {
            case 16: operand.address = uint16_t(operand.address); break;
            case 32: operand.address = uint32_t(operand.address); break;
            }
#else
            operand.address += x86.ip.d;
#endif
            operand.memory = (uint8_t*)&operand.address;
            break;
        case Format::Operand::ADR:
            operand.address = operand.displacement;
            if (operand.index >= 0) {
                operand.address += x86.regs[operand.index].d * operand.scale;
            }
            if (operand.base >= 0) {
                operand.address += x86.regs[operand.base].d;
            }
#if HAVE_X64
            switch (format.address) {
            case 16: operand.address = uint16_t(operand.address); break;
            case 32: operand.address = uint32_t(operand.address); break;
            }
#endif
            operand.memory = x86.memory_address + operand.address;
            break;
        case Format::Operand::REG:
#if HAVE_X64
            if (width == 8 && format.address > 32) width = -8;
#endif
            if (width == 8 && operand.base >= 4) {
                operand.memory = &x86.regs[operand.base - 4].h;
                break;
            }
            operand.memory = &x86.regs[operand.base].l;
            break;
        case Format::Operand::X87:
            operand.memory = (uint8_t*)&ST(operand.base);
            break;
        case Format::Operand::MMX:
            operand.memory = (uint8_t*)&MM(operand.base);
            break;
        case Format::Operand::SSE:
            operand.memory = (uint8_t*)&XMM(operand.base);
            break;
        default:
            break;
        }
    }
}
//------------------------------------------------------------------------------
