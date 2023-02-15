#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include <stdbool.h>
struct amostra{
  char* nomeSensor;
  float valor;
  char* unidade;
  bool lido;
};
typedef void(*tarefa_f)(void);
struct tarefa{
  long int t_inicio_ms;
  long int t_periodo_ms;
  tarefa_f t;
};


#endif
