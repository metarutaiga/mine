#include "x86_register.h"
#include "x86_register.inl"
#include "x86_instruction.h"
#include "x86_instruction.inl"

//------------------------------------------------------------------------------
void x86_instruction::CMPSx(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xA6:  format.width = 8;               break;
    case 0xA7:  format.width = format.width;    break;
    }
    switch (format.width) {
    case 8:     format.instruction = "CMPSB";   break;
    case 16:    format.instruction = "CMPSW";   break;
    case 32:    format.instruction = "CMPSD";   break;
    }
    format.operand[0].type = Format::Operand::ADR;
    format.operand[1].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EDI);
    format.operand[1].base = IndexREG(ESI);

    BEGIN_OPERATION() {
        for (;;) {
            if (format.repeatF2 || format.repeatF3) {
                if (ECX == 0)
                    break;
                ECX--;
            }
            auto SRC = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + ESI);
            auto TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + EDI);
            UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
            ESI = DF == 0 ? ESI + sizeof(SRC) : ESI - sizeof(SRC);
            EDI = DF == 0 ? EDI + sizeof(DEST) : EDI - sizeof(DEST);
            if (format.repeatF2 || format.repeatF3) {
                if (format.repeatF2 && ZF == 1)
                    break;
                if (format.repeatF3 && ZF == 0)
                    break;
                continue;
            }
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::LODSx(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xAC:  format.width = 8;               break;
    case 0xAD:  format.width = format.width;    break;
    }
    switch (format.width) {
    case 8:     format.instruction = "LODSB";   break;
    case 16:    format.instruction = "LODSW";   break;
    case 32:    format.instruction = "LODSD";   break;
    }
    format.operand[0].type = Format::Operand::REG;
    format.operand[1].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EAX);
    format.operand[1].base = IndexREG(ESI);

    BEGIN_OPERATION() {
        for (;;) {
            if (format.repeatF3) {
                if (ECX == 0)
                    break;
                ECX--;
            }
            auto SRC = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + ESI);
            DEST = SRC;
            ESI = DF == 0 ? ESI + sizeof(SRC) : ESI - sizeof(SRC);
            if (format.repeatF3) {
                continue;
            }
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::MOVSx(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xA4:  format.width = 8;               break;
    case 0xA5:  format.width = format.width;    break;
    }
    switch (format.width) {
    case 8:     format.instruction = "MOVSB";   break;
    case 16:    format.instruction = "MOVSW";   break;
    case 32:    format.instruction = "MOVSD";   break;
    }
    format.operand[0].type = Format::Operand::ADR;
    format.operand[1].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EDI);
    format.operand[1].base = IndexREG(ESI);

    BEGIN_OPERATION() {
        for (;;) {
            if (format.repeatF3) {
                if (ECX == 0)
                    break;
                ECX--;
            }
            auto SRC = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + ESI);
            auto& TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + EDI);
            TEMP = SRC;
            ESI = DF == 0 ? ESI + sizeof(SRC) : ESI - sizeof(SRC);
            EDI = DF == 0 ? EDI + sizeof(DEST) : EDI - sizeof(DEST);
            if (format.repeatF3) {
                continue;
            }
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::SCASx(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xAE:  format.width = 8;               break;
    case 0xAF:  format.width = format.width;    break;
    }
    switch (format.width) {
    case 8:     format.instruction = "SCASB";   break;
    case 16:    format.instruction = "SCASW";   break;
    case 32:    format.instruction = "SCASD";   break;
    }
    format.operand[0].type = Format::Operand::ADR;
    format.operand[1].type = Format::Operand::ADR;
    format.operand[0].base = IndexREG(EDI);
    format.operand[1].base = IndexREG(EAX);

    BEGIN_OPERATION() {
        for (;;) {
            if (format.repeatF2 || format.repeatF3) {
                if (ECX == 0)
                    break;
                ECX--;
            }
            auto TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + EDI);
            UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
            EDI = DF == 0 ? EDI + sizeof(DEST) : EDI - sizeof(DEST);
            if (format.repeatF2 || format.repeatF3) {
                if (format.repeatF2 && ZF == 1)
                    break;
                if (format.repeatF3 && ZF == 0)
                    break;
                continue;
            }
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
void x86_instruction::STOSx(Format& format, const uint8_t* opcode)
{
    switch (opcode[0]) {
    case 0xAA:  format.width = 8;               break;
    case 0xAB:  format.width = format.width;    break;
    }
    switch (format.width) {
    case 8:     format.instruction = "STOSB";   break;
    case 16:    format.instruction = "STOSW";   break;
    case 32:    format.instruction = "STOSD";   break;
    }
    format.operand[0].type = Format::Operand::ADR;
    format.operand[1].type = Format::Operand::REG;
    format.operand[0].base = IndexREG(EDI);
    format.operand[1].base = IndexREG(EAX);

    BEGIN_OPERATION() {
        for (;;) {
            if (format.repeatF3) {
                if (ECX == 0)
                    break;
                ECX--;
            }
            auto& TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + EDI);
            TEMP = SRC;
            EDI = DF == 0 ? EDI + sizeof(DEST) : EDI - sizeof(DEST);
            if (format.repeatF3) {
                continue;
            }
            break;
        }
    } END_OPERATION;
}
//------------------------------------------------------------------------------
