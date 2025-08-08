#pragma once

#include <string>

#include "x86_format.h"
#include "x86_register.h"

struct x86_instruction : public x86_format
                       , public x86_register
{
    x86_instruction& x86 = (*this);

    size_t memory_size = 0;
    uint8_t* memory = nullptr;
    uint8_t* opcode = nullptr;
    uint8_t* stack = nullptr;

    void (*exception)(size_t index, void* memory, void* stack) = [](size_t, void*, void*){};

protected:

    enum
    {
        OPERAND_SIZE    = 0b000001,
        DIRECTION       = 0b000010,
        IMMEDIATE       = 0b000100,
        INDIRECT        = 0b001000,
        RELATIVE        = 0b010000,
        THREE_OPERAND   = 0b100000,
    };

    static void         Decode(Format& format, const uint8_t* opcode, const char* instruction, int offset = 0, int immediate_size = 0, int flags = 0);
    static std::string  Disasm(const Format& format, x86_instruction& x86);
    static void         Fixup(Format& format, x86_instruction& x86);

    template<int O, int S, int Z, int A, int P, int C, typename L, typename R>
    static void UpdateFlags(x86_instruction& x86, L& DEST, R TEMP);

    template<int O, int S, int Z, int A, int P, int C, typename L, typename R, typename X, typename Y>
    static void UpdateFlags(x86_instruction& x86, L& DEST, R TEMP, X SRC1, Y SRC2);

protected:
    typedef void instruction(Format&, const uint8_t*);
    typedef void (*instruction_pointer)(Format&, const uint8_t*);

    static instruction _;

    static instruction CS;      // CS segment override prefix
    static instruction SS;      // SS segment override prefix
    static instruction DS;      // DS segment override prefix
    static instruction ES;      // ES segment override prefix
    static instruction FS;      // FS segment override prefix
    static instruction GS;      // GS segment override prefix

    static instruction AAA;     // ASCII Adjust after Addition
    static instruction AAD;     // ASCII Adjust AX before Division
    static instruction AAM;     // ASCII Adjust AX after Multiply
    static instruction AAS;     // ASCII Adjust AL after Subtraction
    static instruction DAA;     // Decimal Adjust AL after Addition
    static instruction DAS;     // Decimal Adjust AL after Subtraction

    static instruction CMPSx;   // Compare String Operands
    static instruction LODSx;   // Load String Operand
    static instruction MOVSx;   // Move Data from String to String
    static instruction SCASx;   // Compare String Data
    static instruction STOSx;   // Store String Data

    static instruction ADC;     // Add with Carry
    static instruction ADD;     // Add
    static instruction AND;     // Logical AND
//  static instruction ARPL;    // Adjust RPL Field of Selector
//  static instruction BOUND;   // Check Array Index Against Bounds
    static instruction BSF;     // Bit Scan Forward
    static instruction BSR;     // Bit Scan Reverse
    static instruction BT;      // Bit Test
    static instruction BTC;     // Bit Test and Complement
    static instruction BTR;     // Bit Test and Reset
    static instruction BTS;     // Bit Test and Set
    static instruction CALL;    // Call Procedure
    static instruction CDQ;     // Convert Word to Doubleword/Convert Doubleword to Quadword
    static instruction CLC;     // Clear Carry Flag
    static instruction CLD;     // Clear Direction Flag
//  static instruction CLI;     // Clear Interrupt Flag
//  static instruction CLTS;    // Clear Task-Switched Flag in CR0
    static instruction CMC;     // Complement Carry Flag
    static instruction CMP;     // Compare Two Operands
    static instruction CWDE;    // Convert Byte to Word/Convert Word to Doubleword
    static instruction DEC;     // Decrement by 1
    static instruction DIV;     // Unsigned Divide
    static instruction ENTER;   // Make Stack Frame for Procedure Parameters
//  static instruction HLT;     // Halt
    static instruction IDIV;    // Signed Divide
    static instruction IMUL;    // Signed Multiply
//  static instruction IN;      // Input from Port
    static instruction INC;     // Increment by 1
//  static instruction INS;     // Input from Port to String
//  static instruction INT;     // Call to Interrupt Procedure
//  static instruction IRET;    // Interrupt Return
    static instruction Jcc;     // Jump if Condition is Met
    static instruction JMP;     // Jump
    static instruction LAHF;    // Load Flags into AH Register
//  static instruction LAR;     // Load Access Rights Byte
    static instruction LEA;     // Load Effective Address
    static instruction LEAVE;   // High Level Procedure Exit
//  static instruction LxDT;    // Load Global/Interrupt Descriptor Table Register
//  static instruction LxS;     // Load Full Pointer
//  static instruction LLDT;    // Load Local Descriptor Table Register
//  static instruction LMSW;    // Load Machine Status Word
//  static instruction LOCK;    // Assert LOCK# Signal Prefix
    static instruction LOOP;    // Loop Control with CX Counter
//  static instruction LSL;     // Load Segment Limit
//  static instruction LTR;     // Load Task Register
    static instruction MOV;     // Move Data
    static instruction MOVSX;   // Move with Sign-Extend
    static instruction MOVZX;   // Move with Zero-Extend
    static instruction MUL;     // Unsigned Multiplication of AL or AX
    static instruction NEG;     // Two's Complement Negation
    static instruction NOP;     // No Operation
    static instruction NOT;     // One's Complement Negation
    static instruction OR;      // Logical Inclusive OR
//  static instruction OUT;     // Output to Port
//  static instruction OUTS;    // Output String to Port
    static instruction POP;     // Pop a Word from the Stack
    static instruction POPAD;   // Pop all General Registers
    static instruction POPFD;   // Pop Stack into FLAGS or EFLAGS Register
    static instruction PUSH;    // Push Operand onto the Stack
    static instruction PUSHAD;  // Push all General Registers
    static instruction PUSHFD;  // Push Flags Register onto the Stack
    static instruction Rxx;     // Rotate
    static instruction REP;     // Repeat Following String Operation
    static instruction RET;     // Return from Procedure
    static instruction SAHF;    // Store AH into Flags
    static instruction Sxx;     // Shift Instructions
    static instruction SBB;     // Integer Subtraction with Borrow
    static instruction SETcc;   // Byte Set on Condition
//  static instruction SxDT;    // Store Global/Interrupt Descriptor Table Register
    static instruction SHxD;    // Double Precision Shift
//  static instruction SLDT;    // Store Local Descriptor Table Register
//  static instruction SMSW;    // Store Machine Status Word
    static instruction STC;     // Set Carry Flag
    static instruction STD;     // Set Direction Flag
//  static instruction STI;     // Set Interrupt Flag
//  static instruction STR;     // Store Task Register
    static instruction SUB;     // Integer Subtraction
    static instruction TEST;    // Logical Compare
    static instruction UD;      // Undefined Instruction
//  static instruction VERx;    // Verify a Segment for Reading or Writing
//  static instruction WAIT;    // Wait until BUSY# Pin is Inactive (HIGH)
    static instruction XCHG;    // Exchange Register/Memory with Register
    static instruction XLAT;    // Table Look-up Translation
    static instruction XOR;     // Logical Exclusive OR
};
