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
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(ESI);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags<1, 1, 1, 1, 1, 1>(x86, TEMP, TEMP - SRC);
        ESI = DF == 0 ? ESI + sizeof(SRC) : ESI + sizeof(SRC);
        EDI = DF == 0 ? EDI + sizeof(DEST) : EDI + sizeof(DEST);
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
    format.operand[0].base = REG(EAX);
    format.operand[1].base = REG(ESI);

    BEGIN_OPERATION() {
        DEST = SRC;
        ESI = DF == 0 ? ESI + sizeof(SRC) : ESI + sizeof(SRC);
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
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(ESI);

    BEGIN_OPERATION() {
        DEST = SRC;
        ESI = DF == 0 ? ESI + sizeof(SRC) : ESI + sizeof(SRC);
        EDI = DF == 0 ? EDI + sizeof(DEST) : EDI + sizeof(DEST);
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
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(EAX);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags<1, 1, 1, 1, 1, 1>(x86, TEMP, SRC - TEMP);
        EDI = DF == 0 ? EDI + sizeof(DEST) : EDI + sizeof(DEST);
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
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(EAX);

    BEGIN_OPERATION() {
        DEST = SRC;
        EDI = DF == 0 ? EDI + sizeof(SRC) : EDI + sizeof(SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
