
/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "image_provider.h"
#include <pico/stdlib.h>
#include <stdlib.h>

extern "C" {
#include "../../../rp2040_hm01b0/arducam.h"
}

#include "pico/stdlib.h"

//static uint8_t image_buf[324*324];
//static uint8_t image_tmp[162*162];
//uint8_t image[96*96];
static uint8_t header[2] = {0x55,0xAA};



TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width,
                      int image_height, int channels, int8_t* image_data) {
    uint8_t header[2] = {0x55, 0xAA};
    static bool first = true;
    static uint8_t* image_buf = 0;
    static uint8_t* image_tmp = 0;
    struct arducam_config config;

    if (first) {

        image_buf = (uint8_t*)malloc(324*324);
        //image_tmp = (uint8_t*)malloc(162*162);

        config.sccb = i2c0;
        config.sccb_mode = I2C_MODE_16_8;
        config.sensor_address = 0x24;
        config.pin_sioc = PIN_CAM_SIOC;
        config.pin_siod = PIN_CAM_SIOD;
        config.pin_resetb = PIN_CAM_RESETB;
        config.pin_xclk = PIN_CAM_XCLK;
        config.pin_vsync = PIN_CAM_VSYNC;
        config.pin_y2_pio_base = PIN_CAM_Y2_PIO_BASE;

        config.pio = pio0;
        config.pio_sm = 0;

        config.dma_channel = 0;
        config.image_buf = image_buf;
        config.image_buf_size = sizeof(324*324);

        arducam_init(&config);

        TF_LITE_REPORT_ERROR(error_reporter, "Camera is initialized.");

        first = false;
    }

    TF_LITE_REPORT_ERROR(error_reporter, "Capture frame");
    arducam_capture_frame(&config);

    uint16_t x, y,i, j, index;
    i = 0;
    index = 0;
    uint8_t temp = 0;
//    for(y = 0; y< 324; y++){
//        for(x = (1+x)%2; x<324; x+=2){
//            image_tmp[index++] = config.image_buf[y*324+x];
//        }
//    }
//    index = 0;
//    for(y = 33; y< 129; y ++){
//        for(x = 33; x<129; x++){
//            image_data[index++] = image_buf[y*324+x];
//        }
//    }
    uart_write_blocking(uart0, header, 2);
    //uart_write_blocking(uart0, config.image_buf, config.image_buf_size);
    uart_write_blocking(uart0, (uint8_t *)image_buf, 324*324);
    //uart_write_blocking(uart0, (uint8_t *)image_data, 96*96);


    return kTfLiteOk;
}
