#ifndef MCP4725_H
#define MCP4725_H

  #include "BUS_I2C.h"

   typedef struct MCP4725_DATA{
       BUS_I2C *mbus;
       char address;
       char ready;
       unsigned int eeprom_data;
       unsigned int dac_reg_data;
    }MCP4725_DATA;
    
    typedef struct MCP4725_FUNCTION{
        void (*Config)(MCP4725_DATA *mcp, unsigned int reg_val);
        void (*Write)(MCP4725_DATA *mcp, unsigned int dac_val);
        void (*Read)(MCP4725_DATA *mcp);
        void (*Power_Down)(MCP4725_DATA *mcp, char x_res);       
    }MCP4725_FUNCTION;
    
    
        void MCP4725_Config(MCP4725_DATA *mcp, unsigned int reg_val);
        void MCP4725_Write(MCP4725_DATA *mcp, unsigned int dac_val);
        void MCP4725_Read(MCP4725_DATA *mcp);
        void MCP4725_Power_Down(MCP4725_DATA *mcp, char x_res); 
        
        
    extern const char MCP4725_FIXED_ADDR;
    extern const char MCP4725_WR_EEPROM_DAC;
    extern const char MCP4725_RES1;
    extern const char MCP4725_RES100;
    extern const char MCP4725_RES500;




#endif