#include <iostream>
#include <string>
#include "tcpipnix.h"
#include "getch.h"
#include <cstdint>
#include <pthread.h>

using namespace std;
using namespace term;

#define PACKET_LENGTH 26

/*------------------------------*/
/*      STRUCT DEFINITIONS      */
/*------------------------------*/
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


/*------------------------------*/
/*      FUNCTION PROTOTYPES     */
/*------------------------------*/
void interpretKeys(const char, copter_setpoints_t &);
void keyThread(void *args);
void heartbeatThread(void *args);

/*------------------------------*/
/*       GLOBAL VARIABLES       */
/*------------------------------*/
copter_setpoints_t copter_setpoints = {0};

int main(int argc, char ** argv) {
  TCP tcpConn;

  pthread_t key_thread;
  pthread_t heartbeat_thread;

  //Specify a port to connect to
  int port = 5555;
  string addr = "127.0.0.1";

  //Specify an IP address such as the above
  if (argc > 1) {
    switch (argc) {
      case 7:
        copter_setpoints.set_pitch = atof(argv[6]);
      case 6:
        copter_setpoints.set_roll = atof(argv[5]);
      case 5:
        copter_setpoints.D = atof(argv[4]);
      case 4:
        copter_setpoints.I = atof(argv[3]);
      case 3:
        copter_setpoints.P = atof(argv[2]);
      case 2:
        addr = string(argv[1]);
        break;
      default:
        break;
    }
  }
  else {
    cout <<
      "Usage: " <<
      argv[0] <<
      " <ip_address> [initial_p] [initial_i] [initial_d]" <<
      "[initial_roll] [initial_pitch]" << endl;
    return 1;
  }

  cout << "Client initialized to: " <<
    "IP address:\t" << addr << endl <<
    "Copter P:\t"   << copter_setpoints.P << endl <<
    "Copter I:\t"   << copter_setpoints.I << endl <<
    "Copter D:\t"   << copter_setpoints.D << endl <<
    "Copter roll:\t"   << copter_setpoints.set_roll << endl <<
    "Copter pitch:\t"  << copter_setpoints.set_pitch << endl;

  cout << "connecting" << endl;
  if (!tcpConn.connectToHost(port, addr.c_str()))
    return 1;

  initTermios(0);

  cout << "connected" << endl;

  //main loop
  while (1)
  {

    char input = getch();
    interpretKeys(input, copter_setpoints);

    cout <<
      "Roll: "      << copter_setpoints.set_roll << "\t" <<
      "Pitch: "     << copter_setpoints.set_pitch << "\t" <<
      "Throttle: "  << (int) copter_setpoints.throttle << "\t" <<
      "P: "         << copter_setpoints.P << "\t" <<
      "I: "         << copter_setpoints.I << "\t" <<
      "D: "         << copter_setpoints.D << endl;

    tcpConn.sendData(tcpConn.getSocket(), (char *)copter_setpoints.data,
        PACKET_LENGTH);

  } //end while

  return 0;
}

/*------------------------------*/
/*        INTERPRETKEYS         */
/*------------------------------*/
void interpretKeys(const char input, copter_setpoints_t &copter_setpoints) {
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
    } //end switch
} //end handleKeys
