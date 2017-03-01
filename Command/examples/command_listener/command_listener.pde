#include <Command.h>
/*
 Command Serial
 by Kenan Millet, Abhimanyu Ghosh
 NYU PolyBOTS, 2017

 Demonstrates how to create and use commands with the serial monitor 
 Adds on a "Command Listener" proof-of-concept that is essentially a callback
 that can be placed in any periodic task such as a timer interrupt service
 routine (ISR) or just in the main user app loop.
 */
 
//Creating the command
Command myCommand("test",
  //functionality of command goes here
  [](){
    Serial.println("Hello World!");
  }
);

void handler(const String& s)
{
  Serial.print("ERR: ");
  Serial.print("\"");
  Serial.print(s);
  Serial.println("\" is not a valid command!");
}

//LED 13 On command
Command on("on",
  //functionality of command goes here
  [](){
    digitalWrite(13, HIGH);
  }
);

//LED 13 Off command
Command off("off",
  //functionality of command goes here
  [](){
    digitalWrite(13, LOW);
  }
);

String s;

void CommandListener(void)
{
  /*
   * If data is available, pull it into the buffer
   * If we hit the newline delimiter, process the current buffer
   * via Command API, then empty the data buffer for next input:
   */
  if(Serial.available() > 0)
  {
    int ch = Serial.read();
     if(ch != (int)'\n') // Cast to int for comparison
     {
      s.concat((char)ch);
     }
     
     else
     {
        Command::exec(s); // Execute command
        s.remove(0); // Clear the data buffer
     }
  }
}

void setup()
{
  // Set up UART (Serial) monitor at 9600 baud, 8n1:
  Serial.begin(9600);

  // Register our simplistic error handler function with the Command library:
  Command::SetErrorHandler(&handler);

  // Perform some digital pin setup to use Pin 13 (Port B, Pin 5)
  // as an OUTPUT:
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // Initialize the output to set LED off by default
}

void loop()
{
  // This callback checks for pending bytes on the Serial port
  // It processes these bytes onto a buffer, and when a delimiting char
  // (in our case, a newline) is reached, it calls the Command API exec()
  // routine to run the command and the configured callback attached 
  // to that particular string:
  CommandListener();
}