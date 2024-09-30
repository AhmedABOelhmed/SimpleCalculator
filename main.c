/******************************************************************************
 *
 * Project: Simple Calculator
 *
 * File Name: main.c
 *
 * Description: source file for the project
 *
 * Author: Ahmed_Ahboelhmed
 *
 *******************************************************************************/

#include "lcd.h"    /* Include LCD library for display functions*/
#include "keypad.h"  /* Include keypad library for handling input*/
#include "gpio.h"    /* Include GPIO library for input/output operations*/
#include <util/delay.h>  /* Include delay library for introducing delays*/

#define START_KEY 13    /* Define the key used to start the calculator*/

int main(void)
{
	uint8 key ;      /* Variable to hold the pressed key*/
	float num1 = 0;   /* Variable to store the first number*/
	char operator = '\0';   /* Variable to store the operator*/
	float num2 = 0;        /* Variable to store the second number*/
	float result;        /* Variable to store the result of the operation*/
	uint8 isNum1Entered = 0;  /* Flag to check if the first number has been entered*/

	LCD_init();   /* Initialize the LCD*/
	LCD_displayStringRowColumn(0,3,"Electronic");    /* Display title on the first row*/
	LCD_displayStringRowColumn(1,3,"Calculator");    /* Display subtitle on the second row*/
	LCD_moveCursor(1,0);    /* Move cursor to the beginning of the second row*/

    /* Wait for the start key to be pressed*/
	do {
	        key = KEYPAD_getPressedKey(); /* Get the pressed key*/
	    } while (key != START_KEY);  /* Loop until the start key is pressed*/

	while(1)/* Main loop*/
	{
		key = KEYPAD_getPressedKey();   /* Get the pressed key*/
		/* Check if the key pressed is a number (0-9)*/
		if((key >= 0) && (key <= 9))
		{
			/* If the first number is not entered yet*/
			if(!isNum1Entered)
			{
				num1 = num1 * 10 + key; /* Update num1*/
				LCD_integerToString(key); /* Display the current digit*/
			}
			else  /* If the first number is already entered*/
			{
				num2 = num2 * 10 + key;  /* Update num2*/
				LCD_integerToString(key);  /* Display the current digit*/
			}
		}
		 /* Check if the pressed key is an operator*/
		else if(key == '+' || key == '-' || key == '%' || key == '*')
		{
			operator = key;/* Store the operator*/
			LCD_displaycharater(operator); /* Display the operator on the LCD*/
			isNum1Entered = 1; /* Set the flag indicating the first number is entered*/
		}
		/* Check if the '=' key is pressed*/
		else if(key == '=')
		{
			/* If no operator is entered, display num1 as the result*/
			if(operator == '\0') 
			{
				LCD_moveCursor(1,0); /* Move cursor to the second row*/
				LCD_integerToString(num1); /* Display num1 as the result*/
			}
			/* Perform calculation based on the operator*/
			else
			{
				/* Perform calculation based on the operator*/
				switch(operator)
					{
						case '+':
							result = num1+num2; /* Addition*/
							LCD_moveCursor(1,0);/* Move cursor to the beginning of the second row*/
							LCD_integerToString(result);/* Display the result*/
							break;
						case '-':
							result = num1-num2;/* Subtraction*/
							LCD_moveCursor(1,0); /* Move cursor to the beginning of the second row*/
							LCD_integerToString(result);/* Display the result*/
							break;
						case '%':
							/* Check for division by zero*/
							if (num2 != 0)
							{
								result = (float)num1 / num2;/* Division*/
								LCD_moveCursor(1,0); /* Move cursor to the beginning of the second row*/
								LCD_floatToString(result);/* Display the result*/
							}
							else
							{
								LCD_clearScreen();/* Clear the LCD*/
								LCD_displayString("Error: div by 0"); /* Display error message*/
								_delay_ms(1000);/* Wait for a second*/
								continue;/* Skip to the next iteration*/
							}
							break;
						case '*':
							result = num1*num2;/* Multiplication*/
							LCD_moveCursor(1,0);/* Move cursor to the beginning of the second row*/
							LCD_integerToString(result);/* Display the result*/
							break;
						default:
							LCD_clearScreen();/* Clear the LCD*/
							LCD_displayString("Unknown Operator!");/* Display error message*/
							break;
					}
			}
		}
		/* Check if the reset key is pressed*/
		else if(key == 13)/* Assuming 13 is the reset key*/
		{
			LCD_clearScreen();/* Clear the LCD*/
			LCD_integerToString(0);/* Reset the display to 0*/
			LCD_moveCursor(0,0);/* Move cursor to the beginning of the first row*/
			num1 = 0;/* Reset num1*/
			num2 = 0;/* Reset num2*/
			result = 0;/* Reset result*/
			isNum1Entered = 0; /* Reset flag*/
			operator = '\0';/* Reset operator*/
		}
		/* Introduce a delay to debounce the keypad*/
		_delay_ms(500);
	}
}
