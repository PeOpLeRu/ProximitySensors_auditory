class Ultra_sensor_distance
{
  uint8_t pin_trig;
  uint8_t pin_echo;
  
public:
  Ultra_sensor_distance(uint8_t _pin_trig = 12, uint8_t _pin_echo = 11)
  {
    this->pin_trig = _pin_trig;
    this->pin_echo = _pin_echo;
    
    pinMode(_pin_trig, OUTPUT);
    pinMode(_pin_echo, INPUT);
  }

  float get_distance()
  {
    long duration;
    float distance_cm;
    
    // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.

    digitalWrite(this->pin_trig, LOW);
    delayMicroseconds(5);
    digitalWrite(this->pin_trig, HIGH);

    // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
    delayMicroseconds(10);
    digitalWrite(this->pin_trig, LOW);

    //  Время задержки акустического сигнала на эхолокаторе.
    duration = pulseIn(this->pin_echo, HIGH);

    // Теперь осталось преобразовать время в расстояние
    distance_cm = (duration / 2) / 29.1;
    
    return distance_cm;
  }
};

class IK_sensor_distance
{
public:
  IK_sensor_distance()
  {
    pinMode(A0, INPUT);
  }

  float get_distance(float K = 12.298)
  {
    float distance_cm = 0.0;

    int16_t analog_value  = analogRead(A0);
    float V = analog_value * 0.0048828125;
    
    distance_cm = K * pow(V, -1.10);

    return distance_cm;
  }
};

Ultra_sensor_distance u_sensor;
IK_sensor_distance ik_sensor;

float K = 0.0;
  
void setup()
{
  Serial.begin (9600);  

  Serial.println("Calibrate start!");

  float delta = 1.0;
  float _step = 0.1;

//  Serial.println(_abs(u_sensor.get_distance() - ik_sensor.get_distance(K)));
//  while (_abs(u_sensor.get_distance() - ik_sensor.get_distance(K)) > delta)
  {
    if ((u_sensor.get_distance() - ik_sensor.get_distance(K)) > 0)
    {
      K += _step;
    }
    else 
    {
      K -= _step;
    }
    Serial.println(u_sensor.get_distance());
    Serial.println(ik_sensor.get_distance(K));
    Serial.print("K = ");
    Serial.println(K);
    delay(900);
  }

  Serial.println("Calibrate is finished!");
}

void loop()
{
  float cm_1 = u_sensor.get_distance();
  Serial.print("Расстояние до объекта: ");
  Serial.print(cm_1);
  Serial.println(" см.");
  
  float cm_2 = ik_sensor.get_distance(K);
  Serial.print("Расстояние до объекта: ");
  Serial.print(cm_2);
  Serial.println(" см.");
  
  // Задержка между измерениями для корректной работы скеча
  delay(750);
}

float _abs(const float num)
{
  if (num >= 0)
    return num;
  else
    return -num;
}
