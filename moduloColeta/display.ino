//**********************************
//************* funcoes para display
//**********************************
void initDisplay(){
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Inicializando:");
  for(byte i=0;i<14;i++){
    lcd.setCursor(i,1);
    lcd.print(".");
    delay(100);
  }
  lcd.setCursor(14,1);
  lcd.print("ok");
  delay(500);

  limparDisplay();
}

void atualizaDisplay(float iRMS,unsigned int vRMS, float fp){
  lcd.setCursor(0,1);
  lcd.print("                ");


  //mostra corrente
  lcd.setCursor(6,1);
  lcd.print("/I=");
  lcd.print(iRMS);
  lcd.print("A");

  //mostra tensao
  lcd.setCursor(0,1);
  lcd.print("V=");
  lcd.print(vRMS);
  lcd.print("V");

  ////mostra consumo
  //lcd.setCursor(9,1);
  //lcd.print("Cons:");


  //mostra fatorPotencia
  //lcd.setCursor(9,1);
  //lcd.print("FP:");
  //lcd.print(fp);
}

void atualizaDisplaymA(int iRMS,unsigned int vRMS, float fp){
  lcd.setCursor(0,1);
  lcd.print("                ");
  //mostra corrente
  lcd.setCursor(6,1);
  lcd.print("/I=");
  lcd.print(iRMS);
  lcd.print("mA");

  //mostra tensao
  lcd.setCursor(0,1);
  lcd.print("V=");
  lcd.print(vRMS);
  lcd.print("V");

  //mostra consumo
  //lcd.setCursor(9,1);
  //lcd.print("Cons:");

  //mostra fatorPotencia
  // desativado em 14/1 (fotos dissertaçao)
  //lcd.setCursor(9,1);
  //lcd.print("FP:");
  //lcd.print(fp);
}

void limparDisplay(){
  //limpa o display
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");

  lcd.setCursor(0,0);
  lcd.print(" Prototipo MCCT ");
}


