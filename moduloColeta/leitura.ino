void fazLeitura(){
  unsigned int accV=0,accA=0,accmA=0,CorrmARMS=0,VoltsRMS=0;
  float mediaV=0.0,mediaA=0.0,mediamA=0.0,CorrRMS=0,accVflt=0,accAflt=0,accmAflt=0;

  //laço para coleta dos valores 
  // este laço faz a leitura sequncial dos tres sensores da rede..
  // o primeiro para correntes acima de 1A
  // o segundo para correntes abaixo de 1A
  // o terceiro para tensao
  // as variaveis acc* so responsaveis por acumular o valor de cada leitura (sera usado mais tarde para remocao do nivel DC)
  for(uint16_t i=0; i<AMOSTRAS; i++){
    vetorA[i] = analogRead(sensorA);
    vetormA[i] = analogRead(sensormA);
    vetorV[i] = analogRead(sensorV);
    accA += vetorA[i];
    accmA += vetormA[i];
    accV += vetorV[i];
    //este delay eh necessario para ajustar o tempo de cada captura [ este tempo eh calculado na inicializaço ]
    delayMicroseconds(tAmostraMinusADTime);
  }
  //calcula a media de cada vetor para remoçao do nivel DC
  mediaA = (float)accA / AMOSTRAS;
  mediamA = (float)accmA / AMOSTRAS;
  mediaV = (float)accV / AMOSTRAS;  

  //remove o nivel DC de cada amostra
  for(uint8_t i=0;i<AMOSTRAS;i++){
    vetorASemDC[i] = (float)vetorA[i] - mediaA;
    vetormASemDC[i] = (float)vetormA[i] - mediamA;
    vetorVSemDC[i] = (float)vetorV[i] - mediaV;
  }

  //debug pra leitura do sensor acima de 1A
  if(Adbg == true){
    Serial.print("A,");
    for(uint8_t i=0;i<AMOSTRAS;i++){
      Serial.print(vetorASemDC[i]); 
      Serial.print(",");
    }
    Serial.println();
  }
  //debug pra leitura do sensor abaixo de 1A
  if(mAdbg == true){
    Serial.print("mA,");
    for(uint8_t i=0;i<AMOSTRAS;i++){
      Serial.print(vetormASemDC[i]); 
      Serial.print(",");
    }
    Serial.println();
  }
  //debug pra leitura do sensor de tensao  
  if(Vdbg == true){
    Serial.print("V,");
    for(uint8_t i=0;i<AMOSTRAS;i++){
      Serial.print(vetorVSemDC[i]); 
      Serial.print(",");
    }
    Serial.println();
  }
  //debug pra overflow (nao sei o que e isso)
  if(overflowDBG == true ){
    Serial.println("Tensao: ");
    for(uint16_t i=0; i<AMOSTRAS; i++){
      Serial.print(vetorV[i]);
      Serial.print(",");
    }
  }

  //somatorio dos quadrados
  //para calculo RMS
  for(uint8_t i=0;i<AMOSTRAS;i++){
    accAflt += (vetorASemDC[i] * vetorASemDC[i]);
    accmAflt += (vetormASemDC[i] * vetormASemDC[i]);    
    accVflt += (vetorVSemDC[i] * vetorVSemDC[i]);
  }

  //raiz quadrada do somatorio dos quadrados peloa raiz do numero de amostras
  CorrRMS = sqrt(accAflt/AMOSTRAS)/ganhoA;
  CorrmARMS = (sqrt(accmAflt/AMOSTRAS)/ganhomA)*1000; //multiplica por 1000 para mostrar em mA
  VoltsRMS = (sqrt(accVflt/AMOSTRAS)/ganhoV)*224900; //divide pelo valor do resistor para encontrar a tenso

  //se Sdbg is true envia valores RMS pela serial tmb.
  if(Sdbg == true){
    Serial.print(CorrRMS);
    Serial.print(" A, ");
    Serial.print(CorrmARMS);
    Serial.print(" mA, ");
    Serial.print(VoltsRMS);
    Serial.println(" V");

  }
  //SE o RMS calculado  maior que 999mA descarta a medida e assume  sensor de alta amperagem.
  //imprime a coisa
  if(CorrmARMS > 1000){
    atualizaDisplay(CorrRMS,VoltsRMS, 0.86);
    testaAlteracaoRMS(CorrRMS,VoltsRMS);
  }
  else{
    atualizaDisplaymA(CorrmARMS,VoltsRMS, 0.86);
    testaAlteracaomARMS(CorrmARMS,VoltsRMS);
  }
}

