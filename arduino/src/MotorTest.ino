/*
* MotorTest
* 12/26/13
* Travis Carney
*
* This is a demo program for controlling a stepper motor. It uses serial communication to accept commands and send responses back.
*/

int mEnable = 11;		// Pin used to enable and disable the motor
int mStep = 12;			// Pin used to control motor step pulses
int mDir = 13;			// Pin used to control motor direction
String cmd = "";		// Contains the command
char cmdChar;			// Contains individual characters that make up a command

/*
* Setup serial communication, pin modes, set default states, and display prompt.
*/
void setup(){
	// Setup serial communication
	Serial.begin(9600);
	
	// Setup pins
	pinMode(mEnable, OUTPUT);
	pinMode(mStep, OUTPUT);
	pinMode(mDir, OUTPUT);

	// Set default output states
	digitalWrite(mEnable, HIGH);	// Disable the motor to prevent over-heating

	// Display instructions
	prompt();
}

/*
* Waits for incoming commands and runs the given command.
*/
void loop(){
	// Wait for incoming data
	if (Serial.available() > 0){
		while (Serial.available()){
			// Build the command
			cmdChar = Serial.read();
			cmd = cmd + cmdChar; 
			delay(1);	// Need this delay otherwise communication doesn't work properly
		}
    
		// Make the command lower-case
		cmd.toLowerCase();
    
		// Enable the motor (EN == LOW)
		if (cmd == "on"){
			Serial.println("Enabling the motor...");
			enableMotor(LOW);
		}
		// Disable the motor (EN == HIGH)
		else if (cmd == "off"){  
			Serial.println("Disabling the motor...");
			enableMotor(HIGH);
		}
		// Run the motor
		else if (cmd == "cw"){
			Serial.println("Running the motor clock-wise...");
			runMotor(5000, HIGH);
		}
		else if (cmd == "ccw"){
			Serial.println("Running the motor clock-wise...");
			runMotor(5000, LOW);
		}
		// Unrecognized command
		else {
			Serial.println("Unrecognized command '" + cmd + "'"); 
		}
    
		// Reset cmd and prompt the user again
		cmd = "";
		prompt();
	}
}

/*
* Displays a message to the user.
*/
void prompt(){
	Serial.println("Please enter a motor command:");
}

/*
* Runs the motor for a given duration (in ms) in either a clockwise or counter-clockwise direction.
* Pass in HIGH for direction to go clockwise and LOW for direction to go counter-clockwise.
*/
void runMotor(long duration, int direction){
	long startTime = 0;  //	When the motor started running

	// Check to see if the motor is disabled
	if (digitalRead(mEnable) == 0){
		startTime = millis();
		digitalWrite(mDir, direction); // Set direction
		do{
			digitalWrite(mStep, HIGH);
			delay(1);
			digitalWrite(mStep, LOW);
		} while ((millis() - startTime) < duration);
	}
	else
		Serial.println("Motor is disabled, cannot run.");
}

/*
* Enables and disables the motor.
* If enb is HIGH, the motor is disabled.
* If enb is LOW, the motor is enabled.
*/
void enableMotor(int enb){
	digitalWrite(mEnable, enb);
}