/**
 * @file    hamming74.c
 * @brief   Implementation and test of a [7,4]-Hamming code.
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
        uint_least8_t reserved:1;
    } bits;
    struct block {
        uint_least8_t data:4;
        uint_least8_t parity:3;
        uint_least8_t reserved:1;
    } block;
} ham47Syst;

/*
typedef union ham47NonSyst 
{
    uint_least8_t whole;
    struct bits
    {
        uint_least8_t p0:1;
        uint_least8_t p1:1;
        uint_least8_t d0:1;
        uint_least8_t p2:1;
        uint_least8_t d1:1;
        uint_least8_t d2:1;
        uint_least8_t d3:1;
        uint_least8_t reserved:1;
    } bits;
} ham47NonSyst;
*/

/**
 * @brief   Encode a four-bit data word using a systematic [7,4] Hamming code.
 * @param   data  Data to be encoded. May only contain four data bits, starting with LSB.
 * @return  Systematic code word. First four bits are copied from data, followed by three parity bits.
 */
uint_least8_t hamming74_encode(uint_least8_t data)
{
    ham47Syst retVal;

    retVal.block.data = data;

    retVal.bits.p0 = retVal.bits.d0 ^ retVal.bits.d1 ^ retVal.bits.d3;
    retVal.bits.p1 = retVal.bits.d0 ^ retVal.bits.d2 ^ retVal.bits.d3;
    retVal.bits.p2 = retVal.bits.d1 ^ retVal.bits.d2 ^ retVal.bits.d3;

    return retVal.whole;
}


/**
 * @brief   "Decode" a [7,4] Hamming code word, i.e. detect and correct possible one-bit errors.
 * @param   encoded_data  Systematic code word to be decoded/corrected. Four data bits (starting with LSB) and three parity bits.
 * @return  Code word with error correction applied (if any one-bit error was present).
 */
uint_least8_t hamming74_decode(uint_least8_t encoded_data)
{
    ham47Syst inDataSys;
    //ham47NonSyst inDataNonSys;
    //syndrome tempParity;
    syndrome syndromeBits;

    inDataSys.whole = encoded_data;

    syndromeBits.bits.s0 = inDataSys.bits.p0 ^ inDataSys.bits.d0 ^ inDataSys.bits.d1 ^ inDataSys.bits.d3;
    syndromeBits.bits.s1 = inDataSys.bits.p1 ^ inDataSys.bits.d0 ^ inDataSys.bits.d2 ^ inDataSys.bits.d3;
    syndromeBits.bits.s2 = inDataSys.bits.p2 ^ inDataSys.bits.d1 ^ inDataSys.bits.d2 ^ inDataSys.bits.d3;

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

    return inDataSys.whole;
}


/**
 * @brief   Perform round-trip checks on a given data word to test the Hamming code implementation.
 * @param   data  Data to be encoded. May only contain four data bits, starting with LSB.
 * @return  True if all checks passed.
 */
bool checks_passed(uint_least8_t data)
{
    bool result = true;
    uint_least8_t encoded = hamming74_encode(data);

    // check: data still contained in first four bits
    result = result && ((data & 0x0F) == (encoded & 0x0F));

    // check: no bit set above first seven bits
    result = result && (encoded < 0x80);

    // check: bit-manipulated code word must be successfully decoded
    for (uint_least8_t k = 0; k < 7; ++k) {
        // one error (bit-flip) at a time
        uint_least8_t manipulated = (encoded ^ (1 << k));
        result = result && (hamming74_decode(manipulated) == encoded);
    }

    return result;
}


/**
 * @brief   Main program entry point.
 */
int main()
{
    printf("Implementation and test of a [7,4] Hamming code\n");

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
}
