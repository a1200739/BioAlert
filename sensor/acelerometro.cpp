#include "acelerometro.h"
#include <Wire.h>
#include "estruturas.h"

//Endereço em hexadecimal do sensor MPU 6050
const int ENDERECO_SENSOR = 0x68;

int girX, girY, girZ, acelX, acelY, acelZ, temperatura;
amostra * acc_env;

void acelerometro_init(amostra* x){
    //Inicializa a biblioteca Wire
    Wire.begin();
    Wire.beginTransmission(ENDERECO_SENSOR);
    Wire.write(0x6B);

    //Inicializa o sensor
    Wire.write(0);
    Wire.endTransmission(true);

    acc_env = x;
}

void amostra_acc_enviar(){
    acc_env[0].valor = girX; acc_env[0].lido = true;
    acc_env[1].valor = girY; acc_env[1].lido = true;
    acc_env[2].valor = girZ; acc_env[2].lido = true;
    acc_env[3].valor = acelX; acc_env[3].lido = true;
    acc_env[4].valor = acelY; acc_env[4].lido = true;
    acc_env[5].valor = acelZ; acc_env[5].lido = true;
    acc_env[6].valor = temperatura; acc_env[6].lido = true;
}

void acelerometro_main(){
    //Começa uma transmissão com o sensor
    Wire.beginTransmission(ENDERECO_SENSOR);

    //Enfilera os bytes a ser transmitidos para o sensor
    //Começando com o registor 0×3B
    Wire.write(0x3B);    //starting with register 0×3B (ACCEL_XOUT_H)

    //Finaliza e transmite os dados para o sensor. O false fará com que o barramento seja libertado após a solicitação
    //(o valor padrão deste parâmetro é true)
    Wire.requestFrom(ENDERECO_SENSOR, 14, true);

    //Armazena o valor dos sensores nas variáveis correspondentes 
    acelX = Wire.read() << 8| Wire.read();   //0×3B (ACCEL_XOUT_H) & 0×3C (ACCEL_XOUT_L)
    acelY = Wire.read() << 8| Wire.read();   //0×3D (ACCEL_YOUT_H) & 0×3E (ACCEL_YOUT_L)
    acelZ = Wire.read() << 8| Wire.read();   //0×3F (ACCEL_ZOUT_H) & 0×40 (ACCEL_ZOUT_L)

    girX = Wire.read() << 8| Wire.read();    //0×43 (GYRO_XOUT_H) & 0×44 (GYRO_XOUT_L)
    girY = Wire.read() << 8| Wire.read();    //0×45 (GYRO_YOUT_H) & 0×46 (GYRO_YOUT_L)
    girZ = Wire.read() << 8| Wire.read();    //0×47 (GYRO_ZOUT_H) & 0×48 (GYRO_ZOUT_L)

    amostra_acc_enviar();
}
