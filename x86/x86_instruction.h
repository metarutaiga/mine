#pragma once

#include <string>

#include "x86_format.h"
#include "x86_register.h"

struct x86_instruction : public x86_format
                       , public x86_register
{
    x86_instruction& x86 = (*this);

protected:
    uint8_t* memory = nullptr;
    uint8_t* opcode = nullptr;
    uint8_t* stack = nullptr;

protected:
    std::string* disasm = nullptr;

    void        Decode(Format& format, int offset, const char* instruction, int direction, int have_operand_size, int immediate_size);
    std::string Disasm(const Format& format, int operand = 2);
    void        Fixup(Format& format);

    template<typename A, typename B>
    static void UpdateFlags(x86_instruction& x86, A& DEST, B TEMP);

protected:
    typedef Format instruction();
    typedef Format (x86_instruction::*instruction_pointer)();

    instruction _;

    instruction CS;     // CS segment override prefix
    instruction SS;     // SS segment override prefix
    instruction DS;     // DS segment override prefix
    instruction ES;     // ES segment override prefix
    instruction FS;     // FS segment override prefix
    instruction GS;     // GS segment override prefix

    instruction AAA;    // ASCII Adjust after Addition
    instruction AAD;    // ASCII Adjust AX before Division
    instruction AAM;    // ASCII Adjust AX after Multiply
    instruction AAS;    // ASCII Adjust AL after Subtraction
    instruction DAA;    // Decimal Adjust AL after Addition
    instruction DAS;    // Decimal Adjust AL after Subtraction

    instruction CMPSx;  // Compare String Operands
    instruction LODSx;  // Load String Operand
    instruction MOVSx;  // Move Data from String to String
    instruction SCASx;  // Compare String Data
    instruction STOSx;  // Store String Data

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
    instruction CDQ;    // Convert Word to Doubleword/Convert Doubleword to Quadword
    instruction CLC;    // Clear Carry Flag
    instruction CLD;    // Clear Direction Flag
//  instruction CLI;    // Clear Interrupt Flag
//  instruction CLTS;   // Clear Task-Switched Flag in CR0
    instruction CMC;    // Complement Carry Flag
    instruction CMP;    // Compare Two Operands
    instruction CWDE;   // Convert Byte to Word/Convert Word to Doubleword
    instruction DEC;    // Decrement by 1
    instruction DIV;    // Unsigned Divide
    instruction ENTER;  // Make Stack Frame for Procedure Parameters
//  instruction HLT;    // Halt
    instruction IDIV;   // Signed Divide
    instruction IMUL;   // Signed Multiply
//  instruction IN;     // Input from Port
    instruction INC;    // Increment by 1
//  instruction INS;    // Input from Port to String
//  instruction INT;    // Call to Interrupt Procedure
//  instruction IRET;   // Interrupt Return
    instruction Jcc;    // Jump if Condition is Met
    instruction JMP;    // Jump
    instruction LAHF;   // Load Flags into AH Register
//  instruction LAR;    // Load Access Rights Byte
    instruction LEA;    // Load Effective Address
    instruction LEAVE;  // High Level Procedure Exit
//  instruction LxDT;   // Load Global/Interrupt Descriptor Table Register
//  instruction LxS;    // Load Full Pointer
//  instruction LLDT;   // Load Local Descriptor Table Register
//  instruction LMSW;   // Load Machine Status Word
//  instruction LOCK;   // Assert LOCK# Signal Prefix
    instruction LOOP;   // Loop Control with CX Counter
//  instruction LSL;    // Load Segment Limit
//  instruction LTR;    // Load Task Register
    instruction MOV;    // Move Data
    instruction MOVSX;  // Move with Sign-Extend
    instruction MOVZX;  // Move with Zero-Extend
    instruction MUL;    // Unsigned Multiplication of AL or AX
    instruction NEG;    // Two's Complement Negation
    instruction NOP;    // No Operation
    instruction NOT;    // One's Complement Negation
    instruction OR;     // Logical Inclusive OR
//  instruction OUT;    // Output to Port
//  instruction OUTS;   // Output String to Port
    instruction POP;    // Pop a Word from the Stack
    instruction POPAD;  // Pop all General Registers
    instruction POPFD;  // Pop Stack into FLAGS or EFLAGS Register
    instruction PUSH;   // Push Operand onto the Stack
    instruction PUSHAD; // Push all General Registers
    instruction PUSHFD; // Push Flags Register onto the Stack
    instruction Rxx;    // Rotate
    instruction REP;    // Repeat Following String Operation
    instruction RET;    // Return from Procedure
    instruction SAHF;   // Store AH into Flags
    instruction Sxx;    // Shift Instructions
    instruction SBB;    // Integer Subtraction with Borrow
    instruction SETcc;  // Byte Set on Condition
//  instruction SxDT;   // Store Global/Interrupt Descriptor Table Register
    instruction SHxD;   // Double Precision Shift
//  instruction SLDT;   // Store Local Descriptor Table Register
//  instruction SMSW;   // Store Machine Status Word
    instruction STC;    // Set Carry Flag
    instruction STD;    // Set Direction Flag
//  instruction STI;    // Set Interrupt Flag
//  instruction STR;    // Store Task Register
    instruction SUB;    // Integer Subtraction
    instruction TEST;   // Logical Compare
//  instruction VERx;   // Verify a Segment for Reading or Writing
//  instruction WAIT;   // Wait until BUSY# Pin is Inactive (HIGH)
    instruction XCHG;   // Exchange Register/Memory with Register
    instruction XLAT;   // Table Look-up Translation
    instruction XOR;    // Logical Exclusive OR
};
