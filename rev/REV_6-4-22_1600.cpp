/*

0 | NOP (No operation)
1 | MVR (Move register -> MVN #[reg], #[reg]/$[addr]/[value]) -> [reg] = [reg/addr/value]
2 | MVA (Move address -> MVA $[addr], #[reg]/$[addr]/[value]) -> [addr] = [reg/value]
3 | ADD (Add -> ADD [one], [two])                             -> R = one + two
4 | ADC (Add with carry -> ADC [one], [two])                  -> R = one + two + c
5 | SBB (Subtract with borrow -> SBB [one]. [two])            -> R = one - two - c
6 | PUSH (Push to stack -> PUSH #[reg]/[value]/$[addr])       -> SP += 1; stack[ SP ] = [reg/addr/value]
7 | POP (Pop from stack -> POP #[reg]/$[addr])                -> {reg/addr] = stack[ SP ]; SP -= 1
8 | AND (Bitwise AND -> AND [one], [two])                     -> R = one & two
9 | OR (Bitwise OR -> OR [one], [two])                        -> R = one | two
A | NOR (Bitwise NOR -> NOR [one], [two])                     -> R = ~( one | two )
B | IN (In port -> IN #[reg]/$[addr], [port])                 -> [reg/addr] = PORT
C | OUT (Out port -> OUT [port], #[reg]/$[addr]/[value])      -> PORT = [reg/addr/value]
D | CMP (Compare -> CMP [one], [two], ^[condition])           -> (perform analysis on elements to determine what should be done, put result in R register)
E | JNZ (Jump if not zero -> JNZ [label])                     -> PC = R != 0 ? [label] : PC
F | NULL

! | Invalid
@ | Pre-processor directive
# | Register
$ | Memory address
% | Variable (used in macro definitions)
^ | Comparison operator
& | Invalid
* | Invalid
( | Begin arithmetic expression
) | End arithmetic expression
; | Comment (semi-colon)
: | Label (colon, at end)

X  | (G)eneral (P)urpose register
Y  | GP register
Z  | GP register
PC | (P)rogram (C)ounter
SP | (S)tack (P)ointer
C  | (C)arry flag
R  | Arithmetic expression (R)esult register
SR | (S)tatus (R)egister

*/

#include<iostream>
#include<string>

#define MEMORY_ADDRESS long long
#define JE  1
#define JGE 2
#define JL  3
#define JLE 4
#define JZ  5

typedef struct {
    int value;
} Reg_t;
typedef struct {
    int entries[ 127 ];
} Stack_t;
typedef struct {
    int val_in;
    int val_out;
} Port_t;

// NOP
bool ASM_NOP( void );
// MVR
template< typename GLOBAL >
bool ASM_MVR( Reg_t* REG_INTO, GLOBAL FROM );
// MVA
template< typename GLOBAL >
bool ASM_MVA( MEMORY_ADDRESS INTO,  GLOBAL FROM );
// ADD
template< typename GLOBAL >
bool ASM_ADD( GLOBAL ONE, GLOBAL TWO );
// ADC - disabled
template< typename GLOBAL >
bool ASM_ADC( GLOBAL ONE, GLOBAL TWO );
// SBB
template< typename GLOBAL >
bool ASM_SBB( GLOBAL ONE, GLOBAL TWO );
// PUSH
template< typename GLOBAL >
bool ASM_PUSH( GLOBAL PUSHVAL );
// POP
template< typename GLOBAL >
bool ASM_POP( GLOBAL INTO );
// AND
template< typename GLOBAL >
bool ASM_AND( GLOBAL ONE, GLOBAL TWO );
// OR
template< typename GLOBAL >
bool ASM_OR( GLOBAL ONE, GLOBAL TWO );
// NOR
template< typename GLOBAL >
bool ASM_NOR( GLOBAL ONE, GLOBAL TWO );
// IN
template< typename GLOBAL >
bool ASM_IN( GLOBAL INTO, int PORT );
// OUT
template< typename GLOBAL >
bool ASM_OUT( int PORT, GLOBAL FROM );
// CMP
template< typename GLOBAL >
bool ASM_CMP( GLOBAL ONE, GLOBAL TWO, int CONDITION );
// JNZ
bool ASM_JNZ( std::string LABEL );

Reg_t* REG_X  = { 0 };
Reg_t* REG_Y  = { 0 };
Reg_t* REG_Z  = { 0 };
Reg_t* REG_PC = { 0 };
Reg_t* REG_SP = { 0 };
Reg_t* REG_C  = { 0 };
Reg_t* REG_R  = { 0 };
Reg_t* REG_SR = { 0 };

Stack_t* STACK = { 0 };

Port_t* PORTS[ 20 ] = { 0 };

MEMORY_ADDRESS MEMORY[ 65535 ] = { 0 };

int main( void ) {
    std::string line1 = "mvr #x, 10";
    std::string line2 = "add #x, 5";
    std::string line3 = "push #x";
    return 0;
}
// NOP
bool ASM_NOP( void ) {
    return true;
}
// MVR
template< typename GLOBAL >
bool ASM_MVR( Reg_t* INTO, GLOBAL FROM ) {
    INTO->value = FROM;
    return true;
}
// MVA
template< typename GLOBAL >
bool ASM_MVA( MEMORY_ADDRESS INTO,  GLOBAL FROM ) {
    MEMORY[ INTO ] = FROM;
    return true;
}
// ADD
template< typename GLOBAL >
bool ASM_ADD( GLOBAL ONE, GLOBAL TWO ) {
    REG_R->value = ONE + TWO;
    return true;
}
// ADC - disabled
template< typename GLOBAL >
bool ASM_ADC( GLOBAL ONE, GLOBAL TWO ) {
    return false;
}
// SBB
template< typename GLOBAL >
bool ASM_SBB( GLOBAL ONE, GLOBAL TWO ) {
    REG_R->value = ONE - TWO;
    return true;
}
// PUSH
template< typename GLOBAL >
bool ASM_PUSH( GLOBAL PUSHVAL ) {
    if( STACK->entries[ 0 ] > 0 ) {
        STACK->entries[ 0 ] = PUSHVAL;
        REG_SP->value += 1;
    } else {
        REG_SP->value += 1;
        STACK->entries[ REG_SP->value ] = PUSHVAL;
    }
    return true;
}
// POP
template< typename GLOBAL >
bool ASM_POP( GLOBAL INTO ) {
    if( REG_SP->value = 0 ) {
        return false;
    } else {
        INTO = STACK->entries[ REG_SP->value ];
        REG_SP->value -= 1;
    }
    return true;
}
// AND
template< typename GLOBAL >
bool ASM_AND( GLOBAL ONE, GLOBAL TWO ) {
    REG_R->value = ONE & TWO;
    return true;
}
// OR
template< typename GLOBAL >
bool ASM_OR( GLOBAL ONE, GLOBAL TWO ) {
    REG_R->value = ONE | TWO;
    return true;
}
// NOR
template< typename GLOBAL >
bool ASM_NOR( GLOBAL ONE, GLOBAL TWO ) {
    REG_R->value = ~( ONE | TWO );
    return true;
}
// IN
template< typename GLOBAL >
bool ASM_IN( GLOBAL INTO, int PORT ) {
    INTO = PORTS[ PORT ]->val_in;
    return true;
}
// OUT
template< typename GLOBAL >
bool ASM_OUT( int PORT, GLOBAL FROM ) {
    PORTS[ PORT ]->val_out = FROM;
    return true;
}
// CMP - TODO implement
template< typename GLOBAL >
bool ASM_CMP( GLOBAL ONE, GLOBAL TWO, int CONDITION ) {
    return false;
}
// JNZ - TODO implement
bool ASM_JNZ( std::string LABEL ) {
    if( REG_R->value != 0 ) {
        // figure out PC label swapping
        return false;
    }
    return false;
}
