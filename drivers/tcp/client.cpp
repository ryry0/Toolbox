#include <iostream>
#include <string>
#include "tcpipnix.h"
#include "getch.h"
#include <cstdint>

using namespace std;
using namespace term;

#define PACKET_LENGTH 26
#define ROLL_OFFSET -1.05
#define PITCH_OFFSET -1.7

typedef union copter_setpoints_t {
  struct {
    float set_roll;
    float set_pitch;
    float set_yaw;
    float P;
    float I;
    float D;
    uint8_t hard_kill;
    uint8_t throttle;
  };
  uint8_t data[PACKET_LENGTH];
} copter_setpoints_t;

int main(int argc, char ** argv) {
  //This client server implements a simple "chat" program.
  //It sends one letter at a time.
  //To talk to a server you need to do the following:
  //Create the TCP Object
  TCP tcpConn;
  float roll, pitch, yaw;
  copter_setpoints_t copter_setpoints = {0};
  copter_setpoints.set_roll = ROLL_OFFSET;
  copter_setpoints.set_pitch = PITCH_OFFSET;

  cout << sizeof (copter_setpoints_t) << endl;
  //Specify a port to connect to
  int port = 5555;

  string addr = "127.0.0.1";


  //Specify an IP address such as the above
  if (argc > 1) {
    addr = string(argv[1]);
    cout << addr << endl;
  }
  else {
    cout << "ip address:";
    cin >> addr;
  }

  //run the connectToHost function, and pass in the port, and the address
  //the function returns true when you connect, and false if you don't
  cout << "connecting" << endl;
  if (!tcpConn.connectToHost(port, addr.c_str()))
    return 1;

  initTermios(0);

  cout << "connected" << endl;

  while (1)
  {

    char input = getch();
    switch(input) {
      case 'w':
        copter_setpoints.set_roll += 0.10f;
        break;
      case 's':
        copter_setpoints.set_roll -= 0.1f;
        break;
      case 'a':
        copter_setpoints.set_pitch += 0.1f;
        break;
      case 'd':
        copter_setpoints.set_pitch -= 0.1f;
        break;
      case 'q':
        copter_setpoints.set_yaw += 0.1f;
        break;
      case 'e':
        copter_setpoints.set_yaw -= 0.1f;
        break;

      case 'r':
        copter_setpoints.set_yaw = 0;
        copter_setpoints.set_roll = 0;
        copter_setpoints.set_pitch = 0;
        copter_setpoints.hard_kill = 0;
        copter_setpoints.throttle = 0;
        break;

      case 'U':
        copter_setpoints.throttle += 5;
        if (copter_setpoints.throttle > 100)
          copter_setpoints.throttle = 100;
        break;

      case 'J':
        if (copter_setpoints.throttle == 0)
          copter_setpoints.throttle = 0;
        else
          copter_setpoints.throttle -= 5;
        break;

      case 'u':
        copter_setpoints.throttle += 1;
        if (copter_setpoints.throttle > 100)
          copter_setpoints.throttle = 100;
        break;

      case 'j':
        if (copter_setpoints.throttle == 0)
          copter_setpoints.throttle = 0;
        else
          copter_setpoints.throttle -= 1;
        break;

      case ' ':
        copter_setpoints.set_yaw = 0;
        copter_setpoints.set_roll = 0;
        copter_setpoints.set_pitch = 0;
        copter_setpoints.throttle = 0;
        copter_setpoints.hard_kill = 1;
        break;

      case 'm':
        copter_setpoints.set_yaw = 0;
        copter_setpoints.set_roll = 0;
        copter_setpoints.set_pitch = 0;
        copter_setpoints.throttle = 0;
        break;

        //P defines
      case ')':
        copter_setpoints.P += 1.0f;
        break;

      case '0':
        copter_setpoints.P += 0.1f;
        break;

      case 'p':
        copter_setpoints.P += 0.01f;
        break;

      case ';':
        copter_setpoints.P -= 0.01f;
        break;

      case '/':
        copter_setpoints.P -= 0.1f;
        break;

      case '?':
        copter_setpoints.P -= 1.0f;
        break;

        //I defines
      case '*':
        //copter_setpoints.I += 10.0f;
        break;

      case '8':
        copter_setpoints.I += .01f;
        break;

      case 'i':
        copter_setpoints.I += 0.001f;
        break;

      case 'k':
        copter_setpoints.I -= 0.001f;
        break;

      case ',':
        copter_setpoints.I -= 0.01f;
        break;

      case '<':
        //copter_setpoints.I -= 10.0f;
        break;

        //D defines
      case '(':
        copter_setpoints.D += 0.1f;
        break;

      case '9':
        copter_setpoints.D += .01f;
        break;

      case 'o':
        copter_setpoints.D += 0.001f;
        break;

      case 'l':
        copter_setpoints.D -= 0.001f;
        break;

      case '.':
        copter_setpoints.D -= 0.01f;
        break;

      case '>':
        copter_setpoints.D -= 0.10f;
        break;

      case '+':
        copter_setpoints.throttle = 100;
        break;

    }

    //run the Senddata function, using tcpConn.getSocket() as the first argument
    //a character pointer to the data for the second argument,
    //and the size of the data to send (in bytes) (1char = 1byte)
    /*
    for (size_t i = 0; i < PACKET_LENGTH; ++i) {
      cout << (int) copter_setpoints.data[i] << endl;
    }
    */
    cout << "Roll: " << copter_setpoints.set_roll << "\t" <<
    "Pitch: " << copter_setpoints.set_pitch << "\t" <<
    "Throttle: " << (int) copter_setpoints.throttle << "\t" <<
      "P: " << copter_setpoints.P << "\t" <<
     "I: " << copter_setpoints.I << "\t" <<
     "D: " << copter_setpoints.D << endl;

    tcpConn.sendData(tcpConn.getSocket(), (char *)copter_setpoints.data,
        PACKET_LENGTH);

  } //end while

  return 0;
}
