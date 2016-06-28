/* ESTA FUNÇAO DEVERA SER SUBSTITUDA PELA ENVIOPORPOST*/
//esta versao envia 2 vetores (tensao e corrente)
//funçao socekt tcp
// os parametros sao: (numero do sensor, vetor de tensao, numero sensor corrente, vetor corrente, tipo evento) 
void sendtoSocket(byte vSensor, unsigned int vToSend[AMOSTRAS], byte iSensor, unsigned int iToSend[AMOSTRAS], char evento){
  if(client.connect(server,10002)){
    Serial.println("-> Conectado.");
    //retirei o envio do numero do sensor de tensao [para esta versao eh sempre o mesmo]
    //sprintf(tmpBuf,"%d:",vSensor);
    //client.print(tmpBuf); //envia nome do sensor
    client.print(evento); //envia tipo do evento pelo socket
    client.print(":"); //envia separador pelo socket

      //percore o vetor de tensao para enviar todo pelo socket
    for(uint8_t i=0; i<AMOSTRAS; i++){
      sprintf(tmpBuf,"%d,",vToSend[i]); //converte valor da posiçao para char*
      client.print(tmpBuf); // envia pelo socket
    }
    sprintf(tmpBuf,":%d:",iSensor); ////converte numero do sensor para char* (concatena com separador :)
    client.print(tmpBuf); //envia pelo socket
    //percore o vetor de corrente para enviar todo pelo socket
    for(uint8_t i=0; i<AMOSTRAS; i++){
      sprintf(tmpBuf,"%d,",iToSend[i]); //converte valor da posiçao para char*
      client.print(tmpBuf); // envia pelo socket
    }
  }
  else{
    // mensagem de erro se no conectar ao servidor..
    Serial.println("-> Falha de conexao.");

  }
  client.stop(); //encerra a comunicaçao
}


