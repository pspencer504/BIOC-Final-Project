#include "NOthman_binaryutils.hpp"
// #inclde "USBSerial.h"

void setbit(uint32_t* addr, uint8_t whichbit)
{
    *addr |= (1<<whichbit);
}
void clearbit(uint32_t* addr, uint8_t whichbit)
{
    *addr &= ~(1<<whichbit);

}
void setbits(uint32_t* addr, uint32_t bitmask)
{

    *addr |= bitmask;

}
void clearbits(uint32_t* addr, uint32_t bitmask)
{
    *addr &= ~bitmask;
}
char* display_binary(uint32_t num)
{
    int i = 0;
    int MSB_idx = 32;
    uint32_t mask = 1 << (MSB_idx - 1);
    static char str[32];
    for(i = MSB_idx; i > 0; i--)
    {
        str[MSB_idx - i] = (num & mask) ? '1' : '0';
        mask = mask >> 1;
    }
    // str[31] = '\0';
    return str;

}