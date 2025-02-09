#ifndef MATRIZ_LED_H
#define MATRIZ_LED_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "matriz_1A.pio.h"

// Definições
#define NUM_PIXELS 25
#define OUT_PIN 7

// Declaração dos frames dos números
extern double *frames[10]; // Ponteiro para os 10 números
extern double off[NUM_PIXELS];

// Funções
uint32_t matrix_rgb(double b, double r, double g);
void desenho_pio(double *desenho, PIO pio, uint sm, double r, double g, double b);
void exibir_numero(int numero, PIO pio, uint sm, double r, double g, double b);  // Nova função

#endif // MATRIZ_LED_H
