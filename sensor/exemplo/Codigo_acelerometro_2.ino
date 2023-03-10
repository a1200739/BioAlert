#include <Wire.h>

//Endereço em hexadecimal do sensor MPU 6050
const int ENDERECO_SENSOR = 0x68;

int girX, girY, girZ, acelX, acelY, acelZ, temperatura;


void setup() 
{
  Serial.begin(115200);

  //Inicializa a biblioteca Wire
  Wire.begin();
  Wire.beginTransmission(ENDERECO_SENSOR);
  Wire.write(0x6B);

  //Inicializa o sensor
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() 
{
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

  //temperatura = Wire.read() << 8| Wire.read(); //0×41 (TEMP_OUT_L)

  girX = Wire.read() << 8| Wire.read();    //0×43 (GYRO_XOUT_H) & 0×44 (GYRO_XOUT_L)
  girY = Wire.read() << 8| Wire.read();    //0×45 (GYRO_YOUT_H) & 0×46 (GYRO_YOUT_L)
  girZ = Wire.read() << 8| Wire.read();    //0×47 (GYRO_ZOUT_H) & 0×48 (GYRO_ZOUT_L)

  //Printa o valor X do acelerómetro na serial
  Serial.print("Acelerómetro X = ");
  Serial.print(acelX);

  //Printa o valor Y do acelerómetro na serial
  Serial.print("\tY = ");
  Serial.print(acelY);

  //Printa o valor Z do acelerómetro na serial
  Serial.print("\tZ = ");
  Serial.println(acelZ);

  //Printa o valor X do giroscópio na serial
  Serial.print("Giroscópio X =");
  Serial.print(girX);

  //Printa o valor Y do giroscópio na serial
  Serial.print("\tY = ");
  Serial.print(girY);

  //Printa o valor Z do giroscópio na serial
  Serial.print("\tZ = ");
  Serial.println(girZ);

  //Printa o valor da tempeartura na serial, calculando em graus celsius
  //Serial.print("Temperatura = ");
  //Serial.println(temperatura / 340.00 + 36.53);

  delay(200);
}

