#include "x86_instruction.h"
#include "x86_instruction.inl"
#include "x86_register.h"
#include "x86_register.inl"

//------------------------------------------------------------------------------
x86_format::Format x86_instruction::CMPSx()
{
    Format format;
    switch (opcode[0]) {
    case 0xA6:  Decode(format, 0, "CMPSB", 0, 0, 0);                                   break;
    case 0xA7:  Decode(format, 0, (operand_size == 16) ? "CMPSW" : "CMPSD", 0, 1, 0);  break;
    }
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(ESI);
    Fixup(format);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags(x86, TEMP, TEMP - SRC);
        ESI = DF == 0 ? ESI + sizeof(SRC) : ESI + sizeof(SRC);
        EDI = DF == 0 ? EDI + sizeof(DEST) : EDI + sizeof(DEST);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::LODSx()
{
    Format format;
    switch (opcode[0]) {
    case 0xAC:  Decode(format, 0, "LODSB", 0, 0, 0);                                   break;
    case 0xAD:  Decode(format, 0, (operand_size == 16) ? "LODSW" : "LODSD", 0, 1, 0);  break;
    }
    format.operand[0].type = Format::Operand::REG;
    format.operand[0].base = REG(EAX);
    format.operand[1].base = REG(ESI);
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = SRC;
        ESI = DF == 0 ? ESI + sizeof(SRC) : ESI + sizeof(SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::MOVSx()
{
    Format format;
    switch (opcode[0]) {
    case 0xA4:  Decode(format, 0, "MOVSB", 0, 0, 0);                                   break;
    case 0xA5:  Decode(format, 0, (operand_size == 16) ? "MOVSW" : "MOVSD", 0, 1, 0);  break;
    }
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(ESI);
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = SRC;
        ESI = DF == 0 ? ESI + sizeof(SRC) : ESI + sizeof(SRC);
        EDI = DF == 0 ? EDI + sizeof(DEST) : EDI + sizeof(DEST);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::SCASx()
{
    Format format;
    switch (opcode[0]) {
    case 0xAE:  Decode(format, 0, "SCASB", 0, 0, 0);                                   break;
    case 0xAF:  Decode(format, 0, (operand_size == 16) ? "SCASW" : "SCASD", 0, 1, 0);  break;
    }
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(EAX);
    Fixup(format);

    BEGIN_OPERATION() {
        auto TEMP = DEST;
        UpdateFlags(x86, TEMP, SRC - TEMP);
        EDI = DF == 0 ? EDI + sizeof(DEST) : EDI + sizeof(DEST);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
x86_format::Format x86_instruction::STOSx()
{
    Format format;
    switch (opcode[0]) {
    case 0xAA:  Decode(format, 0, "STOSB", 0, 0, 0);                                   break;
    case 0xAB:  Decode(format, 0, (operand_size == 16) ? "STOSW" : "STOSD", 0, 1, 0);  break;
    }
    format.operand[0].type = Format::Operand::REG;
    format.operand[0].base = REG(EDI);
    format.operand[1].base = REG(EAX);
    Fixup(format);

    BEGIN_OPERATION() {
        DEST = SRC;
        EDI = DF == 0 ? EDI + sizeof(SRC) : EDI + sizeof(SRC);
    } END_OPERATION;
}
//------------------------------------------------------------------------------
