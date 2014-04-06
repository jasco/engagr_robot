#define sensor1 A0
#define sensor2 A1
#define MOTOR_LEFT  4
#define MOTOR_RIGHT 5

// Serial commands, one character each at this stage
// Except for F,B,S which were chosen for legacy reasons,
// commands were selected for geometric positioning on keyboard
#define CMD_STOP      'S'
#define CMD_FORWARD   'F'
#define CMD_FWD_LEFT  'Q'
#define CMD_FWD_RIGHT 'E'
#define CMD_REVERSE   'B'
#define CMD_REV_LEFT  'Z'
#define CMD_REV_RIGHT 'C'
#define CMD_MANUAL    'G'
#define CMD_AUTO      'H'

// Velocity, scale 0-255
#define FULL_FWD      150
#define HALF_FWD      75

int sensitivity = 18;
int velocity = 150;
int lineFollowingMode = 1;

void setup()
{
    Serial.begin(1152000);

    pinMode(MOTOR_LEFT,OUTPUT);
    pinMode(MOTOR_RIGHT,OUTPUT);

    stop();
}

void loop() {
    // Process serial input if any
    char input = 0;
    if (Serial.available() > 0) {
        input = Serial.read();
        processSerialModeCommand(input);
    }

    if (lineFollowingMode) {
        followTheLine();
    } else {
        processSerialDriveCommand(input);
    }

}

void processSerialDriveCommand(const char input) {
    switch (input) {
        case CMD_STOP:
            stop();
            break;

        case CMD_FORWARD:
            drive(FULL_FWD, FULL_FWD);
            break;

        case CMD_FWD_LEFT:
            drive(FULL_FWD, HALF_FWD);
            break;

        case CMD_FWD_RIGHT:
            drive(HALF_FWD, FULL_FWD);
            break;

        case CMD_REVERSE:
            break;
        case CMD_REV_LEFT:
            break;
        case CMD_REV_RIGHT:
            break;

        case CMD_CRAZY:
            drive(HALF_FWD, 0);
            break;
    }
}

void processSerialModeCommand(char input) {
    switch (input) {
         case CMD_MANUAL:
            stop();
            lineFollowingMode = 1;
            break;

        case CMD_AUTO:
            stop();
            lineFollowingMode = 0;
            break;
   }
}

void drive(int leftVelocity, int rightVelocity) {
    analogWrite(MOTOR_LEFT, leftVelocity);
    analogWrite(MOTOR_RIGHT, rightVelocity);
    delay(20);
}

void stop() {
    analogWrite(MOTOR_LEFT, 0);
    analogWrite(MOTOR_RIGHT, 0);
    delay(100);
}

void followTheLine() {
    int leftLightSensor = analogRead(sensor1);
    int rightLightSensor = analogRead(sensor2);

    int lightDiff = abs(leftLightSensor - rightLightSensor);
    int isVeeringRight = rightLightSensor > leftLightSensor; // following the dark-side
  
    Serial.print(leftLightSensor);
    Serial.print("\t");
    Serial.print(rightLightSensor);
    Serial.print("\t");
    Serial.print(lightDiff);
    Serial.println();
  
    // check if off course
    if (lightDiff >= sensitivity) {
        if (isVeeringRight) {
            //digitalWrite(motor1,LOW);
            drive(0, FULL_FWD);
        } else {
            //digitalWrite(motor2,LOW);
            drive(FULL_FWD, 0);
        }
    } else {
        drive(FULL_FWD, FULL_FWD);
    }
}
