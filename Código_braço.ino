#include <MobaTools.h>
#include <Adafruit_NeoPixel.h>

#define PIN 46        // Pino ao qual o WS2812B está conectado
#define NUM_LEDS 46   // Número de LEDs na fita

// Define os motores de passo
MoToStepper motorBase(200, STEPDIR);  // Motor da base do braço, 200 passos por revolução
MoToStepper motorArmY(200, STEPDIR);   // Motor do braço, 200 passos por revolução
MoToStepper motorArmZ(200, STEPDIR);

const int enablePin = 8;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  pinMode(enablePin, OUTPUT);
  
  // // Habilita o driver do motor (ativo baixo)
  digitalWrite(enablePin, LOW);


  // Anexa os motores de passo aos pinos de controle
  motorBase.attach(2, 5);   // Pino 2 para STEP e 3 para DIR da base
  motorArmY.attach(3, 6);    // Pino 4 para STEP e 5 para DIR do braço
  motorArmZ.attach(4, 7);

  // Configura a velocidade dos motores de passo
  motorBase.setSpeed(100);  // Velocidade do motor da base em passos por segundo
  motorArmY.setSpeed(50);   // Velocidade do motor do braço em passos por segundo
  motorArmZ.setSpeed(50);

/*// Anexa o servo motor ao pino de controle
  servoClaw.attach(9);  // Pino 9 para o servo da garra

  // Define o ângulo inicial da garra
  servoClaw.write(0);   // Garra totalmente fechada
*/
  strip.begin();
  strip.show(); // Inicializa todos os LEDs apagados
}



void loop() {

  ligar_led_verde();
  setBrightnessForAll(strip, strip.Color(255, 0, 0), 255);

  motorBase.move(200);  // Move o motor da base 200 passos para frente

  delay(5000);

  motorArmY.move(-150);   // Move o motor do braço 100 passos para frente
  motorArmZ.move(150);

  delay(10000);


  motorArmY.move(150);  // Move o motor do braço 100 passos para trás
  motorArmZ.move(-150);

  delay(10000);   

  motorBase.move(-200); // Move o motor da base 200 passos para trás
  // Fecha a garra
  // servoClaw.write(0);   // Move o servo da garra para 0 graus (fechada)
  delay(6000);

  setBrightnessForAll(strip, strip.Color(0, 255, 0), 255);

  delay(4000);

}

void ligar_led_verde() {
  int duration = 1000;
  for (int i = 0; i < 3; i++)
  {
    fadeInOut(strip.Color(0, 255, 0), duration);
    delay(1000); 
  }
  
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

