// LEDs
const int grn = 7;
const int red = 5;
const int ylw = 6;

// Buttons
const int PB_on = 10;
const int PB_up = 9;
const int PB_down = 8;
int Btn_on = 0;
int Btn_up = 0;
int Btn_down = 0;

// Motor
const int enaPin = 13;
const int dirPin = 12;
const int pulPin = 11;

// Heights
const float fullyOpen = 100000;
const float fullyClsd = 0;
const float halfOpen_ = 15000;
float motorPosition = 0;

void serial_flush(){
  while(Serial.available()){
    int input = Serial.read(); 
  }
}
void raise(){
  digitalWrite(dirPin, HIGH);
}

void lower(){
  digitalWrite(dirPin, LOW);
}

void rotate_motor(){
  digitalWrite(pulPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(pulPin, LOW);
  delayMicroseconds(500);
}

void setup(){
  pinMode(grn, OUTPUT);
  pinMode(ylw, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(PB_up, INPUT);
  pinMode(PB_down, INPUT);
  pinMode(PB_on, INPUT);
  pinMode(enaPin, OUTPUT);
  pinMode(pulPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(19200);
}

void loop(){
  if(!Serial.available()){
    Btn_on = digitalRead(PB_on);
    Btn_up = digitalRead(PB_up);
    Btn_down = digitalRead(PB_down);
    if(Btn_on == HIGH|| Btn_up == HIGH || Btn_down == HIGH){
    if(Btn_on){
      digitalWrite(red, HIGH);
      digitalWrite(ylw, LOW);
      digitalWrite(grn, LOW);
    }
    else if(Btn_up){
      digitalWrite(red, LOW);
      digitalWrite(ylw, HIGH);
      digitalWrite(grn, LOW);
      if(motorPosition + 2000 <= fullyOpen){
        raise();
        for(int x = 0; x < 2000; x++){
          rotate_motor();
          motorPosition += 1;
        }
      }
      Serial.println(motorPosition);
      delay(50);
      digitalWrite(ylw, LOW);
    }
    else if(Btn_down){
      digitalWrite(red, LOW);
      digitalWrite(ylw, LOW);
      digitalWrite(grn, HIGH);
      if(motorPosition - 2000 >= fullyClsd){
        lower();
        for(int x = 0; x < 2000; x++){
          rotate_motor();
          motorPosition -= 1;
        }
      }
      Serial.println(motorPosition);
      delay(50);
      digitalWrite(grn, LOW);

    }
    while(Serial.available()){
      Serial.read();
    }
  }
  }
  while(Serial.available()){
  
  int temp3 = Serial.read();
  
  switch(temp3){
    case -1:
      break;
      
    case 45: // OFF
      digitalWrite(red, LOW); 
      digitalWrite(ylw, LOW);
      digitalWrite(grn, LOW);
      Serial.write(145);
      break; 
      
    case 48: //0 PG1
      Serial.write(65);
      digitalWrite(red, HIGH);
      digitalWrite(ylw, LOW);
      digitalWrite(grn, LOW);
      
      lower();
      for(motorPosition; motorPosition > fullyClsd; motorPosition--){
        rotate_motor();
      }
      digitalWrite(ylw, HIGH);
      delay(3000);
      raise();
      for(motorPosition; motorPosition < fullyOpen; motorPosition++){
        rotate_motor();
      }
      digitalWrite(grn, HIGH);
      Serial.write(68);
      break;
      
    case 49: //1 PG2
      Serial.write(66);
      digitalWrite(red, HIGH); 
      digitalWrite(ylw, LOW);
      digitalWrite(grn, LOW);
      
      lower();
      for(motorPosition; motorPosition > fullyClsd; motorPosition--){
        rotate_motor();
      }
      digitalWrite(ylw, HIGH);
      delay(4000);
      raise();
      for(motorPosition; motorPosition < fullyOpen; motorPosition++){
        rotate_motor();
      }
      
      digitalWrite(grn, HIGH);
      Serial.write(69);
      break;
      
    case 50: //2 PG3
      Serial.write(67);
      digitalWrite(red, HIGH); 
      digitalWrite(ylw, LOW);
      digitalWrite(grn, LOW);
      
      lower();
      for(motorPosition; motorPosition > fullyClsd; motorPosition--){
        rotate_motor();
      }
      digitalWrite(ylw, HIGH);
      delay(5000);
      raise();
      for(motorPosition; motorPosition < fullyOpen; motorPosition++){
        rotate_motor();
      }
      
      digitalWrite(grn, HIGH);
      Serial.write(70);
      break;
      
    case 51: //3 UP
      digitalWrite(red, LOW); 
      digitalWrite(ylw, HIGH);
      digitalWrite(grn, LOW);
      
      raise();
      if(motorPosition+2000 <= fullyOpen){
        for(int x = 0; x < 2000; x++){
          rotate_motor();
          motorPosition += 1;
        }
      }
      
      Serial.write(151);
      break;
      
    case 52: //4 DOWN
      digitalWrite(red, LOW); 
      digitalWrite(ylw, HIGH);
      digitalWrite(grn, LOW);
      
      lower();
      if(motorPosition-2000 >= fullyClsd){
        for(int x = 0; x < 2000; x++){
          rotate_motor();
          motorPosition -= 1;
        }
      }
      
      Serial.write(152);
      break;
      
    case 53: //5 STOP
      digitalWrite(red, LOW); 
      digitalWrite(ylw, HIGH);
      digitalWrite(grn, LOW);
      Serial.write(153);
      break;
      
    case 54: //6 FO
      digitalWrite(red, LOW); 
      digitalWrite(ylw, LOW);
      digitalWrite(grn, HIGH);
      Serial.write(154);
      
      raise();
      while(motorPosition < fullyOpen){
        rotate_motor();
        motorPosition += 1;
      }
      
      Serial.write(84);
      break;
      
    case 55: //7 FC
      digitalWrite(red, LOW); 
      digitalWrite(ylw, LOW);
      digitalWrite(grn, HIGH);
      Serial.write(155);
      
      lower();
      while(motorPosition > fullyClsd){
        rotate_motor();
        motorPosition -= 1;
      }
      
      Serial.write(84);
      break;
      
    case 56: //8 HO
      digitalWrite(red, LOW);
      digitalWrite(ylw, LOW);
      digitalWrite(grn, HIGH);
      Serial.write(156);
      
      if(motorPosition < halfOpen_){
        raise();
        while(motorPosition < halfOpen_){
          rotate_motor();
          motorPosition += 1;
        }
      }
      else if(motorPosition > halfOpen_){
        lower();
        while(motorPosition > halfOpen_){
          rotate_motor();
          motorPosition -= 1;
        }
      }
      
      Serial.write(84);
      break;
      
    case 57: //9 NC
      digitalWrite(red, HIGH);
      digitalWrite(ylw, HIGH);
      digitalWrite(grn, HIGH);
      Serial.write(157);
      break;
      
    default:
      break;
  }
  delay(2);
  }

}

