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

    switch (format.prefix) {
    case 0xF2:
    case 0xF3:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            auto compare = (format.prefix == 0xF2) ? 1 : 0;
            auto PDEST = &DEST;
            auto PSRC = &SRC;
            auto esi = ESI;
            auto edi = EDI;
            auto ecx = ECX;
            while (ecx) {
                ecx--;
                auto TEMP = (*PDEST);
                auto SRC = (*PSRC);
                UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
                PSRC += step;
                PDEST += step;
                esi += sizeof(DEST) * step;
                edi += sizeof(DEST) * step;
                if (ZF == compare)
                    break;
            }
            ESI = esi;
            EDI = edi;
            ECX = ecx;
        } END_OPERATION;
        break;
    default:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            auto TEMP = DEST;
            UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
            ESI += sizeof(DEST) * step;
            EDI += sizeof(DEST) * step;
        } END_OPERATION;
        break;
    }
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

    switch (format.prefix) {
    case 0xF3:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            auto PSRC = &SRC;
            auto ecx = ECX;
            while (ecx) {
                ecx--;
                DEST = (*PSRC);
                PSRC += step;
            }
            ESI += sizeof(DEST) * step * ECX;
            ECX = 0;
        } END_OPERATION;
        break;
    default:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            DEST = SRC;
            ESI += sizeof(DEST) * step;
        } END_OPERATION;
        break;
    }
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

    switch (format.prefix) {
    case 0xF3:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            auto PDEST = &DEST;
            auto PSRC = &SRC;
            auto ecx = ECX;
            while (ecx) {
                ecx--;
                (*PDEST) = (*PSRC);
                PDEST += step;
                PSRC += step;
            }
            ESI += sizeof(DEST) * step * ECX;
            EDI += sizeof(DEST) * step * ECX;
            ECX = 0;
        } END_OPERATION;
        break;
    default:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            DEST = SRC;
            ESI += sizeof(DEST) * step;
            EDI += sizeof(DEST) * step;
        } END_OPERATION;
        break;
    }
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

    switch (format.prefix) {
    case 0xF2:
    case 0xF3:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            auto compare = (format.prefix == 0xF2) ? 1 : 0;
            auto SRC = SRC1;
            auto PDEST = &DEST;
            auto edi = EDI;
            auto ecx = ECX;
            while (ecx) {
                ecx--;
                auto TEMP = (*PDEST);
                UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
                PDEST += step;
                edi += sizeof(DEST) * step;
                if (ZF == compare)
                    break;
            }
            EDI = edi;
            ECX = ecx;
        } END_OPERATION;
        break;
    default:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            auto TEMP = DEST;
            UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
            ESI += sizeof(DEST) * step;
            EDI += sizeof(DEST) * step;
        } END_OPERATION;
        break;
    }
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

    switch (format.prefix) {
    case 0xF3:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            auto SRC = SRC1;
            auto PDEST = &DEST;
            auto ecx = ECX;
            while (ecx) {
                ecx--;
                (*PDEST) = SRC;
                PDEST += step;
            }
            EDI += sizeof(DEST) * ECX;
            ECX = 0;
        } END_OPERATION;
        break;
    default:
        BEGIN_OPERATION() {
            auto step = (DF == 0) ? 1 : -1;
            DEST = SRC;
            EDI += sizeof(DEST) * step;
        } END_OPERATION;
        break;
    }
}
//------------------------------------------------------------------------------
