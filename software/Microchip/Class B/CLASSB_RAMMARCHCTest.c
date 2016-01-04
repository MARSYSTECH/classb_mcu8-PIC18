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
                             
#include "CLASSB_RAMMARCHCTest.h"

#ifndef MARCH_VARS
#define MARCH_VARS
unsigned char* CLASSB_MarchstartAddress @ 0x60;
unsigned char* CLASSB_MarchbufferAddress @ 0x62;
unsigned char* CLASSB_MARCHtempAddress @ 0x64;
unsigned int CLASSB_MarchIndex @ 0x66 = 0;
unsigned int CLASSB_MarchLength @ 0x68 = 0;
#endif

/******************************************************************************
* Description  : This function implements the March C or March C Minus test.
* It is a complete and irredundant test capable of detecting stuck-at faults,
* addressing faults, transition faults, and coupling faults. This test is of complexity
* 11n (Where n is the number of bits), 10n for March C Minus test.
*
* This test is a destructive test!!! CLASSB_MarchbufferAddress is used as a an array to save user
* memory during the test!
*
* Input:
*     CLASSB_MarchstartAddress     :  pointer to an array declared in user memory for test
*     CLASSB_MarchLength            :  value indicating the number of bytes to test
*     CLASSB_MarchbufferAddress    :  pointer to location in memory where array will be stored during test
*     MARCHCMINUS                   :  designates the test is a March C Minus test.
* Return Values:
*     CLASSB_TEST_PASS 	:  return value = 0
*     CLASSB_TEST_FAIL 	:  return value = 1
*
******************************************************************************/
CLASSBRESULT CLASSB_RAMMarchCTest()
{
    volatile unsigned char testResult;

/******************************************************************************
 * Save memory for test in a known location designated by *CLASSB_MarchbufferAddress
 ******************************************************************************/
    if (CLASSB_MarchbufferAddress != NULL)
    memcpy(CLASSB_MarchbufferAddress, CLASSB_MarchstartAddress, CLASSB_MarchLength);

/******************************************************************************
 * Run March C Test on global RAM used by March C test.
 ******************************************************************************/
    if (CLASSB_MarchstartAddress < (char*)0x6A)
    {
        testResult = MarchCGlobalTest();
        if (testResult)
        {
            MarchCFailure();
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchLength -= (0x6B - (char)CLASSB_MarchstartAddress);
        CLASSB_MarchstartAddress = (char*)0x6A;
    }

/******************************************************************************
 * Wr0 - write zero to all memory for test
 ******************************************************************************/
    CLASSB_MARCHtempAddress = CLASSB_MarchstartAddress;
    for (CLASSB_MarchIndex=0; CLASSB_MarchIndex<CLASSB_MarchLength; CLASSB_MarchIndex++)
    {
        *CLASSB_MarchstartAddress=0x00;
        CLASSB_MarchstartAddress++;
    }

/******************************************************************************
 * Rd0Wr1 - read zero, write one - ascending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (CLASSB_MarchIndex=0; CLASSB_MarchIndex<CLASSB_MarchLength; CLASSB_MarchIndex++)
    {
        PRODL = 0x00;
        for(TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {

#asm
        MOVFF   _CLASSB_MarchstartAddress, FSR1L

        MOVFF   _CLASSB_MarchstartAddress+1, FSR1H

        MOVF    INDF1, W
        XORWF   PRODL, W
        MOVLW   0x01
        BTFSS   STATUS, 0x02
        MOVWF   TBLPTRH

        MOVF    INDF1, W
        BSF     STATUS, 0x00
        RLCF    PRODL, F
        IORWF   PRODL, W
        MOVWF   INDF1
#endasm
        if (TBLPTRH)
        return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;
    }

/******************************************************************************
 * Rd1Wr0 - read one, write zero - ascending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (CLASSB_MarchIndex=0; CLASSB_MarchIndex<CLASSB_MarchLength; CLASSB_MarchIndex++)
    {
        PRODL = 0xFF;
        for(TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {

            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L
 
            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H
  
            MOVF    INDF1, W
            XORWF   PRODL, W
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVF    INDF1, W
            BCF     STATUS, 0x00
            RLCF    PRODL, F
            ANDWF   PRODL, W
            MOVWF   INDF1
            #endasm
            if (TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;

    }

/******************************************************************************
 * For March C Minus test this section is skipped
 * Rd0 - read zero - ascending
 ******************************************************************************/
#ifndef MARCHCMINUS

    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;
    for (CLASSB_MarchIndex=0; CLASSB_MarchIndex < CLASSB_MarchLength; CLASSB_MarchIndex++)
    {
        PRODL = 0x00;
        for(TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L

            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H

            MOVF    INDF1, W
            XORWF   PRODL, W
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH
            #endasm
            if (TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;
    }

#endif

/******************************************************************************
 * Rd0Wr1 - read zero, write one - descending
 ******************************************************************************/
   //    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress + CLASSB_MarchLength - 1;

    CLASSB_MarchstartAddress--;
    CLASSB_MARCHtempAddress = CLASSB_MarchstartAddress;
    for (CLASSB_MarchIndex=CLASSB_MarchLength; CLASSB_MarchIndex>0; CLASSB_MarchIndex--)
    {

        PRODL = 0x00;
        for(TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L

            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H

            MOVF    INDF1, W
            XORWF   PRODL, W
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVF    INDF1, W
            BSF     STATUS, 0x00
            RRCF    PRODL, F
            IORWF   PRODL, W
            MOVWF   INDF1
            #endasm
            if (TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress--;
    }

/******************************************************************************
 * Rd1Wr0 - read one, write zero - descending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;// + CLASSB_MarchLength - 1;
    for (CLASSB_MarchIndex=CLASSB_MarchLength; CLASSB_MarchIndex>0; CLASSB_MarchIndex--)
    {

        PRODL = 0xFF;
        for(TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L

            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H

            MOVF    INDF1, W
            XORWF   PRODL, W
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVF    INDF1, W
            BCF     STATUS, 0x00
            RRCF    PRODL, F
            ANDWF   PRODL, W
            MOVWF   INDF1
            #endasm
            if (TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress--;
    }

/******************************************************************************
 * Rd0 - read zero - descending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MARCHtempAddress;// + CLASSB_MarchLength - 1;
    for (CLASSB_MarchIndex = CLASSB_MarchLength; CLASSB_MarchIndex > 0; CLASSB_MarchIndex--)
    {

        PRODL = 0x00;
        for(TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L
    
            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H
    
            MOVF    INDF1, W
            XORWF   PRODL, W
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH
            #endasm
            if (TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress--;
    }

/******************************************************************************
 * March C (Minus) Test Passed
 ******************************************************************************/
    CLASSB_MarchstartAddress++;
    if (CLASSB_MarchbufferAddress != NULL)
    memcpy(CLASSB_MarchstartAddress, CLASSB_MarchbufferAddress, CLASSB_MarchLength); //restore user memory before stack test

    return CLASSB_TEST_PASS;

} // End of function

/***********************************************************
* Description:
*     This function sets up the memory for a failure.
*     With this the MarchCstartAddress will still be at the
*     at the address of the failure!
***********************************************************/
void MarchCFailure (void)
{
    if (CLASSB_MarchbufferAddress != NULL)
        memcpy(CLASSB_MARCHtempAddress, CLASSB_MarchbufferAddress, CLASSB_MarchLength); //restore user memory
}

/***********************************************************
* Description:
*     This function runs the March C test on the 7 RAM
*     addresses used to run the rest of the March C test.
***********************************************************/
CLASSBRESULT MarchCGlobalTest (void)
{

#asm
    MOVF    0x60, W
    MOVWF   PRODL       //CLASSB_MarchstartAddress
    MOVF    0x61, W
    MOVWF   PRODH       //CLASSB_MarchstartAddress+1
    MOVF    0x62, W
    MOVWF   TBLPTRL     //CLASSB_MarchbufferAddress
    MOVF    0x63, W
    MOVWF   TBLPTRH     //CLASSB_MarchbufferAddress+1
    MOVF    0x68, W
    MOVWF   FSR0L       //CLASSB_MarchLength
    MOVF    0x69, W
    MOVWF   FSR0H       //CLASSB_MarchLength+1
#endasm

    uint8_t i;
    uint8_t r;
    uint8_t bitMask;

    //write 0 - ascending
    
    FSR1 = 0x60;    //FSR1 will be used for the address of the tested location.
                    //Used to make sure that FSR0 is NOT used (because it is being used to save the length)
    for(i = 0; i < 10; i++)
    {
        INDF1 = 0;
        FSR1++;
    }

    //read zero, write one - ascending
    
    FSR1 = 0x60;
    for(i = 0; i < 10; i++)
    {
        bitMask = 0x01;
        for(r = 0; r < 8; r++)
        {
            if(INDF1 & bitMask)
            {
                return CLASSB_TEST_FAIL;
            }
            else
            {
                INDF1 |= bitMask;
            }
            bitMask <<= 1;
        }
        FSR1++;
    }

    //read one, write zero - ascending
    FSR1 = 0x60;
    for(i = 0; i < 10; i++)
    {
        bitMask = 0x01;
        for(r = 0; r < 8; r++)
        {
            if(INDF1 & bitMask)
            {
                INDF1 &= ~bitMask;
            }
            else
            {
                return CLASSB_TEST_FAIL;
            }
            bitMask <<= 1;
        }
        FSR1++;
    }

#ifndef MARCHCMINUS
    FSR1 = 0x60;
    for(i = 0; i < 10; i++)
    {
   
        if(INDF1 != 0)
        {
            return CLASSB_TEST_FAIL;
        }
        FSR1++;
    }
#endif

    FSR1 = 0x69;
    for(i = 0; i < 10; i++)
    {
     
        bitMask = 0x80;
        for(r = 0; r < 8; r++)
        {
            if(INDF1 & bitMask)
            {
                return CLASSB_TEST_FAIL;
            }
            else
            {
                INDF1 |= bitMask;
            }
            bitMask >>= 1;
        }
        FSR1--;
    }

    //read one, write zero - descending
    FSR1 = 0x69;
    for(i = 0; i < 10; i++)
    {
        bitMask = 0x80;
        for(r = 0; r < 8; r++)
        {
            if(INDF1 & bitMask)
            {
                INDF1 &= ~bitMask;
            }
            else
            {
                return CLASSB_TEST_FAIL;
            }
            bitMask >>= 1;
        }
        FSR1--;
    }

    //read zero - descending
    FSR1 = 0x69;
    for(i = 0; i < 10; i++)
    {
        if (INDF1 != 0)
        {
            return CLASSB_TEST_FAIL;
        }
        FSR1--;
    }

#asm
    MOVFF   FSR0H, 0X69     //CLASSB_MarchLength+1
    MOVFF   FSR0L, 0X68     //CLASSB_MarchLength
    MOVFF   TBLPTRH, 0X63   //CLASSB_MarchbufferAddress+1
    MOVFF   TBLPTRL, 0X62   //CLASSB_MarchbufferAddress
    MOVFF   PRODH, 0X61     //CLASSB_MarchstartAddress+1
    MOVFF   PRODL, 0X60     //CLASSB_MarchstartAddress
#endasm

    return CLASSB_TEST_PASS;
}