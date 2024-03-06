// Programa : RFID - Controle de acesso
// Autor : Letícia Fritz

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <EEPROM.h>


#define SS_PIN 10
#define RST_PIN 9
// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Relé de acesso
int RELE = 8;


byte valor;
char st[20];
String cartoesCadastrados = "";
String master = "9B EC 00 16";

int marcadorNovoCartao = 0;

void setup()
{
  // Inicia a serial
  Serial.begin(9600);

  pinMode(RELE, OUTPUT);

  //Verifica espaço disponível
  Serial.print("Espaco Disponivel em Bytes: "); // Mostra a quantidade  de memória da EEPROM do seu microcontrolador
  Serial.println(EEPROM.length());

  // Inicia  SPI bus
  SPI.begin();
  // Inicia MFRC522
  mfrc522.PCD_Init();
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  carregarCartoesCadastrados();

}

void carregarCartoesCadastrados() {
  Serial.println("Carregando cartoes cadastrados...");

  for (int i = 0; i < 1024; i++) {
    int aux = EEPROM.read(i);
    if (aux != 255) {
      String cartao = "";
      char auxChar;
      int espacoBranco = 0;
      if (EEPROM.read(i) != 0) {
        do {
          auxChar = char(EEPROM.read(i));
          cartao = cartao + auxChar;
          //Colocar " " (espaço em branco) a cada 2 letras
          espacoBranco ++;
          if (espacoBranco >= 2) {
            cartao = cartao + " ";
            espacoBranco = 0;
          }
          i++;
        } while (auxChar != ';');
        i--;
      }

      Serial.print("Cartao: ");
      Serial.println(cartao);
      cartoesCadastrados = cartoesCadastrados + cartao;
      Serial.print("Lista de Cartoes: ");
      Serial.println(cartoesCadastrados);
    }
    Serial.println();
  }
}

void liberarAcesso() {
  // Levanta a cancela
  digitalWrite(RELE, HIGH);
  Serial.println("Cartao1 - Acesso liberado !");
  Serial.println();
  
  delay(2000);
  digitalWrite(RELE, LOW);
}

void cadastrarNovoCartao(String cartao) {
  Serial.print("Adicionou o cartao ");
  Serial.print(cartao);
  Serial.println(" na lista");

  cartoesCadastrados = cartoesCadastrados + cartao;

  //remove os espacos em branco do cartao e adiciona o ';'
  cartao.replace(" ", "");

  //Adicionar o novo cartão na memória
  for (int i = 0; i < 1024; i++) {

    int aux = EEPROM.read(i);

    if (aux == 255) {
      for (int j = 0; j < cartao.length(); j++) {
        EEPROM.write(i, byte(cartao.charAt(j)));
        i++;
      }
      break;
    }
  }

}

void removerCartao(String cartao) {
  
  Serial.print("Removeu o cartao ");
  Serial.print(cartao);
  Serial.println(" da lista");

  cartoesCadastrados.replace(cartao, "");
  Serial.print("Cartoes atuais: ");
  Serial.println(cartoesCadastrados);


  //Remove o cartão na memória
  String auxCartoesCadastrados = cartoesCadastrados;
  auxCartoesCadastrados.replace(" ", "");
  for (int i = 0; i < 1024; i++) {

    if (i < auxCartoesCadastrados.length()) {
      EEPROM.write(i, byte(auxCartoesCadastrados.charAt(i)));
    } else {
      EEPROM.write(i, 255);
    }
  }
}

//Quando o cartao MASTER é passado.
void alterarCartoes() {
  Serial.println("Master Reconhecido!");
  Serial.println();

  // Aguarda a aproximacao do cartao
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Seleciona um dos cartoes
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  // Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  String novoCartao = conteudo.substring(1);
  novoCartao.toUpperCase();
  novoCartao.trim();
  novoCartao = novoCartao + " ;";

  if (conteudo.length() > 0 && novoCartao.indexOf(master) < 0) {
    if (cartoesCadastrados.indexOf(novoCartao) < 0) {
      cadastrarNovoCartao(novoCartao);
    } else {
      removerCartao(novoCartao);
    }
    marcadorNovoCartao = 0;

    delay(2000);
  }

  Serial.print("Lista de Cartoes: ");
  Serial.println(cartoesCadastrados);
}

void loop() {

  delay(500);

  Serial.print("marcadorNovoCartao: ");
  Serial.println(marcadorNovoCartao);
  if (marcadorNovoCartao > 0) {
    marcadorNovoCartao ++;
    alterarCartoes();

    if (marcadorNovoCartao > 14) {
      marcadorNovoCartao = 0;
    }
  }
  else {

    // Aguarda a aproximacao do cartao
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      return;
    }
    // Seleciona um dos cartoes
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      return;
    }
    // Mostra UID na serial
    Serial.print("UID da tag :");
    String conteudo = "";
    byte letra;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.println();
    Serial.print("Mensagem : ");
    conteudo.toUpperCase();

    //Testa se o cartão é o MASTER
    if (conteudo.substring(1) == master) {
      marcadorNovoCartao = 1;
      alterarCartoes();
      Serial.println("Master reconhecido");
      Serial.println();
      // Testa se o cartao foi lido
    }
    else if (isCartaoCadastrado(conteudo.substring(1))) {
      liberarAcesso();
      Serial.println("cartão lido");
      Serial.println();
    }
    else {
      Serial.println("Cartao2 - Acesso negado !!!");
      Serial.println();
    }

  }
}

bool isCartaoCadastrado(String cartao) {
  if (cartoesCadastrados.indexOf(cartao) >= 0) {
    Serial.print("Cartão Cadastrado!");
    return true;
    delay(500);
  }
  return false;
}
