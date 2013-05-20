/*************************************************************************
 *
 *    File name   : commandParser.h
 *    Description : module for parsing input commands
 *
 *    History :
 *    1. Date        : 29 August 2010
 *       Author      : shondll
 *       Description : Create
 *
 **************************************************************************/

#ifndef _COMMAND_PARSER_H
#define _COMMAND_PARSER_H

/*******************************************************************************
* Function Name  : CommandParserInit(..)
* Description    : Initializes module variables
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CommandParserInit(void);

/*******************************************************************************
* Function Name  : CommandParserParse(..)
* Description    : Parse the input string for a command match. Recognised command can be retrieved with the 
*					'CommandParserGetCurrentCommand()' function
* Input          : inputStr - the string to analyse
* Output         : None
* Return         : Pointer to the next character after the processed command
*					- when no more commands are in string then NULL is returned
*					- when previous command has not yet been read then 'inputStr' is returned
*******************************************************************************/
char* CommandParserParse(char *inputStr);

/*******************************************************************************
* Function Name  : CommandParserGetCurrentCommand(..)
* Description    : Gets the last received and recognized command
* Input          : None
* Output         : None
* Return         : The last received and recognized command
*******************************************************************************/
XBEE_USER_CMD CommandParserGetCurrentCommand(void);

/*******************************************************************************
* Function Name  : CommandParserDiscardCurrentCommand(..)
* Description    : Call this function after processing the current command to enable reception of the next
* Input          : None
* Output         : None
* Return         : The last received and recognized command
*******************************************************************************/
void CommandParserDiscardCurrentCommand(void);

/*******************************************************************************
* Function Name  : CommandParserGetParam(..)
* Description    : Gets a parameter with given index of the last received and recognized command
* Input          : paramIndex - index of the parameter string to return.
* Output         : None
* Return         : Pointer to the parameter string.
*******************************************************************************/
char* CommandParserGetParam(int paramIndex);

/*******************************************************************************
* Function Name  : CommandParserGetParamsNum(..)
* Description    : Gets number of parameters of the last received and recognized command
* Input          : None
* Output         : None
* Return         : The number of parameters of the last received and recognized command.
*******************************************************************************/
int CommandParserGetParamsNum(void);

/*******************************************************************************
* Function Name  : CommandParserGenerateCommand(..)
* Description    : Generates a user command as if received through the UART
* *NOTE: do not call from interrupts!!
* Input          :	command - the command to generate
*					pParams - pointer to the array of parameters to append (place NULL if no params needed)
*					numberOfParams - if pParams is not NULL this indicates the number of parameters to append
* Output       : None
* Return         : TRUE on success, FALSE if error occured
*******************************************************************************/
BOOL CommandParserGenerateCommand(XBEE_USER_CMD command, char **pParams, int numberOfParams);

#endif // _COMMAND_PARSER_H