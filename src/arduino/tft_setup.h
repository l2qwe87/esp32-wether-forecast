#define USER_SETUP_ID 407

// Driver
#define ST7789_DRIVER       // Configure all registers
#define TFT_WIDTH  170
#define TFT_HEIGHT 320
#define TFT_INVERSION_ON
#define TFT_BACKLIGHT_ON 1

// Pins
#define TFT_BL     16      //(BLK) 16  // LED backlight or VCC
#define TFT_MISO   -1      // Not connected
#define TFT_MOSI   23      //(SDA)
#define TFT_SCLK   18      //(SCL)
#define TFT_CS      5      //(CS)
#define TFT_DC     17      //(DC)
#define TFT_RST    22      // Set TFT_RST to -1 if display RESET is connected to ESP32 board EN

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
//#define LOAD_FONT24
//#define LOAD_FONT36
//#define LOAD_FONT8N
#define LOAD_GFXFF
#define SMOOTH_FONT

// Other options
//#define SPI_READ_FREQUENCY    20000000
//#define SPI_FREQUENCY         40000000
#define SPI_FREQUENCY         80000000