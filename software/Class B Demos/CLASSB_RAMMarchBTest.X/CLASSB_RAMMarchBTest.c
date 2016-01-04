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

#include "CLASSB_RAMMarchBTest.h"
#include "CLASSB_Typedef.h"

#ifndef MARCH_VARS
#define MARCH_VARS
unsigned char* CLASSB_MarchstartAddress @ 0x60;
unsigned char* CLASSB_MarchbufferAddress @ 0x62;
unsigned char* CLASSB_MarchtempAddress @ 0x64;
unsigned int CLASSB_MarchIndex @ 0x66 = 0;
unsigned int CLASSB_MarchLength @ 0x68 = 0;
#endif

/**********************************************************************
* Description:
*     This function implements the March B test. It is a complete and non redundant test 
*     capable of detecting stuck-at faults.linked coupling Idempotent faults or Inversion
*     Coupling faults. This test is of complexity 17n( Where n is the number of bits). 
*     This test uses byte( 8-bit) accesses. The address must be properly aligned to the 
*     data type.This is a 
*     destructive test i.e, the memory contents is not preserved.Hence this test is meant
*     to be run at system startup, before the memory and the run time library is initialized.
*     The memory will be cleared (=0) when "CLASSB_RAMMarchBTest" test returns
*     
* Input:
*     startAddress :  start Address from which the March B test
*                        is to be performed.
*     length :          Number of bytes for which the test is to be
*                        performed.
* Return Values:
*     MARCHB_TEST_FAIL :  return value = 0
*     MARCHB_TEST_PASS :  return value = 1
*                                                 
**********************************************************************/

CLASSBRESULT CLASSB_RAMMarchBTest()
{
    volatile unsigned char testResult;
/******************************************************************************
 * Save memory for test in a known location designated by *CLASSB_MarchbufferAddress
 ******************************************************************************/
    if (CLASSB_MarchbufferAddress != NULL)
    memcpy(CLASSB_MarchbufferAddress, CLASSB_MarchstartAddress, CLASSB_MarchLength);

/******************************************************************************
 * Run March B Test on global RAM used by March B test.
 ******************************************************************************/
    if (CLASSB_MarchstartAddress < (char*)0x6A)
    {
        testResult = MarchBGlobalTest();
        if (testResult)
        {
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchLength -= (0x6B - (char)CLASSB_MarchstartAddress);
        CLASSB_MarchstartAddress = (char*)0x6A;
    }

/******************************************************************************
 * Wr0 - write zero to all memory for test
 ******************************************************************************/
    CLASSB_MarchtempAddress = CLASSB_MarchstartAddress;
    for (CLASSB_MarchIndex = 0; CLASSB_MarchIndex < CLASSB_MarchLength; CLASSB_MarchIndex++)
    {
        *CLASSB_MarchstartAddress=0x00;
        CLASSB_MarchstartAddress++;
    }

/******************************************************************************
 * Rd0Wr1Rd1Wr0Rd0Wr1 - read zero, write one, read 1, write zero, read 0, write 1 - ascending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MarchtempAddress;
    for (CLASSB_MarchIndex = 0; CLASSB_MarchIndex < CLASSB_MarchLength; CLASSB_MarchIndex++)
    {
        PRODL = 0x00;
        for (TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L
                    
            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H
                    
            MOVF    INDF1, W
            XORWF   PRODL, W            //Read zero
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVF    INDF1, W
            BSF     STATUS, 0x00
            RLCF    PRODL, F
            IORWF   PRODL, W
            MOVWF   INDF1               //write one

            MOVF    INDF1, W
            XORWF   PRODL, W            //Read one
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVF    INDF1, W
            BCF     STATUS, 0x00
            RRCF    PRODL, F
            ANDWF   PRODL, W
            MOVWF   INDF1               //write zero

            MOVF    INDF1, W
            XORWF   PRODL, W            //read zero
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVF    INDF1, W
            BSF     STATUS, 0X00
            RLCF    PRODL, F
            IORWF   PRODL, W
            MOVWF   INDF1               //write one
            #endasm
            if (TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;
    }

/******************************************************************************
 * Rd1Wr0Wr1 - read one, write zero, write one - ascending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MarchtempAddress;
    for (CLASSB_MarchIndex = 0; CLASSB_MarchIndex < CLASSB_MarchLength; CLASSB_MarchIndex++)
    {
        PRODL = 0xFF;
        PRODH = 0xFE;
        for (TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L
            
            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H
            
            MOVF    INDF1, W
            XORWF   PRODL, W       //read one
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVFF   PRODH, INDF1    //write zero
            
            BSF     STATUS, 0x00
            RLCF    PRODH, F

            MOVFF   PRODL, INDF1    //write one
            
            #endasm
            if(TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress++;
    }

/******************************************************************************
 * Rd1Wr0Wr1Wr0 - read one, write zero, write one, write zero - descending
 ******************************************************************************/
    CLASSB_MarchstartAddress--;
    CLASSB_MarchtempAddress = CLASSB_MarchstartAddress;
    for (CLASSB_MarchIndex = CLASSB_MarchLength; CLASSB_MarchIndex > 0; CLASSB_MarchIndex--)
    {
        PRODL = 0xFF;
        PRODH = 0x7F;
        for (TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L
            
            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H
            
            MOVF    INDF1, W
            XORWF   PRODL, W        //read one
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVFF   PRODH, INDF1    //write zero
            
            MOVFF   PRODL, INDF1    //write one
            
            MOVFF   PRODH, INDF1    //write zero
            
            BCF     STATUS, 0x00
            RRCF    PRODH, F
            BCF     STATUS, 0x00
            RRCF    PRODL, F
            #endasm
            if(TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress--;
    }

/******************************************************************************
 * Rd0Wr1Wr0 - read zero, write one, write zero - descending
 ******************************************************************************/
    CLASSB_MarchstartAddress = CLASSB_MarchtempAddress;
    for (CLASSB_MarchIndex = CLASSB_MarchLength; CLASSB_MarchIndex > 0; CLASSB_MarchIndex--)
    {
        PRODL = 0x00;
        PRODH = 0x80;
        for (TBLPTRL = 0; TBLPTRL < 8; TBLPTRL++)
        {
            #asm
            MOVFF   _CLASSB_MarchstartAddress, FSR1L
            
            MOVFF   _CLASSB_MarchstartAddress+1, FSR1H
            
            MOVF    INDF1, W
            XORWF   PRODL, W        //read zero
            MOVLW   0x01
            BTFSS   STATUS, 0x02
            MOVWF   TBLPTRH

            MOVFF   PRODH, INDF1    //write one
            
            MOVFF   PRODL, INDF1    //write zero
            
            BCF     STATUS, 0x00
            RRCF    PRODH, F
            #endasm
            if(TBLPTRH)
            return CLASSB_TEST_FAIL;
        }
        CLASSB_MarchstartAddress--;
    }

/******************************************************************************
 * Restore user memory
 * All tests passed
 ******************************************************************************/
    if (CLASSB_MarchbufferAddress != NULL)
    memcpy(CLASSB_MarchtempAddress, CLASSB_MarchbufferAddress, CLASSB_MarchLength); //restore user memory
    return CLASSB_TEST_PASS;
}

/******************************************************************************
* Description:
*     This function tests the global variables used in the March Test
* Return Values:
*     CLASSB_TEST_PASS :  return value = 0
*     CLASSB_TEST_FAIL :  return value = 1
*
******************************************************************************/
CLASSBRESULT MarchBGlobalTest()
{
 #asm
    MOVFF   0x60, PRODL     //CLASSB_MarchstartAddress
    MOVFF   0x61, PRODH     //CLASSB_MarchstartAddress+1
    MOVFF   0x62, TBLPTRL   //CLASSB_MarchbufferAddress
    MOVFF   0x63, TBLPTRH   //CLASSB_MarchbufferAddress+1
    MOVFF   0x68, FSR0L     //CLASSB_MarchLength
    MOVFF   0x69, FSR0H     //CLASSB_MarchLength+1
#endasm

    uint8_t r;
    uint8_t i;
    uint8_t bitMask;

    FSR1 = 0x60;    //FSR1 will be used for the address of the tested location.
                    //Used to make sure that FSR0 is NOT used (because it is being used to save the length)
    //write 0 - ascending
    for(r = 0; r < 10; r++)
    {
        INDF1 = 0;
        FSR1++;
    }

    //read 0, write 1, read 1, write 0, read 0, write 1 - ascending
    FSR1 = 0x60;
    for(r = 0; r < 10; r++)
    {
        bitMask = 0x01;
        for (i = 0; i < 8; i++)
        {
            if (INDF1 & bitMask) //read zero
            {
                return CLASSB_TEST_FAIL;
            }
            else
            {
                INDF1 |= bitMask; //write one
            }
            if(INDF1 & bitMask) //read one
            {
                INDF1 &= ~bitMask; //write zero
            }
            else
            {
                return CLASSB_TEST_FAIL;
            }
            if(INDF1 & bitMask) //read zero
            {
                return CLASSB_TEST_FAIL;
            }
            else
            {
                INDF1 |= bitMask; //write one
            }
            bitMask <<= 1;
        }
        FSR1++;
    }

    //read 1, write 0, write 1 - ascending
    FSR1 = 0x60;
    for(r = 0; r < 10; r++)
    {
        bitMask = 0x01;
        for (i=0; i<8; i++)
        {
            if (INDF1 & bitMask)
            {
                INDF1 &= ~bitMask;
                INDF1 |= bitMask;
            }
            else
            {
                return CLASSB_TEST_FAIL;
            }
            bitMask<<=1;
        }
        FSR1++;
    }

    //read 1, write 0, write 1, write 0 - descending
    FSR1 = 0x69;
    for (r = 0; r < 10; r++)
    {
        bitMask = 0x80;
        for (i=0; i<8; i++)
        {
            if (INDF1 & bitMask)
            {
                INDF1 &= ~bitMask;
                INDF1 |= bitMask;
                INDF1 &= ~bitMask;
            }
            else
            {
                return CLASSB_TEST_FAIL;
            }
            bitMask>>=1;
        }
        FSR1--;
    }

    //read 0, write 1, write 0 - descending
    FSR1 = 0x69;
    for (r = 0; r < 10; r++)
    {
        bitMask = 0x80;
        for (i=0; i<8; i++)
        {
            if (INDF1 & bitMask)
                return CLASSB_TEST_FAIL;
            else
            {
                INDF1 |= bitMask;
                INDF1 &= ~bitMask;
            }
            bitMask>>=1;
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