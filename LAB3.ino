
//defineing the pins for LED, button and signal port
#define task4LED 4
#define signalT1 3
#define task4Pot 8
#define buttonLED 9
#define button 12


unsigned long time1;
unsigned long time2;
//task 2 perameters
const int task2SW = 7;
int freq2Val;
float task2Tot;
float task2Freq;
//task 3 perameters
const int task3SW = 2;
int freq3Val;
float task3Tot;
float task3Freq;

//task4 perameters
unsigned int analogVal[4];
int sumVoltage = 0;
int counter = 0;
float voltage = 0;
float avarageVoltage = 0;

//button and debouncing 
int buttonBounce = 0;
int buttonLED = 0;

//task 2 and 3 struct for task 5
struct task1_2{
  unsigned int task2Freq;
  unsigned int task3Freq;
}freqs;

//semaphores for task 2, 3 and 5
static SemaphoreHandle_t sema;
static SemaphoreHandle_t sema2;
static SemaphoreHandle_t sema3;



void setup(){
xTaskCreatePinnedToCore(
            task1,
            "task1",
            4096,
            NULL,
            1,
            NULL,
            0);

xTaskCreatePinnedToCore(
            task2,
            "task2",
            4096,
            NULL,
            1,
            NULL,
            0);

xTaskCreatePinnedToCore(
            task3,
            "task3",
            4096,
            NULL,
            1,
            NULL,
            0);

xTaskCreatePinnedToCore(
            task4,
            "task4",
            4096,
            NULL,
            1,
            NULL,
            0);

xTaskCreatePinnedToCore(
            task5,
            "task5",
            4096,
            NULL,
            1,
            NULL,
            0);

xTaskCreatePinnedToCore(
            ButtonRead,
            "Button Read",
            4096,
            NULL,
            1,
            NULL,
            0);            

xTaskCreatePinnedToCore(
            ButtonWrite,
            "Button Write",
            4096,
            NULL,
            1,
            NULL,
            0);   
  Serial.begin(9600);                
  delayMicroseconds(2000);        
  pinMode(signalT1,OUTPUT);
  pinMode(task2SW, INPUT);  
  pinMode(task3SW, INPUT);
  pinMode (task4Pot, INPUT);
  pinMode (task4LED, OUTPUT);
  pinMode (buttonLED, OUTPUT);
  pinMode (button, INPUT);
  pinMode (LED_BUILTIN, OUTPUT);
  sema = xSemaphoreCreateBinary();
  sema2 = xSemaphoreCreateBinary();
  sema3 = xSemaphoreCreateBinary();

}


void loop(void){}


void task1(void*parameter){
while(1){

  TickType_t TaskBegin = xTaskGetTickCount();

    digitalWrite(signalT1, HIGH);
     delayMicroseconds(200);
    digitalWrite(signalT1, LOW);
     delayMicroseconds(50);
    digitalWrite(signalT1, HIGH);
     delayMicroseconds(30);
    digitalWrite(signalT1, LOW);

  vTaskDelayUntil(&TaskBegin, 4/portTICK_PERIOD_MS);
}

}

void task2(void*parameter){
 
while(1){
 TickType_t TaskBegin = xTaskGetTickCount();
 
  freq2Val = pulseIn(task2SW,HIGH,1000);
  task2Tot = (freq2Val*2);
  freqs. task2Freq = 1000000/task2Tot;
  xSemaphoreGive(sema);

  vTaskDelayUntil(&TaskBegin, 20/portTICK_PERIOD_MS);

}
}

void task3(void*parameter){

 while(1){
  TickType_t TaskBegin = xTaskGetTickCount();
  
  freq3Val = pulseIn(task3SW,HIGH,1000);
  task3Tot = (freq3Val*2);
  freqs. task3Freq = 1000000/task3Tot;
  xSemaphoreGive(sema2);

  vTaskDelayUntil(&TaskBegin, 8/portTICK_PERIOD_MS);
 }
}


void task4(void*parameter){

while(1){

  TickType_t TaskBegin = xTaskGetTickCount();
  
  if(counter <= 3){
  
    counter = 0;  
}

 analogVal[counter] = analogRead(task4Pot);
 
 

  avarageVoltage = (analogVal[0]+analogVal[1]+analogVal[2]+analogVal[3])/4; 
  counter++;
  
  voltage = avarageVoltage*(3.3/4095.0);

  if (voltage >= 1.65){
    
   digitalWrite (task4LED, HIGH);
    
  }

  else {
    
  digitalWrite (task4LED, LOW);    

  }
  vTaskDelayUntil(&TaskBegin, 20/portTICK_PERIOD_MS);
}

  
}



void task5(void*parameter){
while(1){
  
TickType_t TaskBegin = xTaskGetTickCount();
  
xSemaphoreTake(sema,0);
xSemaphoreTake(sema2,0);
  int Frequancy2task5 = map(task2Freq, 333, 1000, 0, 99);
  int Frequancy3task5 = map(task3Freq, 500, 1000, 0, 99);

  Frequancy2task5 = constrain(Frequancy2task5, 0, 99);
  Frequancy3task5 = constrain(Frequancy3task5, 0, 99);

Serial.print(Frequancy2task5);
Serial.print(Frequancy3task5);

  vTaskDelayUntil(&TaskBegin, 100/portTICK_PERIOD_MS);
  }
}

void ButtonRead (void*parameter){
  while (1){
TickType_t TaskBegin = xTaskGetTickCount();

  ButtonPress = digitalRead (button);

  if (ButtonPress != buttonBounce){
    if (ButtonPress == 0 );
      buttonLED++
  }
    

  xSemaphoreGive(sema3);

  vTaskDelayUntil(&TaskBegin, 8/portTICK_PERIOD_MS);
  }
  
}

void ButtonWrite(void*parameter){
while (1){

TickType_t TaskBegin = xTaskGetTickCount();
  
xSemaphoreTake (sema3,0);

  if (buttonLED > 0){
    digitalWrite (LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite (LED_BUILTIN,LOW);
  }

  buttonLED = 0;
  
  vTaskDelayUntil(&TaskBegin, 8/portTICK_PERIOD_MS);
}  
}
