/*
Author : Nabil Othman
Date : August 26, 2025
Purpose : Header file with function declarations for bitwise operations
*/

#include <cstdint>

/*
setbit
@brief: sets the bit specified by whichbit at the memory location addr
@params:
    addr: pointer to integer of iterest
    whichbit: bit index to set
@returns:
    void
*/
void setbit(uint32_t* addr, uint8_t whichbit);

/*
clearbit
@brief: clears the bit specified by whichbit at the memory location addr
@params:
    addr: pointer to integer of iterest
    whichbit: bit index to clear
@returns:
    void
*/
void clearbit(uint32_t* addr, uint8_t whichbit);

/*
setbits
@brief: sets bits specified by the mask bitmask at the memory location addr
@params:
    addr: pointer to integer of iterest
    whichbit: bitmask to set 
@returns:
    void
*/
void setbits(uint32_t* addr, uint32_t bitmask);

/*
clearbits
@brief: clears bits specified by the mask bitmask at the memory location addr
@params:
    addr: pointer to integer of iterest
    whichbit: bitmask to clear 
@returns:
    void
*/
void clearbits(uint32_t* addr, uint32_t bitmask);

/*
display_binary
@brief: returns a character string of a the binary representation of an integer
@params:
    num: the number to be converted to a character string of bits
@returns:
    character string of bits
*/
char* display_binary(uint32_t num);


