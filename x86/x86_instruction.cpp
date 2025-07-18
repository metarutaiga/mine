#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
static const char* const REG8[8] = { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH" };
static const char* const REG16[8] = { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI" };
static const char* const REG32[8] = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };
static const char* const REG64[8] = { "RAX", "RCX", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI" };
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::Decode(Format& format, int offset, const char* instruction, int direction, int have_operand_size, int immediate_size)
{
    int OPREG = (direction == 0) ? 1 : 0;
    int MODRM = (direction == 0) ? 0 : 1;

    format.size = (have_operand_size == 0) ? 8 : operand_size;
    format.length = offset + 1;
    format.instruction = instruction;
    if (offset) {
        int MOD = (opcode[offset] >> 6) & 0b11;
        int REG = (opcode[offset] >> 3) & 0b111;
        int RM = (opcode[offset] >> 0) & 0b111;

        int SS = (opcode[offset + 1] >> 6) & 0b11;
        int INDEX = (opcode[offset + 1] >> 3) & 0b111;
        int BASE = (opcode[offset + 1] >> 0) & 0b111;

        switch (opcode[offset] & 0b11111000) {
        case 0b00100000:
            format.length += 1;
            format.operand[MODRM].type = Format::Operand::ADR;
            format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.operand[MODRM].base = (BASE == 0b101) ? -1 : BASE;
            format.operand[MODRM].displacement = 0;
            break;
        case 0b01100000:
            format.length += 2;
            format.operand[MODRM].type = Format::Operand::ADR;
            format.operand[MODRM].scale = (INDEX == 0b100) ? 0 : (1 << SS);
            format.operand[MODRM].index = (INDEX == 0b100) ? -1 : INDEX;
            format.operand[MODRM].base = BASE;
            format.operand[MODRM].displacement = IMM8(opcode, format.length - 1);
            break;
        case 0b10100000:
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
    }
    else {
        format.operand[MODRM].type = Format::Operand::REG;
        format.operand[MODRM].base = 0;
        format.operand[OPREG].type = Format::Operand::REG;
        format.operand[OPREG].base = 0;
    }

    if (immediate_size) {
        if (immediate_size < 0)
            immediate_size = format.size;
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
std::string x86_instruction::Disasm(const Format& format, int operand)
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
    for (int i = 0; i < operand; ++i) {
        if (i) disasm += ',';
        disasm += ' ';
        switch (format.operand[i].type) {
        case Format::Operand::ADR:
            switch (format.size) {
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
            switch (format.size) {
            case 8:     disasm += REG8[format.operand[i].base];   break;
            case 16:    disasm += REG16[format.operand[i].base];  break;
            case 32:    disasm += REG32[format.operand[i].base];  break;
            case 64:    disasm += REG64[format.operand[i].base];  break;
            }
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
    for (int i = 0; i < 2; ++i) {
        switch (format.operand[i].type) {
        case Format::Operand::ADR:
            format.operand[i].address = 0;
            format.operand[i].address += regs[format.operand[i].index].d * format.operand[i].scale;
            format.operand[i].address += regs[format.operand[i].base].d;
            format.operand[i].address += format.operand[i].displacement;
            format.operand[i].memory = memory + format.operand[i].address;
            break;
        case Format::Operand::IMM:
            format.operand[i].memory = nullptr;
            format.operand[i].memory += format.operand[i].displacement;
            break;
        case Format::Operand::REG:
            if (format.size != 8 || format.operand[i].base < 4) {
                format.operand[i].memory = &regs[format.operand[i].base].l;
            }
            else {
                format.operand[i].memory = &regs[format.operand[i].base - 4].h;
            }
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
x86_format::Format x86_instruction::_()
{
    Format format;
    format.instruction = "UNKNOWN";

    BEGIN_OPERATION() {
    } END_OPERATION;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CS()
{
    return Format();
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SS()
{
    return Format();
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::DS()
{
    return Format();
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::ES()
{
    return Format();
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::FS()
{
    return Format();
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::GS()
{
    return Format();
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::ADC()
{
    Format format;
    switch (opcode[0]) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:  Decode(format, 1, "ADC", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x14:
    case 0x15:  Decode(format, 0, "ADC",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "ADC",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "ADC",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + SRC + CF);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::ADD()
{
    Format format;
    switch (opcode[0]) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:  Decode(format, 1, "ADD", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x04:
    case 0x05:  Decode(format, 0, "ADD",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "ADD",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "ADD",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::AND()
{
    Format format;
    switch (opcode[0]) {
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:  Decode(format, 1, "AND", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x24:
    case 0x25:  Decode(format, 0, "AND",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "AND",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "AND",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST & SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BSF()
{
    Format format;
    Decode(format, 2, "BSF", 0, 1, 0);
    Fixup(format);

    BEGIN_OPERATION() {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = __builtin_ctz(SRC);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BSR()
{
    Format format;
    Decode(format, 2, "BSR", 0, 1, 0);
    Fixup(format);

    BEGIN_OPERATION() {
        ZF = 1;
        if (SRC) {
            ZF = 0;
            DEST = __builtin_clz(SRC) ^ (sizeof(SRC) * 8 - 1);
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BT()
{
    Format format;
    switch (opcode[1]) {
    case 0xA3:  Decode(format, 2, "BT", 0, 1, 0);  break;
    case 0xBA:  Decode(format, 2, "BT", 0, 1, -1); break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BTC()
{
    Format format;
    switch (opcode[1]) {
    case 0xBA:  Decode(format, 2, "BTC", 0, 1, -1);    break;
    case 0xBB:  Decode(format, 2, "BTC", 0, 1, 0);     break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST ^ (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BTR()
{
    Format format;
    switch (opcode[1]) {
    case 0xB3:  Decode(format, 2, "BTR", 0, 1, 0);     break;
    case 0xBA:  Decode(format, 2, "BTR", 0, 1, -1);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST & ~(1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::BTS()
{
    Format format;
    switch (opcode[1]) {
    case 0xAB:  Decode(format, 2, "BTS", 0, 1, 0);     break;
    case 0xBA:  Decode(format, 2, "BTS", 0, 1, -1);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST & (1 << SRC)) ? 1 : 0;
        DEST = (DEST | (1 << SRC));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CALL()
{
    Format format;
    switch (opcode[0]) {
    case 0xE8:  Decode(format, 0, "CALL", 0, 1, -1);   break;
    case 0xFF:  Decode(format, 1, "CALL", 0, 1, 0);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        Push(EIP);
        if (format.operand[0].type == Format::Operand::IMM)
            EIP += format.operand[0].displacement;
        else
            EIP = *(uint32_t*)format.operand[0].memory;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CDQ()
{
    Format format;
    format.size = operand_size;
    format.length = 1;
    format.instruction = (operand_size == 16) ? "CWD" : "CDQ";

    BEGIN_OPERATION() {
        if (format.size == 16)
            DX = (int16_t)AX < 0 ? 0xFFFF : 0x0000;
        else
            EDX = (int32_t)EAX < 0 ? 0xFFFFFFFF : 0x00000000;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CLC()
{
    Format format;
    format.instruction = "CLC";

    BEGIN_OPERATION() {
        CF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CLD()
{
    Format format;
    format.instruction = "CLD";

    BEGIN_OPERATION() {
        DF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CMC()
{
    Format format;
    format.instruction = "CMC";

    BEGIN_OPERATION() {
        CF = !CF;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CMP()
{
    Format format;
    switch (opcode[0]) {
    case 0x38:
    case 0x39:
    case 0x3A:
    case 0x3B:  Decode(format, 1, "CMP", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x3C:
    case 0x3D:  Decode(format, 0, "CMP",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "CMP",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "CMP",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags(x86, TEMP, TEMP - SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CWDE()
{
    Format format;
    format.size = operand_size;
    format.length = 1;
    format.instruction = (operand_size == 16) ? "CBW" : "CWDE";

    BEGIN_OPERATION() {
        if (format.size == 16)
            AX = (int8_t)AL;
        else
            EAX = (int16_t)AX;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::DEC()
{
    Format format;
    switch (opcode[0]) {
    case 0x48:  Decode(format, 0, "DEC", 0,             0b01, 0);  break;
    case 0xFE:
    case 0xFF:  Decode(format, 1, "DEC", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::DIV()
{
    Format format;
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "DIV", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::ENTER()
{
    Format format;
    Decode(format, 0, "ENTER", 0, 0, 16);
    format.length += 1;
    format.operand[1].displacement = IMM8(opcode, 2);

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
x86_format::Format x86_instruction::IDIV()
{
    Format format;
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "IDIV", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
        typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
        DEST = dest / src;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::IMUL()
{
    Format format;
    switch (opcode[0]) {
    case 0x0F:  Decode(format, 2, "IMUL", 0,             0b01, 0);     break;
    case 0x69:  Decode(format, 1, "IMUL", 0,             0b01, -1);    break;
    case 0x6B:  Decode(format, 1, "IMUL", 0,             0b01, 8);     break;
    case 0xF6:
    case 0xF7:  Decode(format, 1, "IMUL", 0, opcode[0] & 0b01, 0);     break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        typename std::make_signed_t<std::remove_reference_t<decltype(DEST)>> dest = DEST;
        typename std::make_signed_t<std::remove_reference_t<decltype(SRC)>> src = SRC;
        DEST = dest * src;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::INC()
{
    Format format;
    switch (opcode[0]) {
    case 0x40:  Decode(format, 0, "INC", 0,             0b01, 0);  break;
    case 0xFE:
    case 0xFF:  Decode(format, 1, "INC", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST + 1);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::Jcc()
{
    Format format;
    switch (opcode[0]) {
    case 0x70:  Decode(format, 0, "JO", 0, 0, 8);      break;
    case 0x71:  Decode(format, 0, "JNO", 0, 0, 8);     break;
    case 0x72:  Decode(format, 0, "JC", 0, 0, 8);      break;
    case 0x73:  Decode(format, 0, "JNC", 0, 0, 8);     break;
    case 0x74:  Decode(format, 0, "JZ", 0, 0, 8);      break;
    case 0x75:  Decode(format, 0, "JNZ", 0, 0, 8);     break;
    case 0x76:  Decode(format, 0, "JBE", 0, 0, 8);     break;
    case 0x77:  Decode(format, 0, "JA", 0, 0, 8);      break;
    case 0x78:  Decode(format, 0, "JS", 0, 0, 8);      break;
    case 0x79:  Decode(format, 0, "JNS", 0, 0, 8);     break;
    case 0x7A:  Decode(format, 0, "JPE", 0, 0, 8);     break;
    case 0x7B:  Decode(format, 0, "JPO", 0, 0, 8);     break;
    case 0x7C:  Decode(format, 0, "JL", 0, 0, 8);      break;
    case 0x7D:  Decode(format, 0, "JGE", 0, 0, 8);     break;
    case 0x7E:  Decode(format, 0, "JLE", 0, 0, 8);     break;
    case 0x7F:  Decode(format, 0, "JG", 0, 0, 8);      break;
    case 0xE3:  Decode(format, 0, (operand_size == 16) ? "JCXZ" : "JECXZ", 0, 0, 8);   break;
    case 0x0F:
        switch (opcode[1]) {
        case 0x80:  Decode(format, 0, "JO", 0, 1, -1);     break;
        case 0x81:  Decode(format, 0, "JNO", 0, 1, -1);    break;
        case 0x82:  Decode(format, 0, "JC", 0, 1, -1);     break;
        case 0x83:  Decode(format, 0, "JNC", 0, 1, -1);    break;
        case 0x84:  Decode(format, 0, "JZ", 0, 1, -1);     break;
        case 0x85:  Decode(format, 0, "JNZ", 0, 1, -1);    break;
        case 0x86:  Decode(format, 0, "JBE", 0, 1, -1);    break;
        case 0x87:  Decode(format, 0, "JA", 0, 1, -1);     break;
        case 0x88:  Decode(format, 0, "JS", 0, 1, -1);     break;
        case 0x89:  Decode(format, 0, "JNS", 0, 1, -1);    break;
        case 0x8A:  Decode(format, 0, "JPE", 0, 1, -1);    break;
        case 0x8B:  Decode(format, 0, "JPO", 0, 1, -1);    break;
        case 0x8C:  Decode(format, 0, "JL", 0, 1, -1);     break;
        case 0x8D:  Decode(format, 0, "JGE", 0, 1, -1);    break;
        case 0x8E:  Decode(format, 0, "JLE", 0, 1, -1);    break;
        case 0x8F:  Decode(format, 0, "JG", 0, 1, -1);     break;
        }
        break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        // TODO
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::JMP()
{
    Format format;
    switch (opcode[0]) {
    case 0xE9:  Decode(format, 0, "JMP", 0, 1, -1);   break;
    case 0xEB:  Decode(format, 0, "JMP", 0, 0, 8);    break;
    case 0xFF:  Decode(format, 1, "JMP", 0, 1, 0);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        if (format.operand[0].type == Format::Operand::IMM)
            EIP += format.operand[0].displacement;
        else
            EIP = *(uint32_t*)format.operand[0].memory;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::LAHF()
{
    Format format;
    format.instruction = "LAHF";

    BEGIN_OPERATION() {
        AH = FLAGS;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::LEA()
{
    Format format;
    Decode(format, 1, "LEA", 0, 0, 0);
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = (decltype(DEST))format.operand[0].address;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::LEAVE()
{
    Format format;
    format.instruction = "LEAVE";

    BEGIN_OPERATION() {
        ESP = EBP;
        EBP = Pop();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::LOOP()
{
    Format format;
    switch (opcode[0]) {
    case 0xE0:  Decode(format, 0, "LOOPNZ", 0, 1, 8);  break;
    case 0xE1:  Decode(format, 0, "LOOPZ", 0, 1, 8);   break;
    case 0xE2:  Decode(format, 0, "LOOP", 0, 1, 8);    break;
    }

    BEGIN_OPERATION() {
        uint32_t CountReg = (format.size == 16) ? CX : ECX;
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
x86_format::Format x86_instruction::MOV()
{
    Format format;
    switch (opcode[0]) {
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:  Decode(format, 1, "MOV", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0xB0:
    case 0xB1:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB7:  Decode(format, 0, "MOV", 0, 0, -1);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111; break;
    case 0xB8:
    case 0xB9:
    case 0xBA:
    case 0xBB:
    case 0xBC:
    case 0xBD:
    case 0xBE:
    case 0xBF:  Decode(format, 0, "MOV", 0, 1, -1);
                format.operand[0].type = Format::Operand::REG;
                format.operand[0].base = opcode[0] & 0b111; break;
    case 0xC6:  Decode(format, 1, "MOV", 0, 0, -1);        break;
    case 0xC7:  Decode(format, 1, "MOV", 0, 1, -1);        break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::MOVSX()
{
    Format format;
    switch (opcode[1]) {
    case 0xBE:
    case 0xBF:  Decode(format, 2, "MOVSX", 0, opcode[1] & 0b01, 0);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        switch (format.size) {
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
x86_format::Format x86_instruction::MOVZX()
{
    Format format;
    switch (opcode[1]) {
    case 0xB6:
    case 0xB7:  Decode(format, 2, "MOVZX", 0, opcode[1] & 0b01, 0);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        switch (format.size) {
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
x86_format::Format x86_instruction::MUL()
{
    Format format;
    switch (opcode[0]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "MUL", 0, opcode[0] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = DEST / SRC;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::NEG()
{
    Format format;
    switch (opcode[1]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "NEG", 0, opcode[1] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        CF = (DEST == 0) ? 0 : 1;
        DEST = -DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::NOP()
{
    Format format;
    format.instruction = "NOP";

    BEGIN_OPERATION() {
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::NOT()
{
    Format format;
    switch (opcode[1]) {
    case 0xF6:
    case 0xF7:  Decode(format, 1, "NOT", 0, opcode[1] & 0b01, 0);  break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = !DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::OR()
{
    Format format;
    switch (opcode[0]) {
    case 0x08:
    case 0x09:
    case 0x0A:
    case 0x0B:  Decode(format, 1, "OR", opcode[0] & 0b10, opcode[0] & 0b01, 0);    break;
    case 0x0C:
    case 0x0D:  Decode(format, 0, "OR",             0b00, opcode[0] & 0b01, -1);   break;
    case 0x80:
    case 0x81:  Decode(format, 1, "OR",             0b00, opcode[0] & 0b01, -1);   break;
    case 0x83:  Decode(format, 1, "OR",             0b00, opcode[0] & 0b01, 8);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST | SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::POP()
{
    Format format;
    switch (opcode[0]) {
    case 0x58:  Decode(format, 1, "POP", 0, 1, 0); break;
    case 0x8F:  Decode(format, 1, "POP", 0, 1, 0); break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = Pop();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::POPAD()
{
    Format format;
    Decode(format, 0, "POPAD", 0, 1, 0);

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
x86_format::Format x86_instruction::POPFD()
{
    Format format;
    Decode(format, 0, "POPFD", 0, 1, 0);

    BEGIN_OPERATION() {
        EFLAGS = Pop();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::PUSH()
{
    Format format;
    switch (opcode[0]) {
    case 0x50:  Decode(format, 1, "PUSH", 0, 1, 0);    break;
    case 0x68:  Decode(format, 0, "PUSH", 0, 1, -1);   break;
    case 0x6A:  Decode(format, 0, "PUSH", 0, 1, 8);    break;
    case 0xFF:  Decode(format, 1, "PUSH", 0, 1, 0);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        Push(DEST);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::PUSHAD()
{
    Format format;
    Decode(format, 0, "PUSHAD", 0, 1, 0);

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
x86_format::Format x86_instruction::PUSHFD()
{
    Format format;
    Decode(format, 0, "PUSHFD", 0, 1, 0);

    BEGIN_OPERATION() {
        Push(EFLAGS);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::Rxx()
{
    Format format;
    switch (opcode[0]) {
    case 0xC0:
    case 0xC1:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, 1, "ROL", 0, opcode[0] & 0b01, 8);  break;
        case 1: Decode(format, 1, "ROR", 0, opcode[0] & 0b01, 8);  break;
        case 2: Decode(format, 1, "RCL", 0, opcode[0] & 0b01, 8);  break;
        case 3: Decode(format, 1, "RCR", 0, opcode[0] & 0b01, 8);  break;
        }
        break;
    case 0xD0:
    case 0xD1:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, 1, "ROL", 0, opcode[0] & 0b01, 0);  break;
        case 1: Decode(format, 1, "ROR", 0, opcode[0] & 0b01, 0);  break;
        case 2: Decode(format, 1, "RCL", 0, opcode[0] & 0b01, 0);  break;
        case 3: Decode(format, 1, "RCR", 0, opcode[0] & 0b01, 0);  break;
        }
        format.operand[1].type = Format::Operand::IMM;
        format.operand[1].displacement = 1;
        break;
    case 0xD2:
    case 0xD3:
        switch (opcode[1] >> 3 & 7) {
        case 0: Decode(format, 1, "ROL", 0, opcode[0] & 0b01, 0);  break;
        case 1: Decode(format, 1, "ROR", 0, opcode[0] & 0b01, 0);  break;
        case 2: Decode(format, 1, "RCL", 0, opcode[0] & 0b01, 0);  break;
        case 3: Decode(format, 1, "RCR", 0, opcode[0] & 0b01, 0);  break;
        }
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = REG(ECX);
        break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        switch (x86.opcode[1] >> 3 & 7) {
        case 0:
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateleft8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateleft16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateleft32(DEST, SRC);
            break;
        case 1:
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateright8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateright16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateright32(DEST, SRC);
            break;
        case 2: // TODO
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateleft8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateleft16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateleft32(DEST, SRC);
            break;
        case 3: // TODO
            if (sizeof(DEST) == 1)  DEST = __builtin_rotateright8(DEST, SRC);
            if (sizeof(DEST) == 2)  DEST = __builtin_rotateright16(DEST, SRC);
            if (sizeof(DEST) == 4)  DEST = __builtin_rotateright32(DEST, SRC);
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::REP()
{
    repeat_string_operation = true;

    return Format();
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::RET()
{
    Format format;
    switch (opcode[0]) {
    case 0xC2:  Decode(format, 0, "RET", 0, 0, 16);    break;
    case 0xC3:  Decode(format, 0, "RET", 0, 0, 0);     break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        EIP = Pop();
        ESP = ESP + format.operand[0].displacement;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SAHF()
{
    Format format;
    format.instruction = "SAHF";

    BEGIN_OPERATION() {
        FLAGS = AH;
    } END_OPERATION;
}
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
x86_format::Format x86_instruction::SBB()
{
    Format format;
    switch (opcode[0]) {
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:  Decode(format, 1, "SBB", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x1C:
    case 0x1D:  Decode(format, 0, "SBB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "SBB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "SBB",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - (SRC + CF));
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SETcc()
{
    Format format;
    switch (opcode[1]) {
    case 0x90:  Decode(format, 2, "SETO", 0, 0, 0);    break;
    case 0x91:  Decode(format, 2, "SETNO", 0, 0, 0);   break;
    case 0x92:  Decode(format, 2, "SETC", 0, 0, 0);    break;
    case 0x93:  Decode(format, 2, "SETNC", 0, 0, 0);   break;
    case 0x94:  Decode(format, 2, "SETZ", 0, 0, 0);    break;
    case 0x95:  Decode(format, 2, "SETNZ", 0, 0, 0);   break;
    case 0x96:  Decode(format, 2, "SETBE", 0, 0, 0);   break;
    case 0x97:  Decode(format, 2, "SETA", 0, 0, 0);    break;
    case 0x98:  Decode(format, 2, "SETS", 0, 0, 0);    break;
    case 0x99:  Decode(format, 2, "SETNS", 0, 0, 0);   break;
    case 0x9A:  Decode(format, 2, "SETPE", 0, 0, 0);   break;
    case 0x9B:  Decode(format, 2, "SETPO", 0, 0, 0);   break;
    case 0x9C:  Decode(format, 2, "SETL", 0, 0, 0);    break;
    case 0x9D:  Decode(format, 2, "SETGE", 0, 0, 0);   break;
    case 0x9E:  Decode(format, 2, "SETLE", 0, 0, 0);   break;
    case 0x9F:  Decode(format, 2, "SETG", 0, 0, 0);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        // TODO
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
x86_format::Format x86_instruction::STC()
{
    Format format;
    format.instruction = "STC";

    BEGIN_OPERATION() {
        CF = 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::STD()
{
    Format format;
    format.instruction = "STD";

    BEGIN_OPERATION() {
        DF = 1;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SUB()
{
    Format format;
    switch (opcode[0]) {
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:  Decode(format, 1, "SUB", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x2C:
    case 0x2D:  Decode(format, 0, "SUB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "SUB",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "SUB",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST - SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::TEST()
{
    Format format;
    switch (opcode[0]) {
    case 0x84:
    case 0x85:  Decode(format, 1, "TEST", 0, opcode[0] & 0b01, 0);     break;
    case 0xA8:
    case 0xA9:  Decode(format, 0, "TEST", 0, opcode[0] & 0b01, -1);    break;
    case 0xF6:
    case 0xF7:  Decode(format, 1, "TEST", 0, opcode[0] & 0b01, -1);    break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags(x86, TEMP, TEMP & SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::XCHG()
{
    Format format;
    switch (opcode[0]) {
    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
        Decode(format, 0, "XCHG", 0, 1, 0);
        format.operand[0].type = Format::Operand::REG;
        format.operand[0].base = REG(EAX);
        format.operand[1].type = Format::Operand::REG;
        format.operand[1].base = (opcode[0] & 0b111);
        break;
    case 0x86:
    case 0x87:
        Decode(format, 1, "XCHG", 0, opcode[0] & 0b01, 0);
        break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        DEST = SRC;
        SRC = TEMP;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::XLAT()
{
    Format format;
    format.instruction = "XLAT";

    BEGIN_OPERATION() {
        AL = *(x86.memory + EBX + AL);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::XOR()
{
    Format format;
    switch (opcode[0]) {
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:  Decode(format, 1, "XOR", opcode[0] & 0b10, opcode[0] & 0b01, 0);   break;
    case 0x34:
    case 0x35:  Decode(format, 0, "XOR",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x80:
    case 0x81:  Decode(format, 1, "XOR",             0b00, opcode[0] & 0b01, -1);  break;
    case 0x83:  Decode(format, 1, "XOR",             0b00, opcode[0] & 0b01, 8);   break;
    }
    Fixup(format);

    BEGIN_OPERATION() {
        UpdateFlags(x86, DEST, DEST ^ SRC);
        CF = 0;
        OF = 0;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
