#include <WiFi.h>

//Configuracoes:
#define CFG_WIFI_NOME_REDE                "MEO-C17F40-EXT"
#define CFG_WIFI_PASS_REDE                "????"

#define CFG_DEBUG                         true
#define CFG_DEBUG_BAUDRATE                115200
#define CFG_BUFFER_SIZE                   1024

#define CFG_SERVICE_ADDRESS               "http://localhost:8000/amostras/"
#define CFG_DATABASE_ID_RELOGIO           1

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
  char* nomeSensor;
  float valor;
  char* unidade;
  bool lido;
}amostra;
amostra sensores[] = {
  {
    "Frequencia Cardiaca",
    0.0,
    "bps"
    false
  },
  {
    "Oxigenio no Sangue",
    0.0,
    "%"
    false
  },
  {
    "Pressao arterial min",
    0.0,
    "bps",
    false
  },
  {
    "Pressao arterial max",
    0.0,
    "bps",
    false
  },
  {
    "Detetor Quedas",
    0.0,
    "N",
    false
  }
}

char* amostra_to_json(amostra* m,char* buff){
  sprintf(buff,"
  {
    \"sensor\": %d,
    \"unidade\": \"%s\",
    \"valor\": %f
  }
  ", CFG_DATABASE_ID_RELOGIO, m->unidade, m->valor);
  return buff;
}

void amostra_post(char* httpRequestData){
  HTTPClient http;
  char* serverPath = CFG_SERVICE_ADDRESS;
  http.begin(serverPath);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(httpRequestData);
  if(httpResponseCode != 200){
    debug_print("Erro de ligacao");
  }
  http.end();
}

void amostra_main(){
  int i;
  char buffer[CFG_BUFFER_SIZE];

  if(wifi_ligado == false)
    return;

  for(i=0;i< sizeof(sensores) / sizeof(sensores[0]); i++){
      if(sensores[i].lido == true){
        amostra_post(amostra_to_json(sensores[i],buffer));
        sensores[i].lido = false;
      }
  }

}

typedef struct tarefa_t{
  long int t_inicio_ms;
  long int t_perido_ms
  void(*f)(void) t;
}tarefa;

void sensor_freq_cardiaca_main(){
  //leu-se 50
  sensores[0].valor = sensores[0].valor + 1;
  sensores[0].lido = true;

}


tarefa tarefas[] = {
// t        periodo Funcao
  { 0,      500,    wifi_main                   },
  { 0,      500,    amostra_main                },
  { 0,      1000,   sensor_freq_cardiaca_main   }
}



void tarefas_main(){
  int i;
  for(i=0;i<sizeof(tarefas) / sizeof(tarefas[0]), i++){
      if(millis() - tarefas[i].t_inicio_ms >= tarefas[i].t_periodo_ms){
        tarefas[i].t();
        tarefas[i].t_inicio_ms = millis();
      }
  }
}
void setup() {
  wifi_init();
}

void loop() {
  tarefas_main();
}
