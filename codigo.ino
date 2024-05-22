/*
   _____     __     __  __     ______   ______     ______     __  __
  /\  __-.  /\ \   /\ \_\ \   /\__  _\ /\  ___\   /\  ___\   /\ \_\ \
  \ \ \/\ \ \ \ \  \ \____ \  \/_/\ \/ \ \  __\   \ \ \____  \ \  __ \
   \ \____-  \ \_\  \/\_____\    \ \_\  \ \_____\  \ \_____\  \ \_\ \_\
    \/____/   \/_/   \/_____/     \/_/   \/_____/   \/_____/   \/_/\/_/

  DIY-TECH
  by Giovanni Lerda
  https://www.diy-technology.com

*/

/*
  Note:
  This sketch is developed on the basis of the wiring diagram
  that you can find on the KAUDA project website.
  https://www.diy-technology.com/010-kauda

  This example allows you to test the command of the stepper 
  motors through commands launched from the serial port 
  (to which the Arduino board is connected).
  
*/


//Enable pin - DON'T MODIFY
#define EN        8
//Direction pin - DON'T MODIFY
#define X_DIR     5
#define Y_DIR     6
#define Z_DIR     7
//Step pin - DON'T MODIFY
#define X_STP     2
#define Y_STP     3
#define Z_STP     4

//Steps number
int steps = 100; // 100 = 1, 50 = 0.5

// (motor x, motor z)
float target_coordinates[3];
float actual_coordinates[3];

/*
  Note:
  If you increase the value in the steps variable you will increase accordingly
  the speed with which the steps are performed.
*/
float data;

void setup()
{
  Serial.begin(9600);

  // Pin configuration.
  // DON'T MODIFY.
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(EN, OUTPUT);

  digitalWrite(X_DIR, LOW); // Direção de volta para a direita.
  digitalWrite(Z_DIR, HIGH);

}

void set_target(float x, float y) {
  target_coordinates[0] = steps * x;
  target_coordinates[1] = steps * y;

  Serial.println("--- Target coordinates ---");
  Serial.println("X: " + String(target_coordinates[0]));
  Serial.println("Y: " + String(target_coordinates[1]));
  Serial.println("--------------------------");
}


void move_Base() {

  bool right;

  // Motor da base | HIGH = Direita / LOW = Esquerda
  if (actual_coordinates[0] > target_coordinates[0]) {
    digitalWrite(X_DIR, LOW);
    right = false;
  } else if (actual_coordinates[0] < target_coordinates[0]) {
    digitalWrite(X_DIR, HIGH);
    right = true;
  }
  
  while (actual_coordinates[0] != target_coordinates[0]) {
    digitalWrite(X_STP, HIGH); // Execution of a step.
    delayMicroseconds(1500);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(1500);

    if (right) {
      actual_coordinates[0]++;
    } else {
      actual_coordinates[0]--;
    }
  }
}


void move_Joint1() {

  bool down;

  // Motores do cotovelo 1 | HIGH = Baixo / LOW = Cima
  if (actual_coordinates[1] > target_coordinates[1]) {
    digitalWrite(Y_DIR, HIGH);
    digitalWrite(Z_DIR, LOW);
    down = false;
  } else if (actual_coordinates[1] < target_coordinates[1]) {
    digitalWrite(Y_DIR, LOW);
    digitalWrite(Z_DIR, HIGH);
    down = true;
  }
  
  while (actual_coordinates[1] != target_coordinates[1]) {
    digitalWrite(Y_STP, HIGH); 
    digitalWrite(Z_STP, HIGH);
    delayMicroseconds(1500);
    digitalWrite(Z_STP, LOW);
    digitalWrite(Y_STP, LOW);
    delayMicroseconds(1500);

    if (down) {
      actual_coordinates[1]++;
    } else {
      actual_coordinates[1]--;
    }
  }
}

void move_block1() {
  set_target(2,2);

  digitalWrite(EN, LOW); 

  digitalWrite(Y_DIR, LOW); // Motores do cotovelo 1 | HIGH = Baixo / LOW = Cima
  digitalWrite(Z_DIR, HIGH); 
  digitalWrite(X_DIR, HIGH); // Motor da base | HIGH = Direita / LOW = Esquerda

  while (actual_coordinates[0] != target_coordinates[0]){
    digitalWrite(X_STP, HIGH); // Execution of a step.
    delayMicroseconds(1500);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(1500);

    actual_coordinates[0]++;

    if (actual_coordinates[1] != target_coordinates[1]){
      digitalWrite(Y_STP, HIGH); 
      digitalWrite(Z_STP, HIGH);
      delayMicroseconds(1500);
      digitalWrite(Z_STP, LOW);
      digitalWrite(Y_STP, LOW);
      delayMicroseconds(1500);

      actual_coordinates[1]++;
    }
  }

  set_target(0,0);
  delay(1000);

  digitalWrite(Y_DIR, HIGH);
  digitalWrite(Z_DIR, LOW);
  digitalWrite(X_DIR, LOW); // Motor da base

  while (actual_coordinates[0] != target_coordinates[0]){
    digitalWrite(X_STP, HIGH); // Execution of a step.
    delayMicroseconds(1500);
    digitalWrite(X_STP, LOW);
    delayMicroseconds(1500);

    actual_coordinates[0]--;

    if (actual_coordinates[1] != target_coordinates[1]){
      digitalWrite(Y_STP, HIGH); 
      digitalWrite(Z_STP, HIGH);
      delayMicroseconds(1500);
      digitalWrite(Z_STP, LOW);
      digitalWrite(Y_STP, LOW);
      delayMicroseconds(1500);

      actual_coordinates[1]--;
    }

  }
  // Colocar os motores em standby
  digitalWrite(EN, HIGH);
}


void hello_world() {
  digitalWrite(EN, LOW);
  
  set_target(2,0);
  move_Base();

  delay(1000);
  set_target(0,0);
  move_Base();

  delay(1000);
  set_target(0,2);
  move_Joint1();

  delay(1000);
  set_target(0,0);
  move_Joint1();

  digitalWrite(EN, HIGH);
}

void loop() {
  hello_world();
  delay(2000);
  move_block1();
  delay(2000);
}

// usar milis 

/*void loop() {

  if (Serial.available() > 0)
  {
    data = Serial.parseInt();

    if (data > 0) {

      digitalWrite(EN, LOW); 
    
      target_coordinates[0] = steps * data;
      target_coordinates[1] = steps * data;
      Serial.println("-----------------------");
    
      Serial.println("X: " + String(target_coordinates[0]));
      Serial.println("Y: " + String(target_coordinates[1]));

      digitalWrite(Y_DIR, LOW); 
      digitalWrite(Z_DIR, HIGH);
      digitalWrite(X_DIR, HIGH); // Motor da base.

      while (actual_coordinates[0] != target_coordinates[0]){
        digitalWrite(X_STP, HIGH); // Execution of a step.
        delayMicroseconds(1500);
        digitalWrite(X_STP, LOW);
        delayMicroseconds(1500);

        actual_coordinates[0]++;

        if (actual_coordinates[1] != target_coordinates[1]){
          digitalWrite(Y_STP, HIGH); 
          digitalWrite(Z_STP, HIGH);
          delayMicroseconds(1500);
          digitalWrite(Z_STP, LOW);
          digitalWrite(Y_STP, LOW);
          delayMicroseconds(1500);

          actual_coordinates[1]++;
        }
      }

      delay(1000);
    
      target_coordinates[0] = 0;
      target_coordinates[1] = 0;
      Serial.println("-----------------------");

      Serial.println("X: " + String(target_coordinates[0]));
      Serial.println("Y: " + String(target_coordinates[1]));

      digitalWrite(Y_DIR, HIGH);
      digitalWrite(Z_DIR, LOW);
      digitalWrite(X_DIR, LOW); // Motor da base

      while (actual_coordinates[0] != target_coordinates[0]){
        digitalWrite(X_STP, HIGH); // Execution of a step.
        delayMicroseconds(1500);
        digitalWrite(X_STP, LOW);
        delayMicroseconds(1500);

        actual_coordinates[0]--;

        if (actual_coordinates[1] != target_coordinates[1]){
          digitalWrite(Y_STP, HIGH); 
          digitalWrite(Z_STP, HIGH);
          delayMicroseconds(1500);
          digitalWrite(Z_STP, LOW);
          digitalWrite(Y_STP, LOW);
          delayMicroseconds(1500);

          actual_coordinates[1]--;
        }

      }
      // Colocar os motores em standby
      digitalWrite(EN, HIGH);

    }
  }
}*/


/*

  Attention!

  The A axis is not indicated as it will execute the same movements (in parallel)
  that the axis that is selected through the hardware configuration of the CNC Shield performs.
  For more information:https://www.handsontec.com/dataspecs/cnc-3axis-shield.pdf

  This sketch does NOT include the command of the servomotors.
*/
