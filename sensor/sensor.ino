#include <WiFi.h>

const char* idrede = "nome_rede";
const char* password = "senha_rede";

void setup() {
  Serial.begin(115200); //velocidade máxima serial.begin
  WiFi.begin(idrede, password);
 
  while ( WiFi.status() != WL_CONNECTED){
    delay (1000);
    Serial.println("A estabelecer ligação...");
  }
  Serial.println("Ligado");
}

void loop() {
  // put your main code here, to run repeatedly:

}
