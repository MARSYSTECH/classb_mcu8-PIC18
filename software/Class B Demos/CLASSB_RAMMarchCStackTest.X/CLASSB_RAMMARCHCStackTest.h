/**********************************************************************
* � 2013 - 2014 Microchip Technology Inc.
*
* Project name:     Class B Library
* FileName:         CLASSB.h
* Dependencies:     CLASSB_config.h
* Processor:        PIC18F
* Compiler:         XC8
* IDE:              MPLAB� IDE or MPLAB� X
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

#ifndef __CLASSB_MARCHC_RAM_AND_STACK_TEST_H__
#define __CLASSB_MARCHC_RAM_AND_STACK_TEST_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xc.h>
#include <string.h>
#include "CLASSB_Config.h"
#include "CLASSB_Typedef.h"


#define MARCHC_RAM_STACK_TEST_PASS           1
#define MARCHC_RAM_STACK_TEST_FAIL           0

#define ascending   0
#define descending  1

/*******************************************************************************
 * stackTestResult used in the stack test to pass the result to the main function
 *
 * stackTestResult = 0x00, the test passed
 * stackTestResult = 0xFF, the test failed
 *
 ******************************************************************************/
extern unsigned char stackTestResult;

extern unsigned char* CLASSB_MarchstartAddress;
extern unsigned char* CLASSB_MarchbufferAddress;
extern unsigned char* CLASSB_MARCHtempAddress;
extern unsigned int CLASSB_MarchLength;
extern unsigned int CLASSB_MarchIndex;


/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_RAMMarchCTest(startAddress, length, bufferAddress, minus)

  Description:
    Tests the RAM memory region using March C.

  Precondition:
    Interrupts must be disabled.

  Parameters:
    startAddress  - the first address of the tested RAM memory,
    length        - the byte length of the tested RAM memory.
    bufferAddress - the first address of the location in RAM to save user data
    minus         - if the parameter is TRUE the "minus" algorithm is used.
    stack         - if the parameter is TRUE the stack will be tested

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    The start address and length must be even (aligned by 2 bytes).
    Interrupts must be disabled during the test.
    If bufferAddress is NULL the test is destructive.
    To run the stack test a minimum of 33 bytes must be alocated for the bufferAddress.
  ***************************************************************************/
//Function Declarations
CLASSBRESULT CLASSB_RAMMarchCStackTest();

#endif