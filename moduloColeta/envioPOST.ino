
//esta versao deve converter os dados para o formato POST (substitui o sendToSocket)
// e enviar pela rede a um servidor REST ou equivalente
void enviarPOST(byte vSensor, unsigned int *vToSend, byte iSensor, unsigned int *iToSend, char evento){
  /*--------------------------INICIO MONTAR PARAMAMETROS -----------------------------------*/
  char buffer[5];
  String strVolts,strCorr,valores;

  //parte da tenso (deve percorrer o vetor de tensao, convertern para string e concatenar com valores do POST
  for(uint8_t i=0; i<AMOSTRAS; i++){
    sprintf(buffer,"%d,",vToSend[i]); //converte valor da posiçao para char*
    strVolts += buffer;
    sprintf(buffer,"%d,",iToSend[i]); //converte valor da posiçao para char*
    strCorr += buffer;
  }
  valores = "t="+String(evento)+"&v="+strVolts+"&s="+String(iSensor)+"&c="+strCorr;
 
  
  /*--------------------------FIM MONTAR PARAMAMETROS -----------------------------------*/

  /*--------------------------ENVIA A COISA TODA      -----------------------------------*/
  if(!postPage(serverName,serverPort,pageName,(char*)valores.c_str())) Serial.print(F("falha no POST "));
  else Serial.print(F("foi.... "));    

}

//baseado em http://playground.arduino.cc/Code/WebClient
byte postPage(char* domainBuffer,int thisPort,char* page,char* thisData){
  int inChar;
  char outBuf[64];

  Serial.print(F("connecting..."));

  if(client.connect(domainBuffer,thisPort) == 1)
  {
    Serial.println(F("connected"));

    // send the header
    sprintf(outBuf,"POST %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",domainBuffer);
    client.println(outBuf);
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(thisData));
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
  } 
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 10000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
}


