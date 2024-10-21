aux_source_directory(. DIR_LIB_SRCS)
add_library(rp2040_hm01b0 ${DIR_LIB_SRCS})

target_include_directories(rp2040_hm01b0
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

pico_generate_pio_header(rp2040_hm01b0 ${CMAKE_CURRENT_LIST_DIR}/image.pio)

target_link_libraries(
        rp2040_hm01b0
        pico_stdlib
        hardware_dma
        hardware_i2c
        hardware_pio
        hardware_pwm
        hardware_spi
)
# enable usb output, disable uart output


