/*************************************************************************
 *
 *    (c) Olimex Ltd. 2010
 *
 *    File name   : uartUserInterface.c
 *    Description : Implementation of the user interface using USART2 module
 *
 *    History :
 *    1. Date        : 03 May 2010
 *       Author      : Aleksandar Mitev
 *       Description : Create
 *
 **************************************************************************/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct _STRING_COMMAND_PAIR {
	char *commandStr;
	XBEE_USER_CMD command;
} STRING_COMMAND_PAIR;

/* Private define ------------------------------------------------------------*/
#define PARAMS_MAX_NUM 5		// max number of parameters a command can accept
#define PARAM_CHAR_LENGTH 128	// max length in characters (+ null terminator) a param can have

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static XBEE_USER_CMD currentCommand; 				// the last reveived and recognised command
static char commandParams[PARAMS_MAX_NUM][PARAM_CHAR_LENGTH];	// parameters of the last reveived and recognised command
	
static STRING_COMMAND_PAIR supportedCommands[] = {
	{ "STATUS",			XBEE_USRCMD_STATUS },	// print status of the device
	{ "HELP",			XBEE_USRCMD_HELP },		// print help on commands usage
	{ "AT ",			XBEE_USRCMD_ATCMD },	// sent an AT command to the device
	{ "SEND16 ",		XBEE_USRCMD_SEND_16 },	// send message to a 16bit address
	{ "SEND64 ",		XBEE_USRCMD_SEND_64 },	// send message to a 64bit address
	{ "RCV16",			XBEE_USRCMD_RCV_16 },	// receive message from a 16bit address
	{ "RCV64",			XBEE_USRCMD_RCV_64 },	// receive message from a 64bit address	
	{ "",				XBEE_USRCMD_NONE }		// needed to determine end of commands list
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CommandParserInit(..)
* Description    : Initializes module variables
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CommandParserInit(void)
{		
	// init all used variables
	currentCommand = XBEE_USRCMD_NONE;

}

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
char* CommandParserParse(char *inputStr)
{
	static int commandIndex = 0;
	
	// ensure previous command has been processed
	if(currentCommand != XBEE_USRCMD_NONE) {
		return inputStr;
	}

	// check if all commands processed
	commandIndex = 0;
	while(supportedCommands[commandIndex].commandStr[0] != '\0') {
		// check for command string match
		if( strncmp(supportedCommands[commandIndex].commandStr,
					  inputStr,
					  strlen(supportedCommands[commandIndex].commandStr)) == 0) {
							
			// fill in parameters strings if any and store command
			{
				int i, j;
				char *p, *pNew;

				// zero all parameters
				for(i = 0; i < PARAMS_MAX_NUM; i++)
					commandParams[i][0] = '\0';

				p = pNew = inputStr + strlen(supportedCommands[commandIndex].commandStr);
				for(i = 0; i < PARAMS_MAX_NUM; i++) {
					if( (pNew = strchr(p, (int)' ')) != NULL) {
						// copy param string
						for(j = 0; (p < pNew) && j < PARAM_CHAR_LENGTH - 1; j++)
							commandParams[i][j] = *p++;
						commandParams[i][j] = '\0';
						p++; // jump over space character
					}
					else {
						// copy LAST param string
						for(j = 0; (*p != '\n') && (*p != '\0'); j++)
							commandParams[i][j] = *p++;
						commandParams[i][j] = '\0';
						break;
					}
				}
			
				// store new command
				currentCommand = supportedCommands[commandIndex].command;

				if(*p == '\0')
					return NULL;
				else // newline, jump over
					return p + 1;				
			}
		}
		else {
			// move on to the next command
			commandIndex++;
		}
	}
	
	// no command recognised, find newline or end of string
	while( (*inputStr != '\n') && (*inputStr != '\0') )
		inputStr++;

	if(*inputStr == '\0')
		return NULL;
	else // newline, jump over
		return inputStr + 1;						
	
}

/*******************************************************************************
* Function Name  : CommandParserGetCurrentCommand(..)
* Description    : Gets the last received and recognized command
* Input          : None
* Output         : None
* Return         : The last received and recognized command
*******************************************************************************/
XBEE_USER_CMD CommandParserGetCurrentCommand(void)
{
	return currentCommand;
}

/*******************************************************************************
* Function Name  : CommandParserDiscardCurrentCommand(..)
* Description    : Call this function after processing the current command to enable reception of the next
* Input          : None
* Output         : None
* Return         : The last received and recognized command
*******************************************************************************/
void CommandParserDiscardCurrentCommand(void)
{
	currentCommand = XBEE_USRCMD_NONE;
}

/*******************************************************************************
* Function Name  : CommandParserGetParam(..)
* Description    : Gets a parameter with given index of the last received and recognized command
* Input          : paramIndex - index of the parameter string to return.
* Output         : None
* Return         : Pointer to the parameter string.
*******************************************************************************/
char* CommandParserGetParam(int paramIndex)
{
	if( (currentCommand != XBEE_USRCMD_NONE) && (paramIndex < PARAMS_MAX_NUM) )
		return commandParams[paramIndex];
	else
		return NULL;
}

/*******************************************************************************
* Function Name  : CommandParserGetParamsNum(..)
* Description    : Gets number of parameters of the last received and recognized command
* Input          : None
* Output         : None
* Return         : The number of parameters of the last received and recognized command.
*******************************************************************************/
int CommandParserGetParamsNum(void)
{
	int i;
	
	if(currentCommand != XBEE_USRCMD_NONE) {
		for(i = 0; (i < PARAMS_MAX_NUM) && commandParams[i][0] != '\0'; i++);
		
		return i;
	}
	else
		return 0;
}

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
BOOL CommandParserGenerateCommand(XBEE_USER_CMD command, char **pParams, int numberOfParams)
{	
	int i;

	if(pParams != NULL) {
		if(numberOfParams > PARAMS_MAX_NUM)
			return FALSE;
		
		for(i = 0; i < numberOfParams; i++)
			strncpy(commandParams[i], *(pParams + i), PARAM_CHAR_LENGTH - 1);
	}
	
	currentCommand = command;
	
	return TRUE;
}
			

