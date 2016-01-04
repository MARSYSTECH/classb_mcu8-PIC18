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

#include "CLASSB_CPUPCTest.h"

unsigned char PCtestFlag @ 0x00F0; //located at known address 0xF0

//The PC test is done primarily as variables as shown here because of limitations in the linker
//on where functions can be located in Program Memory. This allows for the PC test to be located
//at any 3 locations in Program Memory and the PC latch will be set according to PC_TEST_ADDRESS_1(2)

//This is used as a software trap to detect a stuck bit in PC
const unsigned int PCTrap1 @ PCTestAddr1-2 = 0x0012; //0012 = OPCODE for return
const unsigned int PCTrap2 @ PCTestAddr2-2 = 0x0012;

//The use of DECF makes a false PASS much less likely along with the PCTrap. If there was an
//error then the PCTestFlag will not be decremented and the test will fail.
//This is used to decrement the PCTestFlag
const unsigned int PCTestDEC1 @ PCTestAddr1 = 0x07F0; //07F0 = OPCODE for DECF 0xF0,F
const unsigned int PCTestDEC2 @ PCTestAddr2 = 0x07F0;

//This returns after the decrement
const unsigned int PCTestRETURN1 @ PCTestAddr1+2 = 0x0012;
const unsigned int PCTestRETURN2 @ PCTestAddr2+2 = 0x0012;
/*******************************************************************
  * Description:
  * The Program Counter test is a functional test of the PC. The PC holds the address
  * of the next instruction tO be executed.
  * The tests performs the following major tasks:
  *		1. The Program Counter test invokes functions that are located in Flash memory at different addresses.
  *		2. These functions return a unique value( address of the respective functions).
  *		3. The returned value is verified using the "CLASSB_CPUPCTest" function.
  * Return Values:
  *     PC_TEST_FAIL :  return value = 0.
  *     PC_TEST_PASS :  return value = 1.
  *
  *******************************************************************/

CLASSBRESULT CLASSB_CPUPCTest ()
{
    PCtestFlag = 2;

    CLASSB_PC_Function1();
    CLASSB_PC_Function2();

    // Test if the Program Counter test Flag has been decremented twice
    if(PCtestFlag != 0)
        return CLASSB_TEST_FAIL;
    else
        return CLASSB_TEST_PASS;
}

/********************************************************************
* Description:
* This function is placed in the section called "SslTestSection1".The section
* is located in a defined address where the PC needs to jump using the custom linker script,
* This function returns the address of the CLASSB_PC_Function1.
* In this example the function CLASSB_PC_Function1 is placed in a section page1 at the
* address 0xd00e.
*
* Return Values:
*      returnAddress: returns the address of CLASSB_PC_Function1
*
********************************************************************/

void  CLASSB_PC_Function1(void)
{
    //Set the PC to the location of the first DECF
#if (CLASSB_DEVICE_FLASH_SIZE_KB == 128)
    PCLATU = 1;
#else
    PCLATU = 0;
#endif
    PCLATH = (PCTestAddr1 >> 8);
    PCL = PCTestAddr1;
}

/********************************************************************
* Description:
*  This function is placed in the section called "page2".The section
* is located in a defined address d10e.
* This function returns the address of the CLASSB_PC_Function2.
* Return Values:
*      returnAddress: returns the address of CLASSB_PC_Function2
*
********************************************************************/

void CLASSB_PC_Function2(void)
{
	//Set the PC to the location fo the second DECF
	PCLATU = 0;
	PCLATH = (PCTestAddr2 >> 8);
	PCL = PCTestAddr2;
}