// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Seesaw.h>

struct Plant {
    int motorPin;
    Adafruit_Seesaw sensor;
    int sensorAddress;
    String plantName;
    int threshold;
};

const int numPlants = 2;

Plant plants[numPlants];


void setup() {
    
    plants[0].motorPin = D2;
    plants[1].motorPin = D3;
  
    plants[0].sensorAddress = 0x36;
    plants[1].sensorAddress = 0x37;
  
    plants[0].threshold = 200;
    plants[1].threshold = 200;
  
    Serial.begin(115200);
    
    while (!Serial)
    {
    delay(10);
    }
    
    for (int i = 0; i < numPlants; i++) {
        if (!plants[i].sensor.begin(plants[i].sensorAddress))
        {
            Serial.print("ERROR! sensor ");
            Serial.print(i);
            Serial.println(" not found");
            while (1)
                ; 
        } else {
            Serial.print("seesaw ");
            Serial.print(i);
            Serial.println("started! version: ");
            Serial.println(plants[i].sensor.getVersion(), HEX);
        }
    }
    
    for (int i = 0; i < numPlants; i++) {
        pinMode(plants[i].motorPin, OUTPUT);
        digitalWrite(plants[i].motorPin, LOW);
    }
    
    Particle.function("ManualRun", runMotor);
    
    // Particle.function("SetThreshold", changeThreshold);
    
    Particle.function("Calibrate", setDrySoil);

    
}

void loop() {
    for (int i = 0; i < numPlants; i++) {
        Plant thisplant = plants[i];
        int16_t capread = plants[i].sensor.touchRead(0);
        Serial.print("sensor ");
        Serial.print(i);
        Serial.print(" has capacity: ");
        capread = plants[i].sensor.touchRead(0);
        Serial.println(capread);
        if (capread < thisplant.threshold) {
            // Run water for 500ms
            digitalWrite(thisplant.motorPin, HIGH);
            delay(500);
            // Turn off the water and wait 10 seconds for it to soak in
            digitalWrite(thisplant.motorPin, LOW);
            delay(10000);
        } else {
            digitalWrite(thisplant.motorPin, LOW);
        }
    }
    delay(1000);
}


// Allows the user to manually run the motor
// TODO: consider adding a time that the user wants it to run for
int runMotor(String plantStr) {
    int plantNum = plantStr.toInt();
    digitalWrite(plants[plantNum].motorPin, HIGH);
    delay(1500);
    digitalWrite(plants[plantNum].motorPin, LOW);
    return 0;
}

/* int changeThreshold(String input) {
    char plantStr = input[0];
    int plantNum = plantStr - '0';
    
    Serial.println(input);
    
    bool low = false;
    low = input.length() > input.replace(String("low"), String("")).length();
    bool med = false;
    med = input.length() > input.replace(String("med"), String("")).length();
    bool high = false; 
    high = input.length() > input.replace(String("high"), String("")).length();

    if (low) {
        plants[plantNum].threshold = LOW;
        Serial.println("Setting to Low");Serial.print("New threshold for plant ");
        Serial.print(plantNum);
        Serial.print(" is: ");
        Serial.println(plants[plantNum].threshold);
    }
    if (med) {
        plants[plantNum].threshold = MED;
        Serial.println("Setting to Medium");
        Serial.print("New threshold for plant ");
        Serial.print(plantNum);
        Serial.print(" is: ");
        Serial.println(plants[plantNum].threshold);
    }
    if (high) {
        plants[plantNum].threshold = HIGH;
        Serial.print("New threshold for plant ");
        Serial.print(plantNum);
        Serial.print(" is: ");
        Serial.println(plants[plantNum].threshold);
    }
    
    return 0;
}
*/

int setDrySoil(String input) {
    char plantStr = input[0];
    int plantNum = plantStr - '0';
    
    bool dry = input.length() > input.replace(String("dry"), String("")).length();
    bool med = input.length() > input.replace(String("med"), String("")).length();
    bool wet = input.length() > input.replace(String("wet"), String("")).length();
    
    Serial.println(plants[plantNum].sensor.touchRead(0));
    
    int16_t capacitance = plants[plantNum].sensor.touchRead(0);
    Serial.println(capacitance);
    
    if (dry) {
        plants[plantNum].threshold = capacitance + 50;    
    }
    
    if (med) {
        plants[plantNum].threshold = capacitance - 25;    
    }
    
    if (wet) {
        plants[plantNum].threshold = capacitance - 75;
    }
    
    
    return plants[plantNum].threshold;
}

/*TODOLIST
- finalizing the setup
- testing the thresholds for soil
- maybe think about 3d printing?? 
*/
