
#include <xc.h>  // imports PIC librairy
#include <stdio.h>
#include <stdlib.h>
#include "BUS_I2C.h" // imports bus_I2c library
#include "MCP4725.h" // imports MCP4725 library

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000 


BUS_I2C pbus={BUS_I2C_Init,BUS_I2C_Start,BUS_I2C_Repeated_Start,BUS_I2C_Write,BUS_I2C_Read, BUS_I2C_Stop}; // Defining of functions operating on the I2C bus

MCP4725_DATA comp1,comp2; // Defining of data of components 1 and 2
MCP4725_FUNCTION myfunct={MCP4725_Config, MCP4725_Write, MCP4725_Read, MCP4725_Power_Down}; // defining of function operating on all components MCp4725


int main(int argc, char** argv) {
    unsigned int i;
    
    TRISB= 0x00; // configurates PORTB als output
    
    PORTB=0;
     pbus.Init(20000000,100000); // initialize the clock frequency of the bus of 100KHz
     comp1.mbus= &pbus; // Association of the component 1 to bus
     comp2.mbus= &pbus; // Association of the component 2 to bus
     comp1.address= 0x00; //  address defining of component 1
     comp2.address= 0x01; //  address defining of component 2
     
     myfunct.Config(&comp1, 0x000f); // configuration of eeprom and Udapte DAC register in component 1  
     myfunct.Config(&comp2, 0x00f0); // configuration of eeprom and Udapte DAC register in component 2
     do{                           // reading of status of component 1 and other parameters
       __delay_ms(50);
       myfunct.Read(&comp1);
     }while(comp1.ready !=1);
     do{                           // reading of ststus of component 2 and other parameters
       __delay_ms(50);
       myfunct.Read(&comp2);
     }while(comp2.ready !=1);
     
     PORTB= (char)(comp1.dac_reg_data);  // displays DAC register value of component 1
     __delay_ms(6000);
     PORTB= (char)(comp2.dac_reg_data);  // displays DAC register value of component 2
     __delay_ms(6000);
     
     PORTB= (char)(comp1.eeprom_data);  // displays EEPROM value of component 1
     __delay_ms(6000);
     PORTB= (char)(comp2.eeprom_data);  // displays EEPROM value of component 2
     __delay_ms(6000);
     
     PORTB= 0;
     
     while(1){
        for(i=0;i<4010;i +=100 ){ 
            
            myfunct.Write(&comp2,i);  // udaptes DAC Register on component 2
            myfunct.Write(&comp1,i);  // udaptes DAC Register on component 2
            __delay_ms(20);
        }
        myfunct.Power_Down(&comp1,MCP4725_RES500); // blocks the outpout of component 1
        for(i=0;i<4010;i +=400 ){    // displays EEPROM value of component 2
             myfunct.Write(&comp2,i);
            __delay_ms(20);
        }        
     }
     
     
     
     
    return (EXIT_SUCCESS);
}

