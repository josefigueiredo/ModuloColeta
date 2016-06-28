//testar se houve picos de tensao - implementado em 23/9
void testaSobreTensao(){
  //percorrer o vetor buscando valores acima de PIC_SUP_T e abaixo PIC_INF_T
  for(uint16_t i=0; i<AMOSTRAS; i++){
    if (vetorV[i] >= TENSAO_LIMIT_S || vetorV[i] <= TENSAO_LIMIT_I){
      //sendtoSocket(3, vetorV, 2, vetorA,'a');
      enviarPOST(3, vetorV, 2, vetorA,'a');
      Serial.println(F("Detectado sobre tensao"));
      break;
    }
  }
}
