/**
 * @file    hamming84.c
 * @brief   Implementation and test of an extended [8,4]-Hamming code.
 * @details Lab DKK-04 "Hamming code",
 *          lecture "Digitale Kodier- und Kompressionsverfahren",
 *          University of Applied Sciences Zwickau.
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


typedef union 
{
    uint_least8_t whole:3;
    struct bits
    {
        uint_least8_t s0:1;
        uint_least8_t s1:1;
        uint_least8_t s2:1;
    } bits;
} syndrome;

typedef union 
{
    uint_least8_t whole;
    struct bits
    {
        uint_least8_t d0:1;
        uint_least8_t d1:1;
        uint_least8_t d2:1;
        uint_least8_t d3:1;
        uint_least8_t p0:1;
        uint_least8_t p1:1;
        uint_least8_t p2:1;
        uint_least8_t parity:1;
    } bits;
    struct block {
        uint_least8_t data:4;
        uint_least8_t parityBits:3;
        uint_least8_t parity:1;
    } block;
} ham84Syst;

/**
 * @brief   Encode a four-bit data word using a systematic extended [8,4] Hamming code.
 * @param   data  Data to be encoded. May only contain four data bits, starting with LSB.
 * @return  Systematic code word. First four bits are copied from data, followed by four parity bits.
 */
uint_least8_t hamming84_encode(uint_least8_t data)
{
    ham84Syst retVal;

    retVal.block.data = data;

    retVal.bits.p0 = retVal.bits.d0 ^ retVal.bits.d1 ^ retVal.bits.d3;
    retVal.bits.p1 = retVal.bits.d0 ^ retVal.bits.d2 ^ retVal.bits.d3;
    retVal.bits.p2 = retVal.bits.d1 ^ retVal.bits.d2 ^ retVal.bits.d3;

    retVal.bits.parity = retVal.bits.p0 ^ retVal.bits.p1 ^ retVal.bits.p2;

    return retVal.whole;
    return 0;
}


/**
 * @brief   "Decode" an extended [8,4] Hamming code word, i.e. single-error correction, double-error detection.
 * @param   encoded_data  Systematic code word to be decoded/corrected. Four data bits (starting with LSB) and four parity bits.
 * @return  Code word with error correction applied (if any one-bit error was present),
 *          or special value 0xFFFF if a two-bit error was detected.
 */
uint16_t hamming84_decode(uint_least8_t encoded_data)
{
    ham84Syst inDataSys;
    syndrome syndromeBits;
    int parity = 0;
    uint16_t retVal;

    inDataSys.whole = encoded_data;

    syndromeBits.bits.s0 = inDataSys.bits.p0 ^ inDataSys.bits.d0 ^ inDataSys.bits.d1 ^ inDataSys.bits.d3;
    syndromeBits.bits.s1 = inDataSys.bits.p1 ^ inDataSys.bits.d0 ^ inDataSys.bits.d2 ^ inDataSys.bits.d3;
    syndromeBits.bits.s2 = inDataSys.bits.p2 ^ inDataSys.bits.d1 ^ inDataSys.bits.d2 ^ inDataSys.bits.d3;

    parity =  inDataSys.bits.p0 ^ inDataSys.bits.p1 ^ inDataSys.bits.p2 ^ inDataSys.bits.d0 ^ inDataSys.bits.d1 ^ inDataSys.bits.d2 ^ inDataSys.bits.d3;

    if (((inDataSys.bits.parity != parity) && (syndromeBits.whole == 0))||((inDataSys.bits.parity == parity) && (syndromeBits.whole != 0)))
    {
        retVal = 0xFFFF;
        return retVal;
    }
    else if (inDataSys.bits.parity != parity)
    {
        inDataSys.bits.parity ^= 1;
        retVal = inDataSys.whole;
        return retVal;
    }
    if (syndromeBits.whole != 0)
    {
        switch (syndromeBits.whole)
        {
        case 1:
            inDataSys.bits.p0 ^= 1;
            break;
        case 2:
            inDataSys.bits.p1 ^= 1;
            break;
        case 3:
            inDataSys.bits.d0 ^= 1;
            break;
        case 4:
            inDataSys.bits.p2 ^= 1;
            break;
        case 5:
            inDataSys.bits.d1 ^= 1;
            break;
        case 6:
            inDataSys.bits.d2 ^= 1;
            break;
        case 7:
            inDataSys.bits.d3 ^= 1;
            break;

        default:
            break;
        }
    }
    
    {
        retVal = inDataSys.whole;
    }

    return retVal;
}


/**
 * @brief   Perform round-trip checks on a given data word to test the Hamming code implementation.
 * @param   data  Data to be encoded. May only contain four data bits, starting with LSB.
 * @return  True if all checks passed.
 */
bool checks_passed(uint_least8_t data)
{
    bool result = true;
    int temp = 0;
    uint_least8_t encoded = hamming84_encode(data);

    // check: data still contained in first four bits
    result = result && ((data & 0x0F) == (encoded & 0x0F));

    // check: bit-manipulated code word must be successfully decoded
    for (uint_least8_t k = 0; k < 8; ++k) {
        // one error (bit-flip) at a time
        uint_least8_t manipulated = (encoded ^ (1 << k));
        result = result && (hamming84_decode(manipulated) == encoded);
        temp ^= 1;
    }

    // check: all two-bit errors must be detected
    for (uint_least8_t x = 0; x < 7; ++x) {
        for (uint_least8_t y = x+1; y < 8; ++y) {
            // flip two bits
            uint_least8_t manipulated = (encoded ^ ((1 << x) | (1 << y)));
            result = result && (hamming84_decode(manipulated) == 0xFFFF);
            temp ^= 1;
        }
    }

    return result;
}


/**
 * @brief   Main program entry point.
 */
int main()
{
    printf("Implementation and test of a [8,4] Hamming code\n");

    uint_least8_t num_checks_failed = 0;
    for (uint_least8_t data = 0; data < 0x0F; ++data) {
        if (checks_passed(data) == false) {
            printf("Checks not passed for %02x\n", data);
            ++num_checks_failed;
        }
    }

    if (num_checks_failed == 0) {
        printf("All checks passed!\n");
    }

    return 0;
}
