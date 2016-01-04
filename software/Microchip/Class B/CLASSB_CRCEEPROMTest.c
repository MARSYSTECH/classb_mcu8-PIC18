/**********************************************************************
* © 2013 - 2014 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB.h
* Dependencies:     CLASSB_config.h
* Processor:        PIC18F
* Compiler:         XC8
* IDE:              MPLAB® IDE or MPLAB® X
* Rev				3.0
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Description:     Header file for CLASS B Library.
*
**************************************************************************
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER: You may use this software, and
 * any derivatives created by any person or entity by or on your behalf,
 * exclusively with Microchip's products in accordance with applicable
 * software license terms and conditions, a copy of which is provided for
 * your referencein accompanying documentation. Microchip and its licensors
 * retain all ownership and intellectual property rights in the
 * accompanying software and in all derivatives hereto.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products. You
 * agree that you are solely responsible for testing the software and
 * determining its suitability. Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH
 * MICROCHIP'S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY,
 * TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT
 * LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
 * FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE,
 * HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY
 * OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWABLE BY LAW,
 * MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
 * SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID
 * DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *************************************************************************
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date        Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Corey Simoncic    04/19/2013  Updated for XC-8
* Willem J Smit		09/09/2014  Updated clock tests (REV 3.0)
*
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ADDITIONAL NOTES:
*
**********************************************************************/

#include "CLASSB_CRCEEPROMTest.h"
#include "CLASSB_Typedef.h"

unsigned int crcreg;            //the current remainder of the CRC calculation
unsigned char parity;           //the parity bit for CRC calculation
unsigned char dataXorCrcHigh;   //Data XOR'd with the current CRC High byte

  /********************************************************************
  * Description:
  *     This function computes the periodic checksum using the Cyclic Redundancy
  *     Check (CRC). It detects the single bit Faults in the invariable memory.
  *     This function returns the final CRC Value.
  *     CRC-16 is used as a divisor.
  *
  *     CRC-16 = 1 1000 0000 0000 0101= 8005(hex)
  * Input:
  *     startAddress :  start Address from which the CRC needs to be calculated
  *     length       :  the number of EEPROM addresses to be tested
  *     crcSeed      :  the initial value of the CRC calculation
  *
  * Return Values:
  *     testResult   :  Returns the final CRC result.
  ********************************************************************/
uint16_t CLASSB_CRCEEPROMTest(uint16_t startAddress, uint16_t length, uint16_t crcSeed)
{
    volatile unsigned int testResult;
    uint16_t i;
    uint8_t dataSequence;
    crcreg = crcSeed;

    for(i=0; i<length; i++)
    {
        EEADR = (startAddress + i);
        EEADRH = (startAddress + i)>>8;

        EECON1bits.CFGS = 0;    // Do not select Configuration Space
        EECON1bits.EEPGD = 0;   // Do select Program Memory
        EECON1bits.RD = 1;      // Initiate read

        dataSequence= EEDATA;                                    // put the eeprom data byte in buffer
        
        crcByte_EEPROM(dataSequence);       
    }
    testResult = crcreg;
    return testResult;
    
}

/******************************************************************************
* Description:
*     This function does the CRC calculation of a single byte.
* Input:
*     dataSequence :  the byte to be tested
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

C is current crc value (seed or previous iteration)
D is the current data sequence
S is the data sequence XOR'd with the high byte of the current crc value
P is the parity bit

S[i] = C8^D0, C9^D1, C10^D2, C11^D3... etc

parity = C8^C9^C10^C11^C12^C13^C14^C15 ^ D7^D6^D5^D4^D3^D2^D1^D0

crcreg[0]  =      P
crcreg[1]  =      S0 ^ P
crcreg[2]  =      S1 ^ S0
crcreg[3]  =      S2 ^ S1
crcreg[4]  =      S3 ^ S2
crcreg[5]  =      S4 ^ S3
crcreg[6]  =      S5 ^ S4
crcreg[7]  =      S6 ^ S5
crcreg[8]  = C0 ^ S7 ^ S6
crcreg[9]  = C1      ^ S7
crcreg[10] = C2
crcreg[11] = C3
crcreg[12] = C4
crcreg[13] = C5
crcreg[14] = C6
crcreg[15] = C7 ^ P
******************************************************************************/
void crcByte_EEPROM(uint8_t dataSequence)
{
    WREG = dataSequence;

    #asm
    banksel BANKMASK(_crcreg)
    xorwf   BANKMASK(_crcreg)+1,w           //XOR the upper byte of CRCREG
                                            //with the input data sequence
                                            //This creates S[i]

    movwf   BANKMASK(_dataXorCrcHigh)
    movwf   BANKMASK(_parity)

    //The following section finds the parity of the CRCREG and the Data sequence
    swapf   BANKMASK(_parity),w
    xorwf   BANKMASK(_parity),f         //S0^S4, S1^S5... etc
    BCF     STATUS,0
    RRCF    BANKMASK(_parity),w         //the upper 4 bits are unnecessary
    BCF     STATUS,0
    RRCF    WREG
    BCF     STATUS,0
    xorwf   BANKMASK(_parity),f         //S0^S4^S1^S5, S2^S6^S3^S7
    RRCF    BANKMASK(_parity),w         //the upper 6 bits are unnecessary
    xorwf   BANKMASK(_parity),f         //P = S0^S4^S1^S5^S2^S6^S3^S7
                                        //the upper 7 bits are unnecessary

    BCF     STATUS,0
    RLCF    BANKMASK(_dataXorCrcHigh),w
    xorwf   BANKMASK(_dataXorCrcHigh),f     //S0,S1^S0,S2^S1...etc

    movf    BANKMASK(_crcreg),w         //Puts the lower byte of the CRC into W
                                        //use this to solve for the upper byte
    btfsc   BANKMASK(_parity),0
    xorlw   80h                         //0x80 is used to position the parity bit
                                        //on C7 if it is high. Solves for C15

    btfsc   STATUS,0                    //The Carry bit is currently S7
    xorlw   02h                         //0x02 positions S7 on C1 if it is high
                                        //Solves for C9

    btfsc   BANKMASK(_dataXorCrcHigh),7 //S7 currently holds S7^S6
    xorlw   01h                         //0x01 positions S7^S6 on C0
                                        //Solves for C8

    movwf   BANKMASK(_crcreg)+1         //Saves the high byte of CRCREG

    BCF     STATUS,0
    RLCF    BANKMASK(_dataXorCrcHigh),w //This shifts S[i] over to position it
                                        //on C1:C7
    btfsc   BANKMASK(_parity),0
    xorlw   03h                         //0x03 is used to position the parity bit
                                        //on C0 and C1. Solves for C0:C7

    movwf   BANKMASK(_crcreg)           //Saves the low byte of CRCREG

#endasm
}