

// o limite para teste de mA de 0.10A
// esta funçao serve para detectar se houve alteraçao do valor RMS
// o teste  aplicado a cada 3 leituras (limiteDiferencas=3;)
// se 3 leituras de corrente tiverem uma diferença maior que 100mA entao temos um evento
void testaAlteracaoRMS(float newRMS, float volts){
  //Quando teste der 'positivo' n vezes zera contador e envia esta amostra
  float dif = newRMS - rmsAnterior;
  if(abs(dif) > 0.10){
    numVezesDiferente++;
    if(rmsTestdbg == true){
      Serial.println(numVezesDiferente);
    }
    //se testou n vezes e deu diferença entao tem que alterar.
    if(numVezesDiferente >= limiteDiferencas){
      numVezesDiferente=0;
      rmsAnterior = newRMS;
      //sendtoSocket(3, vetorV, 2, vetorA,testaDif(dif));
      enviarPOST(3, vetorV, 2, vetorA,testaDif(dif));
      Serial.print("Detectado um evento");

      if(rmsTestdbg == true){
        Serial.println("RMS Alterado");
      }
    }
  }
  else{
    numVezesDiferente=0;
  }
}


// idem para funcao anterior mas limite para teste de mA eh de 50mA 
void testaAlteracaomARMS(float newRMS, float volts){
  //Quando teste der 'positivo' n vezes zera contador e envia esta amostra
  float dif = newRMS - rmsAnterior;
  if(abs(dif) > 50){
    numVezesDiferente++;
    if(rmsTestdbg == true){
      Serial.println(numVezesDiferente);
    }
    //se testou n vezes e deu diferença entao tem que alterar.
    if(numVezesDiferente >= limiteDiferencas){
      numVezesDiferente=0;
      rmsAnterior = newRMS;
      //sendtoSocket(3, vetorV, 1, vetormA,testaDif(dif));  
      enviarPOST(3, vetorV, 1, vetormA,testaDif(dif));  
 
      if(rmsTestdbg == true){
        Serial.println("RMS Alterado");
      }
    }
  }
  else{
    numVezesDiferente=0;
  }
}
