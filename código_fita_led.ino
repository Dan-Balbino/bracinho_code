#include <Adafruit_NeoPixel.h>

#define PIN 46        // Pino ao qual o WS2812B está conectado
#define NUM_LEDS 46   // Número de LEDs na fita

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Inicializa todos os LEDs apagados
}

void loop() {
  // Alterna entre vermelho, verde, azul, verde e roxo
  alternateColors();
}

// Função para alternar entre as cores com transição suave
void alternateColors() {
  int duration = 5000;  // Duração para cada ciclo completo (5 segundos)

  fadeInOut(strip.Color(255, 0, 0), duration);  // Vermelho
  fadeInOut(strip.Color(0, 255, 0), duration);  // Verde
  fadeInOut(strip.Color(0, 0, 255), duration);  // Azul
  fadeInOut(strip.Color(0, 255, 0), duration);  // Verde (novamente)
  fadeInOut(strip.Color(128, 0, 128), duration);  // Roxo
}

// Função para fazer a transição suave de brilho de uma cor
void fadeInOut(uint32_t color, int cycleDuration) {
  int fadeSpeed = 5;  // Velocidade de transição (quanto menor o valor, mais lenta a transição)
  int stepCount = 255 / fadeSpeed;  // Número de passos

  // Suave aumento de brilho
  for (int brightness = 0; brightness <= 255; brightness += fadeSpeed) {
    setBrightnessForAll(strip, color, brightness);
    delay(cycleDuration / (2 * stepCount));  // Tempo para cada passo
  }

  // Suave diminuição de brilho
  for (int brightness = 255; brightness >= 0; brightness -= fadeSpeed) {
    setBrightnessForAll(strip, color, brightness);
    delay(cycleDuration / (2 * stepCount));  // Tempo para cada passo
  }
}

// Função auxiliar para definir a intensidade de uma cor em todos os LEDs
void setBrightnessForAll(Adafruit_NeoPixel &strip, uint32_t color, int brightness) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  // Ajusta a cor com base no brilho
  r = (r * brightness) / 255;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;

  // Define a cor ajustada em todos os LEDs
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}