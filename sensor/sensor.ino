#include <WiFi.h>
#include <HttpClient.h>

#include "estruturas.h"
#include "acelerometro.h"
#include "relogio.h"

//Configuracoes:
#define CFG_WIFI_NOME_REDE                "MEO-C17F40-EXT"
#define CFG_WIFI_PASS_REDE                "????"

#define CFG_DEBUG                         true
#define CFG_DEBUG_BAUDRATE                115200
#define CFG_BUFFER_SIZE                   1024

#define CFG_SERVICE_ADDRESS               "127.0.0.1"
#define CFG_SERVICE_ADDRESS_PORT          8000
#define CFG_DATABASE_ID_RELOGIO           1


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

amostra sensores[] = {
  /*Aceletremotro 0-6 | tamanho - 7 */
  {"girX" ,       0.0,  "º",      false },
  {"girY" ,       0.0,  "º",      false },
  {"girZ" ,       0.0,  "º",      false },
  {"acelX",       0.0,  "m/s",    false },
  {"acelY",       0.0,  "m/s",    false },
  {"acelZ",       0.0,  "m/s",    false },
  {"temperatura", 0.0,  "C",      false },

  /*Relogio 7-8 | tamanho - 2 */
  {"bpm",         0.0,  "bpm",    false },
  {"spo2",        0.0,  "spo2",   false },

  /* Simuladores 9-10 | tamanho 2  */
  {"sim1",        0.0,   "sim1",   false },
  {"sim2",        0.0,   "sim2",   false }
};

char* amostra_to_json(amostra* m,char* buff){
  sprintf(buff,"                      \
  {                                   \
    \"sensor\": %d,                   \
    \"unidade\": \"%s\",              \
    \"valor\": %.3f                   \
  }                                   \
  ", CFG_DATABASE_ID_RELOGIO, m->unidade, m->valor);
  return buff;
}

void amostra_post(char* httpRequestData){
  WiFiClient wifi;
  HttpClient http = HttpClient(wifi, CFG_SERVICE_ADDRESS, CFG_SERVICE_ADDRESS_PORT);
  char* serverPath = CFG_SERVICE_ADDRESS;
  char* header = "application/json";
  http.post("/amostras/",header,httpRequestData);
  int httpResponseCode = http.responseStatusCode();
  if(httpResponseCode != 200){
    debug_print("Erro de ligacao");
  }

}

void amostra_main(){
  int i;
  char buffer[CFG_BUFFER_SIZE];

  if(wifi_ligado == false)
    return;

  for(i=0;i< sizeof(sensores) / sizeof(sensores[0]); i++){
      if(sensores[i].lido == true){
        amostra_post(amostra_to_json(&sensores[i],buffer));
        sensores[i].lido = false;
      }
  }

}

amostra* simula_sensor;
void simula_sensor_init(amostra* a){
  simula_sensor = a;
}

void simula_sensor_main(){
  simula_sensor[0].lido = simula_sensor[0].lido+1;
  simula_sensor[0].lido = true;
  simula_sensor[1].lido = simula_sensor[1].lido+1;
  simula_sensor[1].lido = true;
}

tarefa tarefas[] = {
  // t     periodo    Funcao
  { 0,      500,      wifi_main                   },
  { 0,      500,      amostra_main                },
  { 0,      1000,     simula_sensor_main          },
//  { 0,      200,      acelerometro_main           },
//  { 0,      1000,     relogio_main                }
};

void tarefas_main() {
  int i;
  for(i=0;i<sizeof(tarefas) / sizeof(tarefas[0]); i++){
      if(millis() - tarefas[i].t_inicio_ms >= tarefas[i].t_periodo_ms){
        tarefas[i].t();
        tarefas[i].t_inicio_ms = millis();
      }
  }
}
void setup() {
  wifi_init();

  //acelerometro_init(&sensores[0]);
  //relogio_init(&sensores[7]);
  simula_sensor_init(&sensores[9]);
}

void loop() {
  tarefas_main();
}
