// ST7789 135 x 240 display with no chip select line

#define ST7789_DRIVER						// 	Video Driver
#define CGRAM_OFFSET						//	RAM Offsets
#define SPI_FREQUENCY  		40000000		//	Frequency of SPI BUS
#define SPI_READ_FREQUENCY 	6000000			//	Frequency of SPI BUS Reads

#define TFT_WIDTH  			135				
#define TFT_HEIGHT 			240
#define TFT_ROTATE			1

#define TFT_MOSI 			19
#define TFT_SCLK 			18
#define TFT_CS    			5 
#define TFT_DC    			16
#define TFT_RST   			23
#define TFT_BL 	  			4

#define TFT_BACKLIGHT_ON 	HIGH

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT