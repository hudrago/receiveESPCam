#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#define MATRIX_WIDTH 160
#define MATRIX_HEIGHT 120

#define CALC_COLOR(matrixElement) (((matrixElement>>3)<<11)|((matrixElement>>2)<<5)|(matrixElement>>3))

void lcd_setup();
void lcd_loop();
void print_screen();
void print_matrix(uint8_t x, uint8_t y, uint8_t scale);
void fill_matrix();

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  lcd_setup();

  // Serial.println(CALC_COLOR(0), HEX);
  // Serial.println(CALC_COLOR(1), HEX);
  // Serial.println(CALC_COLOR(8), HEX);
  // Serial.println(CALC_COLOR(128), HEX);
  // Serial.println(CALC_COLOR(255), HEX);

  // Serial.println("----STARTING-----");
}

void loop() {
  lcd_loop();
  // if (Serial2.available()) {
  //   Serial.println(Serial2.read());
  // }


}

uint16_t matrix[MATRIX_HEIGHT][MATRIX_WIDTH];

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void lcd_setup() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_WHITE);
}

void lcd_loop() {
    //tft.fillScreen(TFT_BLACK);
    Serial2.write(0xAA);
    print_screen(); 
}

void print_screen() {
    // tft.fillScreen(TFT_WHITE);
    fill_matrix();
    print_matrix(0, 0, 2);
    // Serial2.flush();
}

void print_matrix(uint8_t x, uint8_t y, uint8_t scale) {
    uint32_t pixelColor;
    uint16_t matrixElement;

    for(int i=0; i < MATRIX_HEIGHT; i++){
        for(int j=0; j < MATRIX_WIDTH; j++){

            // pixelColor = CALC_COLOR((i & 1) ? 0 : 255);
            // if (i & 1) {
            //   pixelColor = CALC_COLOR(100);
            // } else pixelColor = CALC_COLOR(150);
            
            matrixElement = matrix[i][j];
            pixelColor = CALC_COLOR(matrixElement);
            
            tft.fillRect(x+j*scale, y+i*scale, scale, scale, pixelColor);  
        
        }
    }
}

// bool dataReady() {
  
//   int data1, data2, data3;
//   bool newData = false;
  
//   if (Serial2.available()) {
    
//     data1 = Serial2.read();  
//     data2 = Serial2.read();  
//     data3 = Serial2.read();  

//     if((data1 == 0x3C) && (data2 == 0xF5) && (data3 == 0x3C)) {
//       Serial.println("------READY-----");
//       newData = true; 
//     }  
  
//   }

//   return newData;

// }

bool dataReady() {
  
  while(!Serial2.available());
  if(Serial2.read() != 0x3C) return false;
  while(!Serial2.available());
  if(Serial2.read() != 0xF5) return false;
  while(!Serial2.available());
  if(Serial2.read() != 0x3C) return false;
  
  // Serial.println("------READY-----");
  return true;

}

void fill_matrix() {
    
    // while (!dataReady()) {
    //   // Serial.println("x");
    // };
    // Serial.println("------FILLING-----");
    for(int i=0; i < MATRIX_HEIGHT; i++){
        for(int j=0; j < MATRIX_WIDTH; j++){
          
          while (!Serial2.available());
          matrix[i][j] = Serial2.read();

        }
    }
   
}