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

#include "CLASSB_RAMCheckerBoardTest.h"
    
/*****************************************************************************
* Description:
*     This function that implements a CheckerBoard RAM test.
*     If bufferAddress is NULL the test will be destructive!!!
* Input:
*     *startAddress  	:  pointer to an array declared in user memory for test
*     length 		:  value indicating the number of bytes to test
*     *bufferAddress    :  pointer to location in memory where array will be stored during test
* Return Values:
*     CLASSB_TEST_PASS 	:  return value = 0
*     CLASSB_TEST_FAIL 	:  return value = 1
*
******************************************************************************/
CLASSBRESULT CLASSB_RAMCheckerboardTest(uint8_t* startAddress, uint16_t length, uint8_t* bufferAddress)
{
    volatile unsigned char i;
    volatile unsigned char r;
    volatile unsigned char checker = 0xAA;

    uint8_t* tempAddr;

    //***********************************************
    if (bufferAddress != NULL)
    memcpy(bufferAddress, startAddress, length); //save user Memory
    tempAddr = startAddress;

    for (r=0;r<2;r++)
    {
        //write checker pattern in RAM
        for (i = 0; i < length; i++)
        {
            (*startAddress) = checker;
            startAddress++;
            checker = ~checker;
        }
        startAddress = tempAddr;

        //read checker pattern in RAM
        for (i=0;i<length;i++)
        {
            if ((*startAddress) != checker)
            {
                if (bufferAddress != NULL)
                memcpy(startAddress, bufferAddress, length); //restore user memory
                return CLASSB_TEST_FAIL;
            }
            startAddress++;
            checker = ~checker;
        }
        //invert checker pattern
        checker = ~checker;
        startAddress = tempAddr;
    }
    if (bufferAddress != NULL)
    memcpy(startAddress, bufferAddress, length); //restore user memory
    return CLASSB_TEST_PASS;

} // End of function