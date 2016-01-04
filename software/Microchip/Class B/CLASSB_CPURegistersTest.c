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

/*********************************************************************
* Description:
*
* This test check the core registers (no peripheral registers are tested) of the device
* by writing some values (usually 55h & AAh) to them and then reading the same. An array
* (by name 'a') of 8 bytes ( defined in .INC file) has been reserved in the RAM area for 
* saving the registers content before testing them. W and STATUS registers are not saved
* as they r automatically saved before calling a function. A macro is also defined in 
* '.INC' file which actually test the registers.
*
* The test return a value 00h in the W register if all the registers are working fine  *0x13 changed MVL 13/04/2010
* otherwise it returns 86h.
*********************************************************************/

#include "CLASSB_CPURegistersTest.h"
#include "CLASSB_Typedef.h"

#asm
Reg_save: DS 8                  			//reserve 8 bytes for saving the stack content in access RAM
STKPTR_Save EQU Reg_save
TOSL_Save EQU Reg_save+1
TOSH_Save EQU Reg_save+2
TOSU_Save EQU Reg_save+3
INTCON_Save EQU Reg_save+4
INTCON2_Save EQU Reg_save+5
INTCON3_Save EQU Reg_save+6
SFR_Save EQU Reg_save+7
#endasm

CLASSBRESULT CLASSB_CPURegistersTest(void)
{
    #asm                                                // Ams Changed
    MOVFF STKPTR, STKPTR_Save           		//save the return address from the stack in the RAM
    MOVFF TOSL, TOSL_Save
    MOVFF TOSH, TOSH_Save
    MOVFF TOSU, TOSU_Save

    MOVFF INTCON, INTCON_Save
    MOVFF INTCON2, INTCON2_Save
    MOVFF INTCON3, INTCON3_Save

    //*********************************
    //Test W register - nearly identical to macro
    MOVLW 0xAA                                          // W reg will be automatically saved while call
    SUBLW 0xAA
    BNZ   ERR1

    MOVLW 55h
    SUBLW 55h
    BNZ   ERR1

    //*********************************
    MOVFF TBLPTRL,SFR_Save
    //Test TBLPTRL register - use macro
    // Replacing macro by asm code- AP
    //AP-Replacing numeric operand 0 or 1 for BSR and access bank by 'b' and 'c'respectively fpr XC8 compiler
    MOVLW 0x55
    MOVWF TBLPTRL
    CPFSEQ TBLPTRL, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF TBLPTRL
    CPFSEQ TBLPTRL, c
    BRA  ERR1
    MOVFF SFR_Save, TBLPTRL

    //*********************************
    MOVFF TBLPTRH,SFR_Save
    //Test TBLPTRH register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF TBLPTRH
    CPFSEQ TBLPTRH, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF TBLPTRH
    CPFSEQ TBLPTRH, c
    BRA  ERR1

    MOVFF SFR_Save, TBLPTRH

    //*********************************
    MOVFF TBLPTRU, SFR_Save
    //Test TBLPTRU register - use macro
    MOVLW 0x15                                          // Replacing macro by asm code- AP
    MOVWF TBLPTRU
    CPFSEQ TBLPTRU, c
    BRA  ERR1

    MOVLW 0x2A                                          // Replacing macro by asm code- AP
    MOVWF TBLPTRU
    CPFSEQ TBLPTRU, c
    BRA  ERR1

    MOVFF SFR_Save, TBLPTRU

    //*********************************
    MOVFF TABLAT, SFR_Save
    //Test TABLAT register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF TABLAT
    CPFSEQ TABLAT, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF TABLAT
    CPFSEQ TABLAT, c
    BRA  ERR1

    MOVFF SFR_Save, TABLAT

    //*********************************
    BRA SKIP
    ERR1:
    // Ams put colon for the label
    MOVFF STKPTR_Save, STKPTR                           //retrieve the return address from the saved location
    MOVF  TOSL_Save, w, c
    MOVWF TOSL, c
    MOVF TOSH_Save, w, c
    MOVWF TOSH, c
    MOVF TOSU_Save, w, c
    MOVWF TOSU, c

    MOVFF INTCON_Save, INTCON                           //retrieve the intcon value
    MOVFF INTCON2_Save, INTCON2
    MOVFF INTCON3_Save, INTCON3

    RETLW CLASSB_TEST_FAIL
    
    VOID:
    NOP;

    SKIP:
    //*********************************
    MOVFF PRODH, SFR_Save
    //Test PRODH register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF PRODH
    CPFSEQ PRODH, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF PRODH
    CPFSEQ PRODH, c
    BRA  ERR1

    MOVFF SFR_Save, PRODH

    //*********************************
    MOVFF PRODL, SFR_Save
    //	Test PRODL register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF PRODL
    CPFSEQ PRODL, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF PRODL
    CPFSEQ PRODL, c
    BRA  ERR1

    MOVFF SFR_Save, PRODL

    //*********************************
    MOVFF FSR0L, SFR_Save
    //	Test FSR0L register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF FSR0L
    CPFSEQ FSR0L, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF FSR0L
    CPFSEQ FSR0L, c
    BRA  ERR1

    MOVFF SFR_Save, FSR0L

    //********************************
    MOVFF FSR0H, SFR_Save
    //	Test FSR0H register - use macro
    MOVLW 0x05                                          // Replacing macro by asm code- AP
    MOVWF FSR0H
    CPFSEQ FSR0H, c
    BRA  ERR1

    MOVLW 0x0A                                          // Replacing macro by asm code- AP
    MOVWF FSR0H
    CPFSEQ FSR0H, c
    BRA  ERR1

    MOVFF SFR_Save, FSR0H

    //*********************************
    MOVFF FSR1L, SFR_Save
    //	Test FSR1L register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF FSR1L
    CPFSEQ FSR1L, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF FSR1L
    CPFSEQ FSR1L, c
    BRA  ERR1

    MOVFF SFR_Save, FSR1L

    //*********************************
    MOVFF FSR1H, SFR_Save
    //	Test FSR1H register - use macro
    MOVLW 0x05                                          // Replacing macro by asm code- AP
    MOVWF FSR1H
    CPFSEQ FSR1H, c
    BRA  ERR1

    MOVLW 0x0A                                          // Replacing macro by asm code- AP
    MOVWF FSR1H
    CPFSEQ FSR1H, c
    BRA  ERR1

    MOVFF SFR_Save, FSR1H

    //*********************************
    MOVFF FSR2L, SFR_Save
    //	Test FSR2L register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF FSR2L
    CPFSEQ FSR2L, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF FSR2L
    CPFSEQ FSR2L, c
    BRA  ERR1

    MOVFF SFR_Save, FSR2L

    //*********************************
    MOVFF FSR2H, SFR_Save
    //	Test FSR2H register - use macro
    MOVLW 0x05                                          // Replacing macro by asm code- AP
    MOVWF FSR2H
    CPFSEQ FSR2H, c
    BRA  ERR1

    MOVLW 0x0A                                          // Replacing macro by asm code- AP
    MOVWF FSR2H
    CPFSEQ FSR2H, c
    BRA  ERR1

    MOVFF SFR_Save, FSR2H

    //*********************************
    MOVFF RCON, SFR_Save
    //	Test RCON register - use macro
    MOVLW 0x1D                                           // Replacing macro by asm code- AP
    MOVWF RCON
    CPFSEQ RCON, c
    BRA  ERR1

    MOVLW 0x8E                                          // Replacing macro by asm code- AP
    MOVWF RCON
    CPFSEQ RCON, c
    BRA  ERR1

    MOVFF SFR_Save, RCON

    //*********************************

    BCF INTCON, 7, c                                    //DISABLE GLOBAL INTERRUPT BIT TO CHECK STACK REGISTERS

    //*********************************
    //Test STATUS register - use macro //ONLY FIVE BITS R USED
    MOVLW 0x15                                          // Replacing macro by asm code- AP
    MOVWF STATUS
    CPFSEQ STATUS, c
    BRA  ERR1

    MOVLW 0x0A                                          // Replacing macro by asm code- AP
    MOVWF STATUS
    CPFSEQ STATUS, c
    BRA  ERR1

//    //********************************* //!!!!!!!!!!!!!!!!!!!! INTCON breaking test - CS
//    //Test INTCON register - use macro
//    MOVLW 0x55                                          // Replacing macro by asm code- AP
//    MOVWF INTCON
//    CPFSEQ INTCON, c
//    BRA  ERR1
//
//    MOVLW 0x2A                                          // Replacing macro by asm code- AP
//    MOVWF INTCON
//    CPFSEQ INTCON, c
//    BRA  ERR1
//    //let the global interrupt bit disable
//
//    //*******************************
//    //Test INTCON2 register - use macro
//    MOVLW 0x55                                          // Replacing macro by asm code- AP
//    MOVWF INTCON2
//    CPFSEQ INTCON2, c
//    BRA  ERR1
//
//    MOVLW 0xA0                                          // Replacing macro by asm code- AP
//    MOVWF INTCON2
//    CPFSEQ INTCON2, c
//    BRA  ERR1
//    // 1st and 3rd bits always read 0
//
//    //*********************************
//    //Test INTCON3 register - use macro
//    MOVLW 0x51                                          // Replacing macro by asm code- AP
//    MOVWF INTCON3                                       //2nd bit always read 0
//    CPFSEQ INTCON3, c
//    BRA  ERR1
//
//    MOVLW 0x8A                                          // Replacing macro by asm code- AP
//    MOVWF INTCON3                                       //5th bit always read 0
//    CPFSEQ INTCON3, c
//    BRA  ERR1

    //*********************************
    //Test TOSU register - use macro
    MOVLW 0x15                                          // Replacing macro by asm code- AP
    MOVWF TOSU                                          //ONLY FIVE BITS R USED
    CPFSEQ TOSU, c
    BRA  ERR1

    MOVLW 0x0A                                          // Replacing macro by asm code- AP
    MOVWF TOSU
    CPFSEQ TOSU, c
    BRA  ERR1

    //*********************************
    //Test TOSH register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF TOSH                                          //ONLY FIVE BITS R USED
    CPFSEQ TOSH, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF TOSH
    CPFSEQ TOSH, c
    BRA  ERR1

    //*********************************
    //Test TOSL register - use macro
    MOVLW 0x55                                          // Replacing macro by asm code- AP
    MOVWF TOSL                                          //ONLY FIVE BITS R USED
    CPFSEQ TOSL, c
    BRA  ERR1

    MOVLW 0xAA                                          // Replacing macro by asm code- AP
    MOVWF TOSL
    CPFSEQ TOSL, c
    BRA  ERR1

    //********************************
    //Test STKPTR register - use macro
    MOVLW 0x15                                          // Replacing macro by asm code- AP
    MOVWF STKPTR
    CPFSEQ STKPTR, c
    BRA  ERR1

    MOVLW 0x0A                                          // Replacing macro by asm code- AP
    MOVWF STKPTR
    CPFSEQ STKPTR, c
    BRA  ERR1

    //*********************************
    MOVFF INTCON_Save,INTCON                            //retrieve the intcon value
    MOVFF INTCON2_Save,INTCON2
    MOVFF INTCON3_Save,INTCON3

    MOVFF STKPTR_Save, STKPTR                           //retrieve the return address from the saved location
    MOVF  TOSL_Save, w, c
    MOVWF TOSL, c
    MOVF TOSH_Save, w, c
    MOVWF TOSH, c
    MOVF TOSU_Save, w, c
    MOVWF TOSU, c
    
   // RETLW CLASSB_TEST_PASS

    #endasm

    return CLASSB_TEST_PASS;
                                     
}