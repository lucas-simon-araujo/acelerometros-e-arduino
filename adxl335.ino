// Definindo a frequência de amostragem em hertz
const int fsample = 60;
// Definindo o período de amostragem em milissegundos
float tsample = (1/fsample)*1000;

// Definindo os pinos de entrada do sinal
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;
/* Inicializando máximo e mínimo valores para a 
leitura analógica*/
/* Entre 0 e 1023 devido ao conversor analógico-digital 
do arduino ser de 10 bits (2^10=1024 pontos)*/
int RawMin = 0;
int RawMax = 1023;

/* Definindo a quantidade de amostras para cada medição
para reduzir o ruído*/
const int sampleSize = 10;

void setup() 
{
  /* Definindo a referência do Arduino como sendo 
  externa (3,3 V no AREF)*/
  /* Pois o adxl 335 trabalha nessa voltagem 
  (regulador de tensão)*/
  analogReference(EXTERNAL);
  /* Inicializando a comunicação serial a uma taxa de 
  9600 bits/s*/
  Serial.begin(9600);
   
}

void loop() 
{
  /* Lendo medições analógicas através da função 
  subscrita ReadAxis*/
  int xRaw = ReadAxis(xInput);
  int yRaw = ReadAxis(yInput);
  int zRaw = ReadAxis(zInput);

  /* Convertendo valores análogicos obtidos em valores
  de miliG*/ 
  // Interpolação linear através da função map
  /* Valores máximos e mínimos analógicos com seus 
  correspondentes em miliG (3G*1000)*/ 
  long xScaled = map(xRaw, RawMin, RawMax, -3000, 3000);
  long yScaled = map(yRaw, RawMin, RawMax, -3000, 3000);
  long zScaled = map(zRaw, RawMin, RawMax, -3000, 3000);

  // Convertendo miliG para G
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;

  /* Exibindo resultados no monitor serial com 
  4 casas decimais*/
  Serial.print(xAccel,4);
  Serial.print("/t");
  Serial.print(yAccel,4);
  Serial.print("/t");
  Serial.println(zAccel,4);

  // Aguardar para a próxima medição
  delay(tsample);
}

// Toma as amostras e ratorna uma medição média
int ReadAxis(int axisPin)
{
  // Define a variável leitura
  long leitura = 0;
  delayMicroseconds(1);
  /* Realiza a soma das leituras enquanto o contador
  é menor que a quantidade de amostras*/
  for (int i = 0; i < sampleSize; i++)
  {
  leitura = leitura + analogRead(axisPin);
  }
  /* Realiza a média dividindo o somatório 
  pela quantidade de amostras*/
  return leitura/sampleSize;
}
