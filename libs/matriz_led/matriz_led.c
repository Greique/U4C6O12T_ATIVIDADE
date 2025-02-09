#include "matriz_led.h"

// Definições de número de pixels e matriz de LEDs
#define NUM_PIXELS 25  // O número total de LEDs na matriz (5x5)

// Frames dos números (representação binária de cada número na matriz 5x5)
double frame0[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0, 
                              0.0, 0.5, 0.0, 0.5, 0.0, 
                              0.0, 0.5, 0.0, 0.5, 0.0, 
                              0.0, 0.5, 0.0, 0.5, 0.0, 
                              0.0, 0.5, 0.5, 0.5, 0.0}; // 0

double frame1[NUM_PIXELS] = {0.0, 0.0, 0.5, 0.0, 0.0, 
                              0.0, 0.0, 0.5, 0.5, 0.0, 
                              0.0, 0.0, 0.5, 0.0, 0.0, 
                              0.0, 0.0, 0.5, 0.0, 0.0, 
                              0.0, 0.5, 0.5, 0.5, 0.0}; // 1

double frame2[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.5, 
                              0.5, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.5, 0.5, 0.5, 0.5, 
                              0.0, 0.0, 0.0, 0.0, 0.5, 
                              0.5, 0.5, 0.5, 0.5, 0.5}; // 2

double frame3[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.5, 
                              0.5, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.5, 0.5, 0.5, 0.5, 
                              0.5, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.5, 0.5, 0.5, 0.5}; // 3

double frame4[NUM_PIXELS] = {0.0, 0.5, 0.0, 0.5, 0.0, 
                              0.0, 0.5, 0.0, 0.5, 0.0, 
                              0.5, 0.5, 0.5, 0.5, 0.0, 
                              0.0, 0.5, 0.0, 0.0, 0.0, 
                              0.0, 0.5, 0.0, 0.0, 0.0}; // 4

double frame5[NUM_PIXELS] = {0.5, 0.5, 0.5, 0.5, 0.5, 
                              0.0, 0.0, 0.0, 0.0, 0.5, 
                              0.0, 0.5, 0.5, 0.5, 0.5, 
                              0.5, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.5, 0.5, 0.5, 0.5}; // 5

double frame6[NUM_PIXELS] = {0.5, 0.5, 0.5, 0.5, 0.0, 
                              0.0, 0.0, 0.0, 0.0, 0.5, 
                              0.0, 0.5, 0.5, 0.5, 0.5, 
                              0.5, 0.0, 0.0, 0.0, 0.5, 
                              0.0, 0.5, 0.5, 0.5, 0.0}; // 6

double frame7[NUM_PIXELS] = {0.5, 0.5, 0.5, 0.5, 0.5, 
                              0.5, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.5, 0.0, 0.0, 0.0, 
                              0.0, 0.0, 0.5, 0.0, 0.0, 
                              0.0, 0.0, 0.5, 0.0, 0.0}; // 7

double frame8[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                              0.5, 0.0, 0.0, 0.0, 0.5, 
                              0.0, 0.5, 0.5, 0.5, 0.0, 
                              0.5, 0.0, 0.0, 0.0, 0.5,
                              0.0, 0.5, 0.5, 0.5, 0.0,};

double frame9[NUM_PIXELS] = {0.5, 0.5, 0.5, 0.5, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.5,
                             0.5, 0.5, 0.5, 0.5, 0.5,
                             0.5, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.5, 0.5, 0.5,};

// Frames para os números de 0 a 9
double *frames[10] = {frame0, frame1, frame2, frame3, frame4, frame5, frame6, frame7, frame8, frame9};
double off[NUM_PIXELS] = {0.0};  // Todos os LEDs apagados

// Conversão RGB para formato WS2812
uint32_t matrix_rgb(double r, double g, double b) {
    // Converter valores de float [0.0, 1.0] para inteiro [0, 255]
    return ((int)(g * 255) << 16) | ((int)(r * 255) << 24) | ((int)(b * 255) << 8);
}

// Função para desenhar o número na matriz de LEDs usando PIO
void desenho_pio(double *frame, PIO pio, uint sm, double r, double g, double b) {
    uint8_t pixel_map[NUM_PIXELS] = {
        20,  21,  22,  23,  24, 
        19,  18,  17,  16,  15, 
        10, 11, 12, 13, 14, 
        9, 8, 7, 6, 5, 
        0, 1, 2, 3, 4
    };

    for (int i = 0; i < NUM_PIXELS; i++) {
        uint32_t color = matrix_rgb(r * frame[pixel_map[i]], g * frame[pixel_map[i]], b * frame[pixel_map[i]]);
        pio_sm_put_blocking(pio, sm, color);  // Envia a cor do LED para o PIO
    }
}

// Função para exibir um número na matriz de LEDs
void exibir_numero(int numero, PIO pio, uint sm, double r, double g, double b) {
    if (numero >= 0 && numero <= 9) {
        desenho_pio(frames[numero], pio, sm, r, g, b);  // Chama a função para exibir o número
    }
}
