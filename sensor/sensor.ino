#include <WiFi.h>

//Configuracoes:
#define CFG_WIFI_NOME_REDE                "MEO-C17F40-EXT"
#define CFG_WIFI_PASS_REDE                "????"

#define CFG_DEBUG                         true
#define CFG_DEBUG_BAUDRATE                115200

#define CFG_NUM_SENSORES                  5

#define CFG_SENSOR1_INDEX                 0
#define CFG_SENSOR1_NOME                  "Frequencia Cardiaca"
#define CFG_SENSOR1_UNIDADE               "bps"

#define CFG_SENSOR2_INDEX                 1
#define CFG_SENSOR2_NOME                  "Oxigenio no Sangue"
#define CFG_SENSOR2_UNIDADE               "%"

#define CFG_SENSOR3_INDEX                 2
#define CFG_SENSOR3_NOME                  "Pressao arterial min"
#define CFG_SENSOR3_UNIDADE               "bps"

#define CFG_SENSOR3_INDEX                 3
#define CFG_SENSOR3_NOME                  "Pressao arterial max"
#define CFG_SENSOR3_UNIDADE               "bps"

#define CFG_SENSOR3_INDEX                 4
#define CFG_SENSOR3_NOME                  "Detetor Quedas"
#define CFG_SENSOR3_UNIDADE               ""

#define CFG_TAXA_AMOSTRAGEM_SENSORES_MS   1000


/*
  Debug na porta serie
*/
void debug_init(){
  #if CFG_DEBUG == true
  Serial.begin(CFG_DEBUG_BAUDRATE);
  #endif
}
void debug_print(char* msg){
  #if CFG_DEBUG == true
  Serial.println(msg);
  #endif
}
/*
  Gestão ligacao wifi
*/
static bool wifi_ligado = false;
void wifi_init(){
  switch(WiFi.status()){
    case WL_NO_SSID_AVAIL:
    case WL_SCAN_COMPLETED:
    case WL_CONNECT_FAILED:
    case WL_CONNECTION_LOST:
    case WL_DISCONNECTED:
      WiFi.begin(CFG_WIFI_NOME_REDE, CFG_WIFI_PASS_REDE);
      break;
  }
}
void wifi_monitorizar(){
  switch(WiFi.status()){
    case WL_CONNECTED:
      wifi_ligado = true;
      break;
    default:
      wifi_ligado = false;
      break;
  }
}
void wifi_main(){
  
  switch(wifi_ligado){
    case false:
      wifi_init();
    break;
    case true:
      wifi_monitorizar();
    break;
  }
  
}
/*
  Estruturas de dados dos sensores
*/
typedef struct amostra_t{
  char nomeSensor[100];
  float valor;
  char unidade[100];
  bool lido;
}amostra;
amostra sensores[CFG_NUM_SENSORES];




void setup() {
  wifi_init();
}

void loop() {
  wifi_main();

}
