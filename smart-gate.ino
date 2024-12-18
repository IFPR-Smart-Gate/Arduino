
// algumas bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

// Definiremos o id que sera liberado o acesso

#define ID "93 62 7D 4F"

//define alguns pinos do esp32 que serao conectados aos modulos e componentes
#define LedVerde 2
#define LedVermelho 15
#define tranca 2
#define buzzer 15
#define SS_PIN 21
#define RST_PIN 22



MFRC522 mfrc522(SS_PIN, RST_PIN);  // define os pinos de controle do modulo de leitura de cartoes RFID

void setup() {
  SPI.begin();  // inicia a comunicacao SPI que sera usada para comunicacao com o mudulo RFID


  mfrc522.PCD_Init();  //inicia o modulo RFID

  Serial.begin(115200);  // inicia a comunicacao serial com o computador na velocidade de 115200 baud rate

  Serial.println("RFID + ESP32");
  Serial.println("Passe alguma tag RFID para verificar o id da mesma.");

  // define alguns pinos como saida
  pinMode(LedVerde, OUTPUT);
  pinMode(LedVermelho, OUTPUT);
  pinMode(tranca, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  
  delay(500);


  Serial.println("Agardando leitura RFID");     // imprime na primeira linha a string "Aguardando"

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;  // se nao tiver um cartao para ser lido recomeça o void loop
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;  //se nao conseguir ler o cartao recomeça o void loop tambem
  }

  String conteudo = "";  // cria uma string

  Serial.print("id da tag :");  //imprime na serial o id do cartao

  for (byte i = 0; i < mfrc522.uid.size; i++) {  // faz uma verificacao dos bits da memoria do cartao
    //ambos comandos abaixo vão concatenar as informacoes do cartao...
    //porem os 2 primeiros irao mostrar na serial e os 2 ultimos guardarao os valores na string de conteudo para fazer as verificacoes
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  
  conteudo.toUpperCase();  // deixa as letras da string todas maiusculas

  Serial.println("\n---------------------------------------");
  Serial.println(conteudo.substring(1));
  Serial.println("\n---------------------------------------");


  if (conteudo.substring(1) == ID){ // verifica se o ID do cartao lido tem o mesmo ID do cartao que queremos liberar o acesso
 
      digitalWrite(LedVerde, HIGH);            // ligamos o led verde
      Serial.println("Acesso Liberado");            // informamos pelo lcd que a tranca foi aberta
 
 
      for(byte s = 10; s > 0; s--){             //vai informando ao usuario quantos segundos faltao para a tranca ser fechada
        Serial.println(s);
        delay(1000);
      }
 
      digitalWrite(tranca, LOW);               // fecha a tranca
      digitalWrite(LedVerde, LOW);             // e desliga o led
 
  } else {
      
      digitalWrite(LedVermelho, HIGH); 
      Serial.println("Acesso Negado");            // informamos pelo lcd que a tranca foi aberta
      
      // ligamos o led verde
      delay(5000);
      digitalWrite(LedVermelho, LOW);             // e desliga o led
  }

}
