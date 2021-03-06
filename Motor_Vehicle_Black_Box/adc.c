#include "pic_specific.h"
#include "adc.h"
#include "clcd.h"

extern char save_array[17];

//Defining the function to save the potentiometer data
void save_speed(unsigned short adc_reg_val)
{
	//Declare the variables
	char buff[5];
	unsigned char i;

	i = 3;
	//Read the value from adc and store in a temp buffer
	do
	{
		buff[i] = (adc_reg_val % 10) + '0';
		adc_reg_val = adc_reg_val / 10;
	} while (i--); 
	buff[4] = '\0';

	//Saving the data into the save array buffer which is used to write the data to the EEPROM
	save_array[13] = buff[0];
	save_array[14] = buff[1];
	save_array[15] = buff[2];

}

//Initialising the ADC
void init_adc(void)
{
	/* Selecting right justified ADRES Registers order */
	ADFM = 1;

	/* 
	 * Acqusition time selection bits 
	 * Set for 4 Tad
	 */
	ACQT2 = 0;
	ACQT1 = 1;
	ACQT0 = 0;

	/*
	 * Selecting the conversion clock of Fosc / 32 -> 1.6usecs -> 1Tad
	 * Our device frequency is 20 MHz
	 */
	ADCS0 = 0;
	ADCS1 = 1;
	ADCS2 = 0;

	/* Stop the conversion to start with */
	GODONE = 0;

	/* Selecting the channel */
	CHS2 = 1;

	/* Voltage reference bit as VSS */
	VCFG1 = 0;
	/* Voltage reference bit as VDD */
	VCFG0 = 0;

	/* Just clearing the ADRESH & ADRESL registers, for time pass */
	ADRESH = 0;
	ADRESL = 0;

	/* Turn ON the ADC module */
	ADON = 1;
}

//Defining the function to read value from ADC
unsigned short read_adc(unsigned char channel)
{
	unsigned short reg_val;

	/* Start the conversion */
	GO = 1;
	while (GO);
	reg_val = (ADRESH << 8) | ADRESL; 

	return reg_val;
}

void display_speed(unsigned short adc_reg_val)
{
	char buff[5];
	unsigned char i;

	i = 3;
	do
	{
		buff[i] = (adc_reg_val % 10) + '0';
		adc_reg_val = adc_reg_val / 10;
	} while (i--);
	buff[4] = '\0';

	clcd_print(buff, LINE2(13));
}

//Defining the function to save the event data to the save data array which is used to save the data to the EEPROM
void ON_OFF_up_down_gear(unsigned short gear)
{
	//Saving the corresponding data to the save array buffer
	switch (gear)
	{
		case 1:
			clcd_print("GR ", LINE2(9));
			save_array[10] = 'R';
			save_array[9] = 'G';
			save_array[11] = ' ';
			break;

		case 2:
			clcd_print("GN ", LINE2(9));
			save_array[9] = 'G';
			save_array[10] = 'N';
			save_array[11] = ' ';
			break;

		case 3:
			save_array[10] = '1';
			save_array[9] = 'G';
			save_array[11] = ' ';
			clcd_print("G1 ", LINE2(9));
			break;

		case 4:
			save_array[10] = '2';
			save_array[9] = 'G';
			save_array[11] = ' ';
			clcd_print("G2 ", LINE2(9));
			break; 
		case 5:
			save_array[9] = 'G';
			save_array[10] = '3';
			save_array[11] = ' ';
			clcd_print("G3 ", LINE2(9));
			break;

		case 6:
			save_array[9] = 'G';
			save_array[10] = '4';
			save_array[11] = ' ';
			clcd_print("G4 ", LINE2(9));
			break;

		case 10:
			save_array[9] = 'O';
			save_array[10] = 'N';
			save_array[11] = 32;
			break;

		case 11:
			save_array[9] = 'O';
			save_array[10] = 'F';
			save_array[11] = 'F';
			break;
	}
	save_array[8] = ' ';
	save_array[12] = ' ';
}
