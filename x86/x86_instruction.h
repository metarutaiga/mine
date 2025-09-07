#pragma once

#include "x86_format.h"
#include "x86_register.h"

struct x86_instruction : public x86_format
                       , public x86_register
{
    x86_instruction& x86 = (*this);

protected:
    static instruction _;

    static instruction CS;          // CS segment override prefix
    static instruction SS;          // SS segment override prefix
    static instruction DS;          // DS segment override prefix
    static instruction ES;          // ES segment override prefix
    static instruction FS;          // FS segment override prefix
    static instruction GS;          // GS segment override prefix

    static instruction REPE;        // Repeat Following String Operation
    static instruction REPNE;       // Repeat Following String Operation
    static instruction WAIT;        // Wait until BUSY# Pin is Inactive (HIGH)

    static instruction AAA;         // ASCII Adjust after Addition
    static instruction AAD;         // ASCII Adjust AX before Division
    static instruction AAM;         // ASCII Adjust AX after Multiply
    static instruction AAS;         // ASCII Adjust AL after Subtraction
    static instruction DAA;         // Decimal Adjust AL after Addition
    static instruction DAS;         // Decimal Adjust AL after Subtraction

    static instruction CMPSx;       // Compare String Operands
    static instruction LODSx;       // Load String Operand
    static instruction MOVSx;       // Move Data from String to String
    static instruction SCASx;       // Compare String Data
    static instruction STOSx;       // Store String Data

    static instruction ADC;         // Add with Carry
    static instruction ADD;         // Add
    static instruction AND;         // Logical AND
    static instruction CALL;        // Call Procedure
    static instruction CBW;         // Convert Byte to Word/Convert Word to Doubleword
    static instruction CLC;         // Clear Carry Flag
    static instruction CLD;         // Clear Direction Flag
//  static instruction CLI;         // Clear Interrupt Flag
    static instruction CMC;         // Complement Carry Flag
    static instruction CMP;         // Compare Two Operands
    static instruction CWD;         // Convert Word to Doubleword/Convert Doubleword to Quadword
    static instruction DEC;         // Decrement by 1
    static instruction DIV;         // Unsigned Divide
//  static instruction HLT;         // Halt
    static instruction IDIV;        // Signed Divide
    static instruction IMUL;        // Signed Multiply
//  static instruction IN;          // Input from Port
    static instruction INC;         // Increment by 1
//  static instruction INT;         // Call to Interrupt Procedure
//  static instruction IRET;        // Interrupt Return
    static instruction Jcc;         // Jump if Condition is Met
    static instruction JMP;         // Jump
    static instruction LAHF;        // Load Flags into AH Register
    static instruction LEA;         // Load Effective Address
//  static instruction LxS;         // Load Full Pointer
//  static instruction LOCK;        // Assert LOCK# Signal Prefix
    static instruction LOOP;        // Loop Control with CX Counter
    static instruction MOV;         // Move Data
    static instruction MUL;         // Unsigned Multiplication of AL or AX
    static instruction NEG;         // Two's Complement Negation
    static instruction NOP;         // No Operation
    static instruction NOT;         // One's Complement Negation
    static instruction OR;          // Logical Inclusive OR
//  static instruction OUT;         // Output to Port
    static instruction POP;         // Pop a Word from the Stack
    static instruction POPF;        // Pop Stack into FLAGS or EFLAGS Register
    static instruction PUSH;        // Push Operand onto the Stack
    static instruction PUSHF;       // Push Flags Register onto the Stack
    static instruction Rxx;         // Rotate
    static instruction RET;         // Return from Procedure
    static instruction SAHF;        // Store AH into Flags
    static instruction Sxx;         // Shift Instructions
    static instruction SBB;         // Integer Subtraction with Borrow
    static instruction STC;         // Set Carry Flag
    static instruction STD;         // Set Direction Flag
//  static instruction STI;         // Set Interrupt Flag
    static instruction SUB;         // Integer Subtraction
    static instruction TEST;        // Logical Compare
    static instruction XCHG;        // Exchange Register/Memory with Register
    static instruction XLAT;        // Table Look-up Translation
    static instruction XOR;         // Logical Exclusive OR

    // 80186 - 1982
//  static instruction BOUND;       // Check Array Index Against Bounds
    static instruction ENTER;       // Make Stack Frame for Procedure Parameters
//  static instruction INS;         // Input from Port to String
    static instruction LEAVE;       // High Level Procedure Exit
//  static instruction OUTS;        // Output String to Port
    static instruction POPA;        // Pop all General Registers
    static instruction PUSHA;       // Push all General Registers

    // 80286 - 1982
//  static instruction ARPL;        // Adjust RPL Field of Selector
//  static instruction CLTS;        // Clear Task-Switched Flag in CR0
//  static instruction LAR;         // Load Access Rights Byte
//  static instruction LLDT;        // Load Local Descriptor Table Register
//  static instruction LMSW;        // Load Machine Status Word
//  static instruction LSL;         // Load Segment Limit
//  static instruction LTR;         // Load Task Register
//  static instruction LxDT;        // Load Global/Interrupt Descriptor Table Register
//  static instruction SLDT;        // Store Local Descriptor Table Register
//  static instruction SMSW;        // Store Machine Status Word
//  static instruction STR;         // Store Task Register
//  static instruction SxDT;        // Store Global/Interrupt Descriptor Table Register
//  static instruction VERx;        // Verify a Segment for Reading or Writing

    // 80386 - 1985
    static instruction BSF;         // Bit Scan Forward
    static instruction BSR;         // Bit Scan Reverse
    static instruction BT;          // Bit Test
    static instruction BTC;         // Bit Test and Complement
    static instruction BTR;         // Bit Test and Reset
    static instruction BTS;         // Bit Test and Set
    static instruction MOVSX;       // Move with Sign-Extend
    static instruction MOVZX;       // Move with Zero-Extend
    static instruction SETcc;       // Byte Set on Condition
    static instruction SHxD;        // Double Precision Shift

    // 80486 - 1989
    static instruction BSWAP;       // Byte Swap
    static instruction CMPXCHG;     // Compare and Exchange
    static instruction XADD;        // Exchange and Add

    // Pentium - 1993
    static instruction CMPXCHG8B;   // Compare and Exchange Bytes
//  static instruction CPUID;       // CPU Identification
    static instruction RDTSC;       // Read Time-Stamp Counter

    // Pentium Pro - 1995
    static instruction CMOVcc;      // Conditional Move
    static instruction RDPMC;       // Read Performance-Monitoring Counters
};
