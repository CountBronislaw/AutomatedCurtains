/*
* MotorController
* 12/26/13
* Travis Carney
*
* This program controls a stepper motor via ethernet communication. Commands are sent to the arduino, processed, and then a reply is sent back.
*
*/

#include <Ethernet.h>
#include <SPI.h>
#include <EthernetUdp.h>

// Pin definitions
int mEnable = 2;		// Pin used to enable and disable the motor
int mStep = 3;			// Pin used to control motor step pulses
int mDir = 4;			// Pin used to control motor direction

// Other variables
unsigned int port = 8888;				// Port to listen on
byte mac[] = {							// Mac address of the ethernet shield
  0x90, 0xA2, 0xDA, 0x00, 0xDF, 0x64 };
IPAddress ip(192, 168, 2, 140);			// IP address of the arduino
char packetBuff[UDP_TX_PACKET_MAX_SIZE];// Contains the incoming packet data
char replyBuff[255];					// Contains the reply message
EthernetUDP udp;						// EthernetUDP instance for sending and receiving data

/*
* Setup serial and ethernet communication, pin modes, set default states, and display prompt.
*/
void setup() {
	// Setup ethernet communication
	Ethernet.begin(mac, ip);
	udp.begin(port);

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
void loop() {
	// Wait for incoming data
	int packetSize = udp.parsePacket();

	if (packetSize) {
		udp.read(packetBuff, UDP_TX_PACKET_MAX_SIZE);

		// Enable the motor (EN == LOW)
		if (strcmp(packetBuff, "on") == 0) { 
			strncpy(replyBuff, "Enabling the motor...", 255);
			enableMotor(LOW);
		}
		// Disable the motor (EN == HIGH)
		else if (strcmp(packetBuff, "off") == 0) {  
			strncpy(replyBuff, "Disabling the motor...", 255);
			enableMotor(HIGH);
		}
		// Run the motor
		else if (strcmp(packetBuff, "cw") == 0) {
			strncpy(replyBuff, "Running the motor clock-wise...", 255);
			runMotor(5000, HIGH);
		}
		else if (strcmp(packetBuff, "ccw") == 0) {
			strncpy(replyBuff, "Running the motor counter clock-wise...", 255);
			runMotor(5000, LOW);
		}
		// Unrecognized command
		else {
			strncpy(replyBuff, "Unrecognized command", 255);
		}
    
		// Send response back to the IP who sent a command
		udp.beginPacket(udp.remoteIP(), udp.remotePort());
		udp.write(replyBuff);
		udp.endPacket();

		// Reset the packet buffer and prompt the user again
		memset(packetBuff, 0, UDP_TX_PACKET_MAX_SIZE);
		prompt();
	}
}

/*
* Displays a message to the user.
*/
void prompt() {
	Serial.println("Please enter a motor command:");
}

/*
* Runs the motor for a given duration (in ms) in either a clockwise or counter-clockwise direction.
* Pass in HIGH for direction to go clockwise and LOW for direction to go counter-clockwise.
*/
void runMotor(long duration, int direction) {
	long startTime = 0;  //	When the motor started running

	// Check to see if the motor is disabled
	if (digitalRead(mEnable) == 0){
		startTime = millis();
		digitalWrite(mDir, direction); // Set direction
		while ((millis() - startTime) < duration) {
			digitalWrite(mStep, HIGH);
			delayMicroseconds(500);
			digitalWrite(mStep, LOW);
			delayMicroseconds(500);
		}
	}
	else
		Serial.println("Motor is disabled, cannot run.");
}

/*
* Enables and disables the motor.
* If enb is HIGH, the motor is disabled.
* If enb is LOW, the motor is enabled.
*/
void enableMotor(int enb) {
	digitalWrite(mEnable, enb);
}