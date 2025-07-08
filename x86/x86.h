//==============================================================================
// miCPU : x86 Header
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#pragma once

#include "miCPU.h"

struct x86 : public miCPU
{
    union register_t
    {
        uint32_t d;
        uint16_t w;
        struct
        {
            uint8_t l;
            uint8_t h;
        };
    };
    union flags_t
    {
        uint32_t flags;
        struct
        {
            uint32_t _c:1;  // Carry
            uint32_t _1:1;
            uint32_t _p:1;  // Parity
            uint32_t _3:1;
            uint32_t _a:1;  // Auxiliary
            uint32_t _5:1;
            uint32_t _z:1;  // Zero
            uint32_t _s:1;  // Sign
            uint32_t _t:1;  // Trap
            uint32_t _i:1;  // Interrupt ?
            uint32_t _d:1;  // Direction ?
            uint32_t _o:1;  // Overflow
        };
    };
    register_t empty = {};
    register_t regs[16] = {};
    flags_t eflags = {};
    uint32_t eip = 0;

    uint8_t* opcode = nullptr;
    bool operand_size_override = false;

    uint8_t* memory = nullptr;
    uint8_t* stack = nullptr;

    std::string* disasm = nullptr;

    virtual ~x86();
    bool Initialize(size_t space, const void* program, size_t size) override;
    bool Step() override;
    std::string Disassemble(int count) override;

protected:
    struct Format
    {
        struct Addressing
        {
            bool imm = false;
            bool reg = false;

            int scale = 0;
            int index = 0;
            int base = -1;
            int32_t displacement = 0;

            uint8_t* memory = nullptr;
        };
        int size = 0;
        int length = 0;
        const char* instruction = "";
        Addressing addressing[2] = {};
    };
    Format      Decode(int offset, const char* instruction, int direction, int operand_size, int immediate_size);
    void        Disasm(const Format& format);
    void        Fixup(Format& format);

    template<typename A, typename B>
    void UpdateEFlags(A& DEST, B TEMP);

protected:
    typedef void instruction();
    typedef void (x86::*instruction_pointer)();

    instruction esc;
    instruction grp1;
    instruction grp2;
    instruction grp3;
    instruction grp4;
    instruction grp5;
    instruction two;
    instruction ___;

//  instruction CS;     // CS segment override prefix
//  instruction SS;     // SS segment override prefix
//  instruction DS;     // DS segment override prefix
//  instruction ES;     // ES segment override prefix
//  instruction FS;     // FS segment override prefix
//  instruction GS;     // GS segment override prefix
    instruction OS;     // Operand-size override
//  instruction AS;     // Address-size override

    instruction AAA;    // ASCII Adjust after Addition
    instruction AAD;    // ASCII Adjust AX before Division
    instruction AAM;    // ASCII Adjust AX after Multiply
    instruction AAS;    // ASCII Adjust AL after Subtraction
    instruction ADC;    // Add with Carry
    instruction ADD;    // Add
    instruction AND;    // Logical AND
//  instruction ARPL;   // Adjust RPL Field of Selector
//  instruction BOUND;  // Check Array Index Against Bounds
    instruction BSF;    // Bit Scan Forward
    instruction BSR;    // Bit Scan Reverse
    instruction BT;     // Bit Test
    instruction BTC;    // Bit Test and Complement
    instruction BTR;    // Bit Test and Reset
    instruction BTS;    // Bit Test and Set
    instruction CALL;   // Call Procedure
    instruction CLC;    // Clear Carry Flag
    instruction CLD;    // Clear Direction Flag
    instruction CLI;    // Clear Interrupt Flag
//  instruction CLTS;   // Clear Task-Switched Flag in CR0
    instruction CMC;    // Complement Carry Flag
    instruction CMP;    // Compare Two Operands
    instruction CMPS;   // Compare String Operands
    instruction Cxx;    // Convert Byte to Word/Convert Word to Doubleword/Convert Doubleword to Quadword
    instruction DAA;    // Decimal Adjust AL after Addition
    instruction DAS;    // Decimal Adjust AL after Subtraction
    instruction DEC;    // Decrement by 1
    instruction DIV;    // Unsigned Divide
    instruction ENTER;  // Make Stack Frame for Procedure Parameters
    instruction HLT;    // Halt
    instruction IDIV;   // Signed Divide
    instruction IMUL;   // Signed Multiply
    instruction IN;     // Input from Port
    instruction INC;    // Increment by 1
    instruction INS;    // Input from Port to String
    instruction INT;    // Call to Interrupt Procedure
    instruction IRET;   // Interrupt Return
    instruction Jcc;    // Jump if Condition is Met
    instruction JMP;    // Jump
    instruction LAHF;   // Load Flags into AH Register
    instruction LAR;    // Load Access Rights Byte
    instruction LEA;    // Load Effective Address
    instruction LEAVE;  // High Level Procedure Exit
//  instruction LxDT;   // Load Global/Interrupt Descriptor Table Register
//  instruction LxS;    // Load Full Pointer
//  instruction LLDT;   // Load Local Descriptor Table Register
//  instruction LMSW;   // Load Machine Status Word
    instruction LOCK;   // Assert LOCK# Signal Prefix
    instruction LODS;   // Load String Operand
    instruction LOOP;   // Loop Control with CX Counter
//  instruction LSL;    // Load Segment Limit
//  instruction LTR;    // Load Task Register
    instruction MOV;    // Move Data
    instruction MOVS;   // Move Data from String to String
    instruction MOVSX;  // Move with Sign-Extend
    instruction MOVZX;  // Move with Zero-Extend
    instruction MUL;    // Unsigned Multiplication of AL or AX
    instruction NEG;    // Two's Complement Negation
    instruction NOP;    // No Operation
    instruction NOT;    // One's Complement Negation
    instruction OR;     // Logical Inclusive OR
    instruction OUT;    // Output to Port
    instruction OUTS;   // Output String to Port
    instruction POP;    // Pop a Word from the Stack
    instruction POPA;   // Pop all General Registers
    instruction POPF;   // Pop Stack into FLAGS or EFLAGS Register
    instruction PUSH;   // Push Operand onto the Stack
    instruction PUSHA;  // Push all General Registers
    instruction PUSHF;  // Push Flags Register onto the Stack
    instruction Rxx;    // Rotate
    instruction REP;    // Repeat Following String Operation
    instruction RET;    // Return from Procedure
    instruction SAHF;   // Store AH into Flags
    instruction Sxx;    // Shift Instructions
    instruction SBB;    // Integer Subtraction with Borrow
    instruction SCAS;   // Compare String Data
    instruction SETcc;  // Byte Set on Condition
//  instruction SxDT;   // Store Global/Interrupt Descriptor Table Register
    instruction SHLD;   // Double Precision Shift Left
    instruction SHRD;   // Double Precision Shift Right
//  instruction SLDT;   // Store Local Descriptor Table Register
//  instruction SMSW;   // Store Machine Status Word
    instruction STC;    // Set Carry Flag
    instruction STD;    // Set Direction Flag
    instruction STI;    // Set Interrupt Flag
    instruction STOS;   // Store String Data
//  instruction STR;    // Store Task Register
    instruction SUB;    // Integer Subtraction
    instruction TEST;   // Logical Compare
//  instruction VERx;   // Verify a Segment for Reading or Writing
    instruction WAIT;   // Wait until BUSY# Pin is Inactive (HIGH)
    instruction XCHG;   // Exchange Register/Memory with Register
    instruction XLAT;   // Table Look-up Translation
    instruction XOR;    // Logical Exclusive OR

    static const instruction_pointer one[256];
    static const instruction_pointer group[16][8];
};
