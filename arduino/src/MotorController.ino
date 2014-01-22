/*
* MotorController
* 12/26/13
* Travis Carney
*
* This program controls a stepper motor via ethernet communication. Commands are sent to the arduino, processed, and then a reply is sent back.
*/

#include <Ethernet.h>
#include <SPI.h>
#include <EthernetUdp.h>

// Pin definitions
int mEnable = 2;		// Pin used to enable and disable the motor
int mStep = 3;			// Pin used to control motor step pulses
int mDir = 4;			// Pin used to control motor direction

// Other variables
unsigned int listenPort = 8888;			// Port to listen on
byte mac[] = {							// Mac address of the ethernet shield
  0x90, 0xA2, 0xDA, 0x00, 0xDF, 0x64 };
IPAddress ip(192, 168, 2, 140);			// IP address of the arduino
char packetBuff[UDP_TX_PACKET_MAX_SIZE];// Contains the incoming packet data
char replyBuff[255];					// Contains the reply message
EthernetUDP udp;						// EthernetUDP instance for sending and receiving data
bool isMoving;							// Flag used to determine if the motor is active

/*
* Setup serial and ethernet communication, pin modes, set default states, and display prompt.
*/
void setup() {
	// Setup ethernet communication
	Ethernet.begin(mac, ip);
	udp.begin(listenPort);

	// Setup serial communication
	Serial.begin(9600);
	
	// Setup pins
	pinMode(mEnable, OUTPUT);
	pinMode(mStep, OUTPUT);
	pinMode(mDir, OUTPUT);

	// Set default output states
	digitalWrite(mEnable, HIGH);	// Disable the motor to prevent over-heating

	// Default flags
	isMoving = false;

	// Display instructions
	prompt();
}

/*
* Waits for incoming commands
*/
void loop() {
	// Wait for incoming data
	int packetSize = udp.parsePacket();

	if (packetSize)
		parseCommand();
}

/*
* Processes the incoming packet and calls the appropriate function.
*/
void parseCommand() {
	udp.read(packetBuff, UDP_TX_PACKET_MAX_SIZE);
	IPAddress ip = udp.remoteIP();
	int port = udp.remotePort();

	// Enable the motor (EN == LOW)
	if (strcmp(packetBuff, "on") == 0) { 
		sendMessage("Enabling the motor...", ip, port);
		enableMotor(LOW);
	}
	// Disable the motor (EN == HIGH)
	else if (strcmp(packetBuff, "off") == 0) {  
		sendMessage("Disabling the motor...", ip, port);
		enableMotor(HIGH);
	}
	// Run the motor
	else if (strcmp(packetBuff, "cw") == 0) {
		sendMessage("Running the motor clock-wise...", ip, port);
		runMotor(5000, HIGH);
	}
	else if (strcmp(packetBuff, "ccw") == 0) {
		sendMessage("Running the motor counter clock-wise...", ip, port);
		runMotor(5000, LOW);
	}
	// Report status
	else if (strcmp(packetBuff, "status") == 0) {
		getStatus();
	}
	// Unrecognized command
	else {
		sendMessage("Unrecognized command", ip, port);
	}

	// Reset the packet buffer and prompt the user again
	memset(packetBuff, 0, UDP_TX_PACKET_MAX_SIZE);
	prompt();
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
		isMoving = true;
		startTime = millis();
		digitalWrite(mDir, direction); // Set direction

		while ((millis() - startTime) < duration) {
			digitalWrite(mStep, HIGH);
			delayMicroseconds(500);
			digitalWrite(mStep, LOW);
			delayMicroseconds(500);
		}

		isMoving = false;
	}
	else
		sendMessage("Motor is disabled, cannot run.", udp.remoteIP(), udp.remotePort());
}

/*
* Enables and disables the motor.
* If enb is HIGH, the motor is disabled.
* If enb is LOW, the motor is enabled.
*/
void enableMotor(int enb) {
	digitalWrite(mEnable, enb);
}

/*
* This function sends out a string on the given IP and port.
*/
void sendMessage(const char * msg, IPAddress ip, uint16_t port) { 
	// Put the message into the reply buffer
	strncpy(replyBuff, msg, 255);

	// Send out the message
	udp.beginPacket(ip, port);
	udp.write(replyBuff);
	udp.endPacket();

	// Clear out the reply buffer
	memset(replyBuff, 0, 255);
}

/*
* Sends out the current status of the motor to the IP and port of the request.
*/
void getStatus() {
	String status = "";
	
	// Enabled or disabled
	if (digitalRead(mEnable) == 0)
		status += "Enabled; ";
	else
		status += "Disabled; ";

	// Running or not running
	if (digitalRead(mStep) == 0)
		status += "Not running;";
	else 
		status += "Running";

	// Send out the current status
	sendMessage(status.c_str(), udp.remoteIP(), udp.remotePort());
}