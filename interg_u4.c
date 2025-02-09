#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "matriz_led.h"

// Definição dos pinos dos LEDs RGB e botões
#define LED_VERDE_PIN 11
#define LED_AZUL_PIN 12
#define BOTAO_A_PIN 5
#define BOTAO_B_PIN 6
#define OUT_PIN 7 // Definição explícita do pino da matriz de LEDs WS2812

// Definições do I2C para o display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Variáveis para o estado dos LEDs
bool led_verde_estado = false;
bool led_azul_estado = false;

// Variáveis do display OLED
ssd1306_t ssd;
PIO pio = pio0;
uint sm;

// Inicializa o display OLED
void inicializar_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Configuração da matriz de LEDs WS2812
void setup_ws2812() {
    uint offset = pio_add_program(pio, &pio_matrix_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);
}

// Callback para tratar os botões A e B via interrupção
void botao_callback(uint gpio, uint32_t eventos) {
    static uint32_t ultimo_tempo = 0;
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

    // Debounce via tempo
    if (tempo_atual - ultimo_tempo < 200) return;
    ultimo_tempo = tempo_atual;

    if (gpio == BOTAO_A_PIN) {
        led_verde_estado = !led_verde_estado;
        gpio_put(LED_VERDE_PIN, led_verde_estado);
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, led_verde_estado ? "LED Verde: ON" : "LED Verde: OFF", 10, 10);
        ssd1306_send_data(&ssd);
        printf("Botao A, LED Verde: %s\n", led_verde_estado ? "Ligado" : "Desligado");
    } else if (gpio == BOTAO_B_PIN) {
        led_azul_estado = !led_azul_estado;
        gpio_put(LED_AZUL_PIN, led_azul_estado);
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, led_azul_estado ? "LED Azul: ON" : "LED Azul: OFF", 10, 30);
        ssd1306_send_data(&ssd);
        printf("Botao B, LED Azul: %s\n", led_azul_estado ? "Ligado" : "Desligado");
    }
}

// Configuração das interrupções dos botões
void configurar_interrupcoes() {
    gpio_set_irq_enabled_with_callback(BOTAO_A_PIN, GPIO_IRQ_EDGE_FALL, true, &botao_callback);
    gpio_set_irq_enabled_with_callback(BOTAO_B_PIN, GPIO_IRQ_EDGE_FALL, true, &botao_callback);
}

int main() {
    stdio_init_all();
    inicializar_display();
    setup_ws2812();

    gpio_init(LED_VERDE_PIN);
    gpio_set_dir(LED_VERDE_PIN, GPIO_OUT);
    gpio_init(LED_AZUL_PIN);
    gpio_set_dir(LED_AZUL_PIN, GPIO_OUT);
    
    gpio_init(BOTAO_A_PIN);
    gpio_set_dir(BOTAO_A_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_A_PIN);
    
    gpio_init(BOTAO_B_PIN);
    gpio_set_dir(BOTAO_B_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_B_PIN);
    
    configurar_interrupcoes();

    while (true) {
        int c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT) {
            if (c >= '0' && c <= '9') {
                int numero_atual = c - '0';
                exibir_numero(numero_atual, pio, sm, 0.5, 0.5, 0.5);
                char mensagem[20];
                sprintf(mensagem, "Numero: %d", numero_atual);
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, mensagem, 10, 20);
                ssd1306_send_data(&ssd);
                printf("Numero Digitado: %d\n", numero_atual);
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                char mensagem[20];
                sprintf(mensagem, "Letra: %c", (char)c);
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, mensagem, 10, 20);
                ssd1306_send_data(&ssd);
                printf("Letra Digitada: %c\n", (char)c);
            }
        }
        sleep_ms(10);
    }
    return 0;
}
