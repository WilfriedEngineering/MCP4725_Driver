#include "MCP4725.h"
#include "BUS_I2C.h"



const char MCP4725_FIXED_ADDR= 0xc0;
const char MCP4725_WR_EEPROM_DAC= 0x60;
const char MCP4725_RES1= 0x10 ;
const char MCP4725_RES100= 0x20;
const char MCP4725_RES500= 0x30;


char MCP4725_tab[5];

    void MCP4725_Config(MCP4725_DATA *mcp, unsigned int reg_val){
        mcp->mbus->Start();
        mcp->mbus->Write(MCP4725_FIXED_ADDR | (char)(mcp->address <<1) );
        mcp->mbus->Write(MCP4725_WR_EEPROM_DAC);
        mcp->mbus->Write((char)(reg_val>>4));
        mcp->mbus->Write((char)(reg_val<<4));
        mcp->mbus->Stop();
    }
    
    
    void MCP4725_Write(MCP4725_DATA *mcp, unsigned int dac_val){
        mcp->mbus->Start();
        mcp->mbus->Write(MCP4725_FIXED_ADDR | (char)(mcp->address <<1) );
        mcp->mbus->Write((char)(dac_val>>8));
        mcp->mbus->Write((char)(dac_val));
        mcp->mbus->Stop();
    }
    
    
    void MCP4725_Read(MCP4725_DATA *mcp){
        char i;
        mcp->mbus->Start();
        mcp->mbus->Write(MCP4725_FIXED_ADDR | (char)(mcp->address <<1) | 0x01);
        for(i=0;i<5;i++){
            if(i==4) MCP4725_tab[i]= mcp->mbus->Read(0);
            else MCP4725_tab[i]= mcp->mbus->Read(1);
        }
        mcp->mbus->Stop();
        if(MCP4725_tab[0] & 0x80) mcp->ready= 1;
        else mcp->ready= 0;
        mcp->dac_reg_data= (unsigned int)(MCP4725_tab[1]<<4) + (unsigned int)(MCP4725_tab[2]>>4);
        mcp->eeprom_data= (unsigned int)( (MCP4725_tab[3] & 0x0f)<<8) + (unsigned int)(MCP4725_tab[4]); 
        
    }
    
    
    void MCP4725_Power_Down(MCP4725_DATA *mcp, char x_res){
        mcp->mbus->Start();
        mcp->mbus->Write(MCP4725_FIXED_ADDR | (char)(mcp->address <<1) );
        mcp->mbus->Write(x_res);
        mcp->mbus->Write(0x00);
        mcp->mbus->Stop();
    }