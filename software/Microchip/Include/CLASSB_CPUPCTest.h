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

#ifndef __CLASSB_CPU_PC_TEST_H__
#define __CLASSB_CPU_PC_TEST_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <xc.h>
#include "CLASSB_Typedef.h"
#include "CLASSB_config.h"

extern uint8_t PCtestFlag;

/****************************************************************************
  Function:
    CLASSBRESULT CLASSB_CPUPCTest()

  Summary:
    Tests CPU program counter.

  Description:
    Tests CPU program counter.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Returns zero if successful. Non zero means - failed.

  Remarks:
    The device flash memory size must be set using parameter
    CLASSB_DEVICE_FLASH_SIZE in classb_conig.h.
  ***************************************************************************/
CLASSBRESULT CLASSB_CPUPCTest();

#if   (CLASSB_DEVICE_FLASH_SIZE_KB == 2)

#define PCTestAddr1 0x554
#define PCTestAddr2 0x2AA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 4)

#define PCTestAddr1 0x554
#define PCTestAddr2 0xAAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 6)

#define PCTestAddr1 0x1554
#define PCTestAddr2 0xAAA


#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 8)

#define PCTestAddr1 0x1554
#define PCTestAddr2 0xAAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 12)

#define PCTestAddr1 0x1554
#define PCTestAddr2 0x2AAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 16)

#define PCTestAddr1 0x1554
#define PCTestAddr2 0x2AAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 32)

#define PCTestAddr1 0x1554
#define PCTestAddr2 0x2AAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 64)

#define PCTestAddr1 0x5554
#define PCTestAddr2 0xAAAA

#elif (CLASSB_DEVICE_FLASH_SIZE_KB == 128)

#define PCTestAddr1 0x15554
#define PCTestAddr2 0xAAAA

#else

#error "Valid CLASSB_DEVICE_FLASH_SIZE must be defined in CLASSB_config.h"

#endif

//Function Declarations
CLASSBRESULT CLASSB_CPUPCTest();
void CLASSB_PC_Function1(void);// @ PCTestAddr1;
void CLASSB_PC_Function2(void);// @ PCTestAddr2;

#endif