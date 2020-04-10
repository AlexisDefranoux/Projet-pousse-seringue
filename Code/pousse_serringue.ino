#include <LiquidCrystal.h>
#include <Keypad.h>
 
#define BAUD (9600)
int x;
 
//Pin LCD
LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);

//Pin Keypad
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'X','0','=','/'},
};
byte rowPins[ROWS] = {53,51,49,47}; //connect to row pinouts 
byte colPins[COLS] = {45,43,41,39}; //connect to column pinouts
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
 
boolean valOnePresent = false;
boolean next = false;
boolean final = false;
boolean calcul_termine = false;
String num1, num2;
int ans = 200;
int direction = 0;
char op;
 
void setup(){

  //Set Moteur
  pinMode(6,OUTPUT); // Enable
  pinMode(5,OUTPUT); // Step
  pinMode(4,OUTPUT); // Dir
  digitalWrite(6, HIGH); // Set Enable low

  //Set LCD
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Debit Volumique");
  delay(2000);
  lcd.clear(); //clears the LCD screen and positions the cursor in the upper-left corner. 
  Serial.begin(BAUD);
}    
 
void loop(){
  char key = myKeypad.getKey();
 
  if ((calcul_termine == false) && key != NO_KEY && (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0')){
    if (valOnePresent != true){
      num1 = num1 + key;
      int numLength = num1.length();
      lcd.setCursor(15 - numLength, 0); //to adjust one whitespace for operator
      lcd.print(num1);
    }
    else {
      num2 = num2 + key;
      int numLength = num2.length();
      lcd.setCursor(15 - numLength, 1);
      lcd.print(num2);
      final = true;
    }
  }
  else if ((calcul_termine == false) && (valOnePresent == false) && (key != NO_KEY) && (key == '/' || key == '*' || key == '-' || key == '+')){
    if (valOnePresent == false){
      valOnePresent = true;
      op = key;
      lcd.setCursor(15,0); //operator on right corner
      lcd.print(op);
    }
  }
 
  if ((calcul_termine == false) && (final == true) && (key != NO_KEY) && (key == '=')) {
    if (op == '+'){
        ans = num1.toInt()*425  ;
        //1ml = 468pas et Dvmoteur = 0,2ml/s
    }    
    lcd.clear();
    lcd.setCursor(15,0);
    lcd.autoscroll();
    lcd.print(ans);
    lcd.noAutoscroll();
    calcul_termine = true;
    valOnePresent = false;
    final = false;
  }
   
  if (calcul_termine) {
    //lcd.print(Saisir la direction:
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Direction");
    key = NO_KEY;
    while ( key == NO_KEY) {
    key = myKeypad.getKey();
    }
    if (key == '+') { 
    direction = 1;

    }
    else if (key == '-') {
      direction = 2;  
    }
  }
  if ((key != NO_KEY) && (key == 'X')){
    lcd.clear();
    valOnePresent = false;
    final = false;
    num1 = "";
    num2 = "";
    ans = 0;
    op = ' ';
  }
   
  if (direction == 1){
    lcd.clear();
    lcd.print("Direction +ve");
 
    digitalWrite(6,LOW); // Set Enable low
    digitalWrite(4,HIGH); // Set Dir high
    for(int x = 0; x < ans; x++) // Loop 200 times
    {
 
      digitalWrite(5,HIGH); // Output high
      delay(1); // Wait
      digitalWrite(5,LOW); // Output low
      delay(1); // Wait
    }
    delay(1000); // pause one second
    direction = 0; 
    calcul_termine = false;
    lcd.clear();   
    num1 = "";
    num2 = "";
    ans = 0;
    op = ' ';  
    digitalWrite(6,HIGH); // Set Enable low
  }
 
  else if (direction == 2){
    lcd.clear();
    lcd.print("Direction -ve"); 
 
    digitalWrite(6,LOW); // Set Enable low
    digitalWrite(4,LOW); // Set Dir hig
    for(int x = 0; x < ans; x++) // Loop 200 times
    {

      digitalWrite(5,HIGH); // Output high
      delay(1); // Wait
      digitalWrite(5,LOW); // Output low
      delay(1); // Wait
    }
    delay(1000); // pause one second
    direction = 0;
    calcul_termine = false;
    lcd.clear();
    num1 = "";
    num2 = "";
    ans = 0;
    op = ' ';
    //lcd.setCursor(0,0);  
    digitalWrite(6,HIGH); // Set Enable low
  }               
}
