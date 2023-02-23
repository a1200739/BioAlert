#ifndef RELOGIO_H
#define RELOGIO_H
#include "estruturas.h"

void relogio_init(amostra* x);
void relogio_main();

void onBeatDetected();
void onBeatDetected1();
void getbpm_spO2();
void display_data(int bpm, int spo2);
void drawLine(int *x_pos);
void refresh();
void rtcModule();
void heart_beat(int *x_pos);
void relogio_main();
#endif
