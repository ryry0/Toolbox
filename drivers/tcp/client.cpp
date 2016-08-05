#include <iostream>
#include <string>
#include "tcpipnix.h"
#include "getch.h"
#include <cstdint>
#include <pthread.h>
#include <unistd.h> //for usleep

using namespace std;
using namespace term;

#define PACKET_LENGTH 27
#define USLEEP_INTERVAL 500000 //sleep for .5 secs
#define MAX_THROTTLE 1000
#define MIN_THROTTLE 0

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
    uint16_t throttle;
    uint8_t hard_kill;
  };
  uint8_t data[PACKET_LENGTH];
} copter_setpoints_t;


/*------------------------------*/
/*      FUNCTION PROTOTYPES     */
/*------------------------------*/
void interpretKeys(const char);
void *keyThread(void *args);
void *controllerThread(void *args);
void *heartbeatThread(void *args);

/*------------------------------*/
/*       GLOBAL VARIABLES       */
/*------------------------------*/
copter_setpoints_t copter_setpoints = {0};
pthread_mutex_t copter_mutex;

int main(int argc, char ** argv) {
  TCP tcpConn;

  pthread_t key_thread;
  pthread_t heartbeat_thread;
  pthread_attr_t attr;

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
      " [initial_roll] [initial_pitch]" << endl;
    return 1;
  }

  cout << "Client initialized to: " << endl <<
    "IP address:\t" << addr << endl <<
    "Copter P:\t"   << copter_setpoints.P << endl <<
    "Copter I:\t"   << copter_setpoints.I << endl <<
    "Copter D:\t"   << copter_setpoints.D << endl <<
    "Copter roll:\t"   << copter_setpoints.set_roll << endl <<
    "Copter pitch:\t"  << copter_setpoints.set_pitch << endl;

  //Run initializations
  initTermios(0); //init getch()
  pthread_mutex_init(&copter_mutex, NULL); //init mutex

  //connect to quadcopter
  cout << "connecting" << endl;
  if (!tcpConn.connectToHost(port, addr.c_str()))
    return 1;
  cout << "connected" << endl;

  if (pthread_create(&key_thread, NULL, keyThread, &tcpConn)  != 0) {
    cout << "Could not spawn key_thread" << endl;
    return 1;
  }

  if (pthread_create(&heartbeat_thread, NULL, heartbeatThread, &tcpConn) != 0) {
    cout << "Could not spawn heartbeat_thread" << endl;
    return 1;
  }

  //wait until the threads join
  pthread_join(key_thread, NULL);
  pthread_join(heartbeat_thread, NULL);

  return 0;
}

/*------------------------------*/
/*        HEARTBEATTHREAD       */
/*------------------------------*/

void *heartbeatThread(void *args) {
  TCP *connection = (TCP*) args;

  for (;;) {
    pthread_mutex_lock(&copter_mutex);
    connection->sendData(connection->getSocket(), (char *)copter_setpoints.data,
        PACKET_LENGTH);
    pthread_mutex_unlock(&copter_mutex);

    usleep(USLEEP_INTERVAL);
  }
}

/*------------------------------*/
/*          KEYTHREAD           */
/*------------------------------*/

void *keyThread(void *args) {
  TCP *connection = (TCP*) args;
  for (;;) {
    char input = getch();

    pthread_mutex_lock(&copter_mutex);
    interpretKeys(input);
    connection->sendData(connection->getSocket(), (char *)copter_setpoints.data,
        PACKET_LENGTH);
    pthread_mutex_unlock(&copter_mutex);

    cout <<
      "Roll: "      << copter_setpoints.set_roll << "\t" <<
      "Pitch: "     << copter_setpoints.set_pitch << "\t" <<
      "Throttle: "  << (int) copter_setpoints.throttle << "\t" <<
      "P: "         << copter_setpoints.P << "\t" <<
      "I: "         << copter_setpoints.I << "\t" <<
      "D: "         << copter_setpoints.D << endl;

  } //end for
}
/*------------------------------*/
/*        INTERPRETKEYS         */
/*------------------------------*/
void interpretKeys(const char input) {
    switch(input) {
      case 'd':
        copter_setpoints.set_roll += 1.0f;
        break;
      case 'a':
        copter_setpoints.set_roll -= 1.0f;
        break;
      case 'w':
        copter_setpoints.set_pitch += 1.0f;
        break;
      case 's':
        copter_setpoints.set_pitch -= 1.0f;
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
        copter_setpoints.throttle += 50;
        if (copter_setpoints.throttle > MAX_THROTTLE)
          copter_setpoints.throttle = MAX_THROTTLE;
        break;

      case 'J':
        if (copter_setpoints.throttle <= MIN_THROTTLE)
          copter_setpoints.throttle = MIN_THROTTLE;
        else
          copter_setpoints.throttle -= 50;
        break;

      case 'u':
        copter_setpoints.throttle += 10;
        if (copter_setpoints.throttle > MAX_THROTTLE)
          copter_setpoints.throttle = MAX_THROTTLE;
        break;

      case 'j':
        if (copter_setpoints.throttle <= MIN_THROTTLE)
          copter_setpoints.throttle = MIN_THROTTLE;
        else
          copter_setpoints.throttle -= 10;
        break;

      case ' ':
        copter_setpoints.set_yaw = 0;
        copter_setpoints.set_roll = 0;
        copter_setpoints.set_pitch = 0;
        copter_setpoints.throttle = MIN_THROTTLE;
        copter_setpoints.hard_kill = 1;
        break;

      case 'm':
        copter_setpoints.set_yaw = 0;
        copter_setpoints.set_roll = 0;
        copter_setpoints.set_pitch = 0;
        copter_setpoints.throttle = MIN_THROTTLE;
        break;

        //P defines
      case ')':
        copter_setpoints.P += 1.0f;
        break;

      case '0':
        copter_setpoints.P += 0.01f;
        break;

      case 'p':
        copter_setpoints.P += 0.001f;
        break;

      case ';':
        copter_setpoints.P -= 0.001f;
        break;

      case '/':
        copter_setpoints.P -= 0.01f;
        break;

      case '?':
        copter_setpoints.P -= 1.0f;
        break;

        //I defines
      case '*':
        //copter_setpoints.I += 10.0f;
        break;

      case '8':
        copter_setpoints.I += .001f;
        break;

      case 'i':
        copter_setpoints.I += 0.0001f;
        break;

      case 'k':
        copter_setpoints.I -= 0.0001f;
        break;

      case ',':
        copter_setpoints.I -= 0.001f;
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
        //copter_setpoints.throttle = 100;
        break;
    } //end switch
} //end handleKeys
