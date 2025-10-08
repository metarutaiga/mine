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
        auto ecx = ECX;
        auto esi = ESI;
        auto edi = EDI;
        auto esi_move = DF == 0 ? sizeof(SRC) : -sizeof(SRC);
        auto edi_move = DF == 0 ? sizeof(DEST) : -sizeof(DEST);
        for (;;) {
            if (format.prefix == 0xF2 || format.prefix == 0xF3) {
                if (ecx == 0)
                    break;
                ecx--;
            }
            auto TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + edi);
            auto SRC = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + esi);
            UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
            esi += esi_move;
            edi += edi_move;
            if (format.prefix == 0xF2 || format.prefix == 0xF3) {
                if (format.prefix == 0xF2 && ZF == 1)
                    break;
                if (format.prefix == 0xF3 && ZF == 0)
                    break;
                continue;
            }
            break;
        }
        ECX = ecx;
        ESI = esi;
        EDI = edi;
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
        auto ecx = ECX;
        auto esi = ESI;
        auto esi_move = DF == 0 ? sizeof(SRC) : -sizeof(SRC);
        for (;;) {
            if (format.prefix == 0xF3) {
                if (ecx == 0)
                    break;
                ecx--;
            }
            auto SRC = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + esi);
            DEST = SRC;
            esi += esi_move;
            if (format.prefix == 0xF3) {
                continue;
            }
            break;
        }
        ECX = ecx;
        ESI = esi;
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
        auto ecx = ECX;
        auto esi = ESI;
        auto edi = EDI;
        auto esi_move = DF == 0 ? sizeof(SRC) : -sizeof(SRC);
        auto edi_move = DF == 0 ? sizeof(DEST) : -sizeof(DEST);
        for (;;) {
            if (format.prefix == 0xF3) {
                if (ecx == 0)
                    break;
                ecx--;
            }
            auto& TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + edi);
            auto SRC = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + esi);
            TEMP = SRC;
            esi += esi_move;
            edi += edi_move;
            if (format.prefix == 0xF3) {
                continue;
            }
            break;
        }
        ECX = ecx;
        ESI = esi;
        EDI = edi;
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
        auto ecx = ECX;
        auto edi = EDI;
        auto edi_move = DF == 0 ? sizeof(DEST) : -sizeof(DEST);
        for (;;) {
            if (format.prefix == 0xF2 || format.prefix == 0xF3) {
                if (ecx == 0)
                    break;
                ecx--;
            }
            auto TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + edi);
            UpdateFlags<OSZAPC, BORROW>(x86, TEMP, TEMP - SRC, TEMP, SRC);
            edi += edi_move;
            if (format.prefix == 0xF2 || format.prefix == 0xF3) {
                if (format.prefix == 0xF2 && ZF == 1)
                    break;
                if (format.prefix == 0xF3 && ZF == 0)
                    break;
                continue;
            }
            break;
        }
        ECX = ecx;
        EDI = edi;
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
        auto ecx = ECX;
        auto edi = EDI;
        auto edi_move = DF == 0 ? sizeof(DEST) : -sizeof(DEST);
        for (;;) {
            if (format.prefix == 0xF3) {
                if (ecx == 0)
                    break;
                ecx--;
            }
            auto& TEMP = *(std::remove_reference_t<decltype(DEST)>*)(x86.memory_address + edi);
            TEMP = SRC;
            edi += edi_move;
            if (format.prefix == 0xF3) {
                continue;
            }
            break;
        }
        ECX = ecx;
        EDI = edi;
    } END_OPERATION;
}
//------------------------------------------------------------------------------
