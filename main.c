#include "EPD_4in2.h"

#include <signal.h>

#include <math.h>
#include <stdlib.h>

void Handler(int signo) {
  // System Exit
  printf("\r\nHandler:exit\r\n");
  DEV_Module_Exit();

  exit(0);
}

int main() {
  printf("EPD_4IN2_test Demo\r\n");
  if (DEV_Module_Init() != 0) {
    return -1;
  }
  signal(SIGINT, Handler);

  printf("e-Paper Init and Clear...\r\n");
  EPD_4IN2_Init();
  EPD_4IN2_Clear();
  DEV_Delay_ms(500);

  // Create a new image cache
  UBYTE *BlackImage;
  UWORD Imagesize = ((EPD_4IN2_WIDTH + 7) / 8) * EPD_4IN2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    return -1;
  }
  printf("Paint_NewImage\r\n");

  printf("show window BMP-----------------\r\n");

  for (int y = 0; y < EPD_4IN2_HEIGHT; ++y) {
    uint8_t *p = BlackImage + ((EPD_4IN2_WIDTH + 7) / 8) * y;
    for (int x = 0; x < EPD_4IN2_WIDTH; ++x) {
      const int px = 100, py = 200;
      int d = sqrt((x - px) * (x - px) + (y - py) * (y - py));
      int col = d % 50 > 25;
      int i = x % 8;
      if (col)
        *(p + x / 8) |= (0x80 >> i);
      else
        *(p + x / 8) &= 0xFF ^ (0x80 >> i);
    }
  }

  EPD_4IN2_Display(BlackImage);
  DEV_Delay_ms(2000);


  EPD_4IN2_Clear();
  EPD_4IN2_Init_4Gray();
  printf("show Gray------------------------\r\n");
  free(BlackImage);
  BlackImage = NULL;
  Imagesize = ((EPD_4IN2_WIDTH + 3) / 4) * EPD_4IN2_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    return -1;
  }

  for (int y = 0; y < EPD_4IN2_HEIGHT; ++y) {
    uint8_t *p = BlackImage + ((EPD_4IN2_WIDTH + 3) / 4) * y;
    for (int x = 0; x < EPD_4IN2_WIDTH; ++x) {
      const int px = 200, py = 200;
      int d = sqrt((x - px) * (x - px) + (y - py) * (y - py));
      int col = (d % 64) / 16;
      int i = x % 4;
      // reset
      *(p + x / 4) &= 0xFF ^ (3 << (6 - i * 2));
      *(p + x / 4) |= (col << (6 - i * 2));
    }
  }

  EPD_4IN2_4GrayDisplay(BlackImage);
  DEV_Delay_ms(2000);

  EPD_4IN2_Clear();
  EPD_4IN2_Init();
  EPD_4IN2_Clear();
  printf("Goto Sleep...\r\n");
  EPD_4IN2_Sleep();
  free(BlackImage);
  BlackImage = NULL;
  DEV_Delay_ms(2000); // important, at least 2s
  printf("close 5V, Module enters 0 power consumption ...\r\n");
  DEV_Module_Exit();

  return 0;
}