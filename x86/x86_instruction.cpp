#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::_(Format& format, const uint8_t* opcode)
{
    format.instruction = "UNKNOWN";
    format.operand_count = 0;

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
void x86_instruction::REPE(Format& format, const uint8_t* opcode)
{
    format.repeatF2 = false;
    format.repeatF3 = true;
}
//------------------------------------------------------------------------------
void x86_instruction::REPNE(Format& format, const uint8_t* opcode)
{
    format.repeatF2 = true;
    format.repeatF3 = false;
}
//------------------------------------------------------------------------------
void x86_instruction::WAIT(Format& format, const uint8_t* opcode)
{
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void x86_instruction::BSWAP(Format& format, const uint8_t* opcode)
{
    format.instruction = "BSWAP";
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::REG;
    format.operand[0].base = opcode[1] & 0b111;

    BEGIN_OPERATION() {
        DEST = __builtin_bswap32((uint32_t)DEST);
    } END_OPERATION;
}
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
    format.operand_count = 1;

    BEGIN_OPERATION() {
        Push32(EIP);
        EIP = (uint32_t)DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::CBW(Format& format, const uint8_t* opcode)
{
    switch (format.width) {
    case 16:    format.instruction = "CBW";     break;
    case 32:    format.instruction = "CWDE";    break;
#if HAVE_X64
    case 64:    format.instruction = "CDQE";    break;
#endif
    }
    format.operand_count = 1;
    format.operand[0].type = Format::Operand::REG;
    format.operand[0].flags = Format::Operand::HIDE;
    format.operand[0].base = IndexREG(EAX);

    switch (format.width) {
    case 16:    BEGIN_OPERATION() { DEST = (int8_t)DEST;  } END_OPERATION_RANGE(16, 16);    break;
    case 32:    BEGIN_OPERATION() { DEST = (int16_t)DEST; } END_OPERATION_RANGE(32, 32);    break;
#if HAVE_X64
    case 64:    BEGIN_OPERATION() { DEST = (int32_t)DEST; } END_OPERATION_RANGE(64, 64);    break;
#endif
    }
}
//------------------------------------------------------------------------------
void x86_instruction::CLC(Format& format, const uint8_t* opcode)
{
    format.instruction = "CLC";
    format.operand_count = 0;

    OPERATION() {
        CF = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CLD(Format& format, const uint8_t* opcode)
{
    format.instruction = "CLD";
    format.operand_count = 0;

    OPERATION() {
        DF = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CMC(Format& format, const uint8_t* opcode)
{
    format.instruction = "CMC";
    format.operand_count = 0;

    OPERATION() {
        CF = !CF;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::CMOVcc(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "CMOVcc", 2);

    switch (opcode[1]) {
    case 0x40: BEGIN_OPERATION() { if (((OF)          ) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVO
    case 0x41: BEGIN_OPERATION() { if (((OF)          ) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVNO
    case 0x42: BEGIN_OPERATION() { if (((CF)          ) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVC
    case 0x43: BEGIN_OPERATION() { if (((CF)          ) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVNC
    case 0x44: BEGIN_OPERATION() { if (((ZF)          ) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVZ
    case 0x45: BEGIN_OPERATION() { if (((ZF)          ) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVNZ
    case 0x46: BEGIN_OPERATION() { if (((CF | ZF)     ) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVBE
    case 0x47: BEGIN_OPERATION() { if (((CF | ZF)     ) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVA
    case 0x48: BEGIN_OPERATION() { if (((SF)          ) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVS
    case 0x49: BEGIN_OPERATION() { if (((SF)          ) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVNS
    case 0x4A: BEGIN_OPERATION() { if (((PF)          ) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVPE
    case 0x4B: BEGIN_OPERATION() { if (((PF)          ) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVPO
    case 0x4C: BEGIN_OPERATION() { if (((SF ^ OF)     ) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVL
    case 0x4D: BEGIN_OPERATION() { if (((SF ^ OF)     ) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVGE
    case 0x4E: BEGIN_OPERATION() { if (((SF ^ OF) | ZF) == 1) DEST = SRC; } END_OPERATION;  break;  // CMOVLE
    case 0x4F: BEGIN_OPERATION() { if (((SF ^ OF) | ZF) == 0) DEST = SRC; } END_OPERATION;  break;  // CMOVG
    }
}
//------------------------------------------------------------------------------
void x86_instruction::CWD(Format& format, const uint8_t* opcode)
{
    switch (format.width) {
    case 16:    format.instruction = "CWD"; break;
    case 32:    format.instruction = "CDQ"; break;
#if HAVE_X64
    case 64:    format.instruction = "CQO"; break;
#endif
    }
    format.operand[1].type = Format::Operand::REG;
    format.operand[1].flags = Format::Operand::HIDE;
    format.operand[1].base = IndexREG(EAX);
    format.operand[0].type = Format::Operand::REG;
    format.operand[0].flags = Format::Operand::HIDE;
    format.operand[0].base = IndexREG(EDX);

    BEGIN_OPERATION() {
        const auto& SRC = (std::make_signed_t<std::remove_reference_t<decltype(SRC1)>>&)SRC1;
        DEST = (SRC >> (sizeof(SRC) * 8 - 1));
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
    format.operand_count = 1;

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
    case 0xEB:  Decode(format, opcode, "JMP", 1,  8, OPERAND_SIZE | RELATIVE);              break;
    case 0xFF:
        switch (opcode[1] & 0b00111000) {
        case 0b00100000:    Decode(format, opcode, "JMP", 1, 0, OPERAND_SIZE);              break;
//      case 0b00101000:    Decode(format, opcode, "JMP", 2, -1, OPERAND_SIZE | INDIRECT);  break;
        }
    }
    format.operand_count = 1;

    BEGIN_OPERATION() {
        EIP = (uint32_t)DEST;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::LAHF(Format& format, const uint8_t* opcode)
{
    format.instruction = "LAHF";
    format.operand_count = 0;

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
    format.operand_count = 0;

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
    format.operand[1].type = Format::Operand::REG;
    format.operand[1].flags = Format::Operand::HIDE;
    format.operand[1].base = IndexREG(ECX);
    format.operand[0].type = Format::Operand::IMM;
    format.operand[0].flags = Format::Operand::HIDE;

    BEGIN_OPERATION() {
        auto CountReg = SRC;
        if (CountReg) {
            SRC = SRC - 1;
            bool BranchCond = true;
            switch (x86.opcode[0]) {
            case 0xE0:  BranchCond = (ZF == 0); break;
            case 0xE1:  BranchCond = (ZF != 0); break;
            }
            if (BranchCond) {
                EIP += format.operand[0].displacement;
            }
        }
    } END_OPERATION;
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
                format.operand[0].base = IndexREG(EAX);                 break;
    case 0xA2:
    case 0xA3:  Decode(format, opcode, "MOV", 1, 32, (opcode[0] & 0b01) | INDIRECT);
                format.operand[1].type = Format::Operand::REG;
                format.operand[1].base = IndexREG(EAX);                 break;
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
    case 0xBE:  format.operand[1].flags = Format::Operand::BIT8;    break;
    case 0xBF:  format.operand[1].flags = Format::Operand::BIT16;   break;
    }

    switch (opcode[1]) {
    case 0xBE:  BEGIN_OPERATION() { DEST = (int8_t)SRC; } END_OPERATION;    break;
    case 0xBF:  BEGIN_OPERATION() { DEST = (int16_t)SRC; } END_OPERATION;   break;
    }
}
//------------------------------------------------------------------------------
void x86_instruction::MOVZX(Format& format, const uint8_t* opcode)
{
    Decode(format, opcode, "MOVZX", 2, 0, DIRECTION | OPERAND_SIZE);
    switch (opcode[1]) {
    case 0xB6:  format.operand[1].flags = Format::Operand::BIT8;    break;
    case 0xB7:  format.operand[1].flags = Format::Operand::BIT16;   break;
    }

    switch (opcode[1]) {
    case 0xB6:  BEGIN_OPERATION() { DEST = (uint8_t)SRC; } END_OPERATION;   break;
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
    format.operand_count = 0;

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
    format.operand_count = 1;

    BEGIN_OPERATION() {
        DEST = Pop32();
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::POPA(Format& format, const uint8_t* opcode)
{
    format.instruction = "POPA";
    format.operand_count = 0;

    OPERATION() {
        DI = Pop16();
        SI = Pop16();
        BP = Pop16();
        Pop16();
        BX = Pop16();
        DX = Pop16();
        CX = Pop16();
        AX = Pop16();
    };
}
//------------------------------------------------------------------------------
void x86_instruction::POPAD(Format& format, const uint8_t* opcode)
{
    format.instruction = "POPAD";
    format.operand_count = 0;

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
void x86_instruction::POPF(Format& format, const uint8_t* opcode)
{
    format.instruction = "POPF";
    format.operand_count = 0;

    OPERATION() {
        FLAGS = Pop16();
    };
}
//------------------------------------------------------------------------------
void x86_instruction::POPFD(Format& format, const uint8_t* opcode)
{
    format.instruction = "POPFD";
    format.operand_count = 0;

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
    format.operand_count = 1;

    BEGIN_OPERATION() {
        Push32(DEST);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::PUSHA(Format& format, const uint8_t* opcode)
{
    format.instruction = "PUSHA";
    format.operand_count = 0;

    OPERATION() {
        auto Temp = SP;
        Push16(AX);
        Push16(CX);
        Push16(DX);
        Push16(BX);
        Push16(Temp);
        Push16(BP);
        Push16(SI);
        Push16(DI);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::PUSHAD(Format& format, const uint8_t* opcode)
{
    format.instruction = "PUSHAD";
    format.operand_count = 0;

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
void x86_instruction::PUSHF(Format& format, const uint8_t* opcode)
{
    format.instruction = "PUSHF";
    format.operand_count = 0;

    OPERATION() {
        Push16(FLAGS);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::PUSHFD(Format& format, const uint8_t* opcode)
{
    format.instruction = "PUSHFD";
    format.operand_count = 0;

    OPERATION() {
        Push32(EFLAGS);
    };
}
//------------------------------------------------------------------------------
void x86_instruction::RDPMC(Format& format, const uint8_t* opcode)
{
    format.instruction = "RDPMC";
    format.operand_count = 0;

    OPERATION() {
        EDX = 0;
        EAX = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::RDTSC(Format& format, const uint8_t* opcode)
{
    format.instruction = "RDTSC";
    format.operand_count = 0;

    OPERATION() {
        EDX = 0;
        EAX = 0;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::RET(Format& format, const uint8_t* opcode)
{
    format.instruction = "RET";
    switch (opcode[0]) {
    case 0xC2:
        format.length = 3;
        format.operand_count = 1;
        format.operand[0].type = Format::Operand::IMM;
        format.operand[0].displacement = IMM16(opcode, 1);
        break;
    case 0xC3:
        format.operand_count = 0;
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
    format.operand_count = 0;

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
    format.operand_count = 1;

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
    format.operand_count = 0;

    OPERATION() {
        CF = 1;
    };
}
//------------------------------------------------------------------------------
void x86_instruction::STD(Format& format, const uint8_t* opcode)
{
    format.instruction = "STD";
    format.operand_count = 0;

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
        format.operand[0].base = IndexREG(EAX);
        format.operand[1].base = opcode[0] & 0b111;
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
    format.operand_count = 0;

    OPERATION() {
        AL = *(x86.memory_address + EBX + AL);
    };
}
//------------------------------------------------------------------------------
