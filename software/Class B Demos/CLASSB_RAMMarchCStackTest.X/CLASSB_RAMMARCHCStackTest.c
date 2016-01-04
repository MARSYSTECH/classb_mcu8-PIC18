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

#include "CLASSB_RAMMARCHCStackTest.h"
#include "CLASSB_Typedef.h"

#ifndef MARCH_VARS
#define MARCH_VARS
unsigned char* CLASSB_MarchstartAddress @ 0x60;
unsigned char* CLASSB_MarchbufferAddress @ 0x62;
unsigned char* CLASSB_MARCHtempAddress @ 0x64;
unsigned int CLASSB_MarchIndex @ 0x66 = 0;
unsigned int CLASSB_MarchLength @ 0x68 = 0;
#endif

unsigned char stackTestResult;

/***********************************************************
 *
 * Description:
 *    This function performs the March C test on the stack.
 *    If the March C Minus test is desired set minus = 1
 * Inputs:
 *    *CLASSB_MarchCbufferAddress    :   pointer to the location to store memory during test
 *    MARCHCMINUS                    :   designates the test is a March C Minus test.
 *
 * ********************************************************/
CLASSBRESULT CLASSB_RAMMarchCStackTest(void)
{
/*****************************************************************************
* Save Stack here.
* ***************************************************************************/
    CLASSB_MARCHtempAddress = CLASSB_MarchbufferAddress;
    *CLASSB_MarchbufferAddress = STKPTR;

    CLASSB_MarchbufferAddress++;

    for (CLASSB_MarchIndex = 1; CLASSB_MarchIndex <= 31; CLASSB_MarchIndex++)
    {
        STKPTR = (char)CLASSB_MarchIndex;
        *CLASSB_MarchbufferAddress = TOSL;
        CLASSB_MarchbufferAddress++;
        *CLASSB_MarchbufferAddress = TOSH;
        CLASSB_MarchbufferAddress++;
        *CLASSB_MarchbufferAddress = TOSU;
        CLASSB_MarchbufferAddress++;
    }


/*****************************************************************************
* Run Stack Test here.
* ***************************************************************************/

//Write zero into stack
    STKPTR = 0;
    #asm
        MOVLW   0x00
        MOVWF   STKPTR
        Wr0:
        INCF    STKPTR
        CLRF    TOSL
        CLRF    TOSH
        CLRF    TOSU
        MOVF    STKPTR, W
        XORLW   0x1F
        BTFSS   STATUS, 0x02
        GOTO    Wr0

    //Read zero, Write one - ascending
        MOVLW   0x00
        MOVWF   STKPTR

        Rd0Wr1ASC:
        INCF    STKPTR

        BTFSC   TOSL,0
        GOTO    STACKFAIL
        BSF     TOSL,0
        BTFSC   TOSL,1
        GOTO    STACKFAIL
        BSF     TOSL,1
        BTFSC   TOSL,2
        GOTO    STACKFAIL
        BSF     TOSL,2
        BTFSC   TOSL,3
        GOTO    STACKFAIL
        BSF     TOSL,3
        BTFSC   TOSL,4
        GOTO    STACKFAIL
        BSF     TOSL,4
        BTFSC   TOSL,5
        GOTO    STACKFAIL
        BSF     TOSL,5
        BTFSC   TOSL,6
        GOTO    STACKFAIL
        BSF     TOSL,6
        BTFSC   TOSL,7
        GOTO    STACKFAIL
        BSF     TOSL,7

        BTFSC   TOSH,0
        GOTO    STACKFAIL
        BSF     TOSH,0
        BTFSC   TOSH,1
        GOTO    STACKFAIL
        BSF     TOSH,1
        BTFSC   TOSH,2
        GOTO    STACKFAIL
        BSF     TOSH,2
        BTFSC   TOSH,3
        GOTO    STACKFAIL
        BSF     TOSH,3
        BTFSC   TOSH,4
        GOTO    STACKFAIL
        BSF     TOSH,4
        BTFSC   TOSH,5
        GOTO    STACKFAIL
        BSF     TOSH,5
        BTFSC   TOSH,6
        GOTO    STACKFAIL
        BSF     TOSH,6
        BTFSC   TOSH,7
        GOTO    STACKFAIL
        BSF     TOSH,7

        BTFSC   TOSU,0
        GOTO    STACKFAIL
        BSF     TOSU,0
        BTFSC   TOSU,1
        GOTO    STACKFAIL
        BSF     TOSU,1
        BTFSC   TOSU,2
        GOTO    STACKFAIL
        BSF     TOSU,2
        BTFSC   TOSU,3
        GOTO    STACKFAIL
        BSF     TOSU,3
        BTFSC   TOSU,4
        GOTO    STACKFAIL
        BSF     TOSU,4

        MOVF    STKPTR, W
        XORLW   0x1F
        BTFSS   STATUS, 0x02
        GOTO    Rd0Wr1ASC

    //Read one, Write zero - ascending
        MOVLW   0x00
        MOVWF   STKPTR

        Rd1Wr0ASC:
        INCF    STKPTR

        BTFSS   TOSL,0
        GOTO    STACKFAIL
        BCF     TOSL,0
        BTFSS   TOSL,1
        GOTO    STACKFAIL
        BCF     TOSL,1
        BTFSS   TOSL,2
        GOTO    STACKFAIL
        BCF     TOSL,2
        BTFSS   TOSL,3
        GOTO    STACKFAIL
        BCF     TOSL,3
        BTFSS   TOSL,4
        GOTO    STACKFAIL
        BCF     TOSL,4
        BTFSS   TOSL,5
        GOTO    STACKFAIL
        BCF     TOSL,5
        BTFSS   TOSL,6
        GOTO    STACKFAIL
        BCF     TOSL,6
        BTFSS   TOSL,7
        GOTO    STACKFAIL
        BCF     TOSL,7

        BTFSS   TOSH,0
        GOTO    STACKFAIL
        BCF     TOSH,0
        BTFSS   TOSH,1
        GOTO    STACKFAIL
        BCF     TOSH,1
        BTFSS   TOSH,2
        GOTO    STACKFAIL
        BCF     TOSH,2
        BTFSS   TOSH,3
        GOTO    STACKFAIL
        BCF     TOSH,3
        BTFSS   TOSH,4
        GOTO    STACKFAIL
        BCF     TOSH,4
        BTFSS   TOSH,5
        GOTO    STACKFAIL
        BCF     TOSH,5
        BTFSS   TOSH,6
        GOTO    STACKFAIL
        BCF     TOSH,6
        BTFSS   TOSH,7
        GOTO    STACKFAIL
        BCF     TOSH,7

        BTFSS   TOSU,0
        GOTO    STACKFAIL
        BCF     TOSU,0
        BTFSS   TOSU,1
        GOTO    STACKFAIL
        BCF     TOSU,1
        BTFSS   TOSU,2
        GOTO    STACKFAIL
        BCF     TOSU,2
        BTFSS   TOSU,3
        GOTO    STACKFAIL
        BCF     TOSU,3
        BTFSS   TOSU,4
        GOTO    STACKFAIL
        BCF     TOSU,4

        MOVF    STKPTR, W
        XORLW   0x1F
        BTFSS   STATUS, 0x02
        GOTO    Rd1Wr0ASC

    //March C Minus Test will skip this section.
    //Read zero - ascending
    #ifndef MARCHCMINUS
        MOVLW   0x00
        MOVWF   STKPTR

        Rd0NMINUS:
        INCF    STKPTR

        MOVLW   0x00
        XORWF   TOSL
        BTFSS   STATUS,2
        GOTO    STACKFAIL

        MOVLW   0x00
        XORWF   TOSH
        BTFSS   STATUS,2
        GOTO    STACKFAIL

        MOVLW	0x00
        XORWF   TOSU
        BTFSS   STATUS,2
        GOTO    STACKFAIL

        MOVF    STKPTR, W
        XORLW   0x1F
        BTFSS   STATUS, 0x02
        GOTO    Rd0NMINUS
    #endif

    //Read zero, Write 1 - descending
        MOVLW   0x20
        MOVWF   STKPTR

        Rd0Wr1DES:
        DECF    STKPTR

        BTFSC   TOSU,4
        GOTO    STACKFAIL
        BSF     TOSU,4
        BTFSC   TOSU,3
        GOTO    STACKFAIL
        BSF     TOSU,3
        BTFSC   TOSU,2
        GOTO    STACKFAIL
        BSF     TOSU,2
        BTFSC   TOSU,1
        GOTO    STACKFAIL
        BSF     TOSU,1
        BTFSC   TOSU,0
        GOTO    STACKFAIL
        BSF     TOSU,0

        BTFSC   TOSH,7
        GOTO    STACKFAIL
        BSF     TOSH,7
        BTFSC   TOSH,6
        GOTO    STACKFAIL
        BSF     TOSH,6
        BTFSC   TOSH,5
        GOTO    STACKFAIL
        BSF     TOSH,5
        BTFSC   TOSH,4
        GOTO    STACKFAIL
        BSF     TOSH,4
        BTFSC   TOSH,3
        GOTO    STACKFAIL
        BSF     TOSH,3
        BTFSC   TOSH,2
        GOTO    STACKFAIL
        BSF     TOSH,2
        BTFSC   TOSH,1
        GOTO    STACKFAIL
        BSF     TOSH,1
        BTFSC   TOSH,0
        GOTO    STACKFAIL
        BSF     TOSH,0

        BTFSC   TOSL,7
        GOTO    STACKFAIL
        BSF     TOSL,7
        BTFSC   TOSL,6
        GOTO    STACKFAIL
        BSF     TOSL,6
        BTFSC   TOSL,5
        GOTO    STACKFAIL
        BSF     TOSL,5
        BTFSC   TOSL,4
        GOTO    STACKFAIL
        BSF     TOSL,4
        BTFSC   TOSL,3
        GOTO    STACKFAIL
        BSF     TOSL,3
        BTFSC   TOSL,2
        GOTO    STACKFAIL
        BSF     TOSL,2
        BTFSC   TOSL,1
        GOTO    STACKFAIL
        BSF     TOSL,1
        BTFSC   TOSL,0
        GOTO    STACKFAIL
        BSF     TOSL,0

        MOVF    STKPTR, W
        XORLW   0x01
        BTFSS   STATUS, 0x02
        GOTO    Rd0Wr1DES

    //Read one, Write zero - descending
        MOVLW   0x20
        MOVWF   STKPTR

        Rd1Wr0DES:
        DECF    STKPTR

        BTFSS   TOSU,4
        GOTO    STACKFAIL
        BCF     TOSU,4
        BTFSS   TOSU,3
        GOTO    STACKFAIL
        BCF     TOSU,3
        BTFSS   TOSU,2
        GOTO    STACKFAIL
        BCF     TOSU,2
        BTFSS   TOSU,1
        GOTO    STACKFAIL
        BCF     TOSU,1
        BTFSS   TOSU,0
        GOTO    STACKFAIL
        BCF     TOSU,0

        BTFSS   TOSH,7
        GOTO    STACKFAIL
        BCF     TOSH,7
        BTFSS   TOSH,6
        GOTO    STACKFAIL
        BCF     TOSH,6
        BTFSS   TOSH,5
        GOTO    STACKFAIL
        BCF     TOSH,5
        BTFSS   TOSH,4
        GOTO    STACKFAIL
        BCF     TOSH,4
        BTFSS   TOSH,3
        GOTO    STACKFAIL
        BCF     TOSH,3
        BTFSS   TOSH,2
        GOTO    STACKFAIL
        BCF     TOSH,2
        BTFSS   TOSH,1
        GOTO    STACKFAIL
        BCF     TOSH,1
        BTFSS   TOSH,0
        GOTO    STACKFAIL
        BCF     TOSH,0

        BTFSS   TOSL,7
        GOTO    STACKFAIL
        BCF     TOSL,7
        BTFSS   TOSL,6
        GOTO    STACKFAIL
        BCF     TOSL,6
        BTFSS   TOSL,5
        GOTO    STACKFAIL
        BCF     TOSL,5
        BTFSS   TOSL,4
        GOTO    STACKFAIL
        BCF     TOSL,4
        BTFSS   TOSL,3
        GOTO    STACKFAIL
        BCF     TOSL,3
        BTFSS   TOSL,2
        GOTO    STACKFAIL
        BCF     TOSL,2
        BTFSS   TOSL,1
        GOTO    STACKFAIL
        BCF     TOSL,1
        BTFSS   TOSL,0
        GOTO    STACKFAIL
        BCF     TOSL,0

        MOVF    STKPTR, W
        XORLW   0x01
        BTFSS   STATUS, 0x02
        GOTO    Rd1Wr0DES

    //Read zero - ascending
        MOVLW   0x00
        MOVWF   STKPTR

        Rd0ASC:
        INCF    STKPTR

        MOVLW   0x00
        XORWF   TOSL
        BTFSS   STATUS,2
        GOTO    STACKFAIL

        MOVLW   0X00
        XORWF   TOSH
        BTFSS   STATUS,2
        GOTO    STACKFAIL

        MOVLW	0x00
        XORWF   TOSU
        BTFSS   STATUS,2
        GOTO    STACKFAIL

        MOVF    STKPTR, W
        XORLW   0x1F
        BTFSS   STATUS, 0x02
        GOTO    Rd0ASC

    //The Stack test passed!
        GOTO    STACKEND

    //The Stack test failed!
        STACKFAIL:
        BANKSEL (_stackTestResult)
        movlw   0xFF
        movwf   BANKMASK(_stackTestResult)

        STACKEND:
        NOP
    #endasm


/*****************************************************************************
* Restore Stack here.
* ***************************************************************************/

    CLASSB_MarchbufferAddress = CLASSB_MARCHtempAddress;
    CLASSB_MarchbufferAddress++;

    FSR0 = CLASSB_MarchbufferAddress;
    STKPTR = 0x00;
    #asm
        RestoreStack:
        INCF	STKPTR
        MOVF 	POSTINC0, W
        MOVWF	TOSL

        MOVF	POSTINC0, W
        MOVWF	TOSH

        MOVF	POSTINC0, W
        MOVWF	TOSU

        MOVLW	0x1F
        XORWF	STKPTR, W
        BTFSS   STATUS, 0x02
        GOTO	RestoreStack
    #endasm

    STKPTR = *CLASSB_MARCHtempAddress;

    if (stackTestResult == 0xFF)
    {
        return CLASSB_TEST_FAIL;
    }
    else
    {
        return CLASSB_TEST_PASS;
    }
}