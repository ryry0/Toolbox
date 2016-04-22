#include <iostream>
#include <iomanip>
#include <string>
#include "tcpipnix.h"
#include "getch.h"
#include "serialnix.h"
#include <cstdint>
#include <pthread.h>
#include <unistd.h> //for usleep

using namespace std;
using namespace term;

#define PACKET_LENGTH 26
#define USLEEP_INTERVAL 500000 //sleep for .5 secs
#define CONT_PACKET_LENGTH 13
//#define ENABLE_TCP

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

typedef union controller_packet_t {
  struct {
    float roll;
    float pitch;
    float yaw;
    uint8_t throttle;
  };
  uint8_t data[CONT_PACKET_LENGTH];
} controller_packet_t;

typedef struct cont_thread_args_t {
  int serial_fd;
  TCP *tcpConn_p;
} cont_thread_args_t;

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
  pthread_t controller_thread;
  pthread_attr_t attr;

  int port = 5555;
  int serial_fd = 0;
  string addr = "127.0.0.1";
  string serial_addr = "127.0.0.1";

  if (argc > 1) {
    switch (argc) {
      case 8:
        copter_setpoints.set_pitch = atof(argv[7]);
      case 7:
        copter_setpoints.set_roll = atof(argv[6]);
      case 6:
        copter_setpoints.D = atof(argv[5]);
      case 5:
        copter_setpoints.I = atof(argv[4]);
      case 4:
        copter_setpoints.P = atof(argv[3]);
      case 3:
        serial_addr = string(argv[2]);
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
      " <ip_address> <serial_port> [initial_p] [initial_i] [initial_d]" <<
      " [initial_roll] [initial_pitch]" << endl;
    return 1;
  }

  cout << "Client initialized to: " <<
    "IP address:\t" << addr << endl <<
    "Serial address:\t" << serial_addr << endl <<
    "Copter P:\t"   << copter_setpoints.P << endl <<
    "Copter I:\t"   << copter_setpoints.I << endl <<
    "Copter D:\t"   << copter_setpoints.D << endl <<
    "Copter roll:\t"   << copter_setpoints.set_roll << endl <<
    "Copter pitch:\t"  << copter_setpoints.set_pitch << endl;

  //Run initializations
  initTermios(0); //init getch()
  serial_fd = open_port(serial_addr.c_str());
  if (serial_fd == -1) {
    cout << "Could not connect to serial port at " << serial_addr << endl;
    return 1;
  }
  if (init_serial_port(serial_fd) == -1) {
    cout << "Could not intialize serial port" << endl;
    return 1;
  }

#ifdef ENABLE_TCP
  //connect to quadcopter
  cout << "connecting" << endl;
  if (!tcpConn.connectToHost(port, addr.c_str()))
    return 1;
  cout << "connected" << endl;
#endif

  pthread_mutex_init(&copter_mutex, NULL); //init mutex

  if (pthread_create(&key_thread, NULL, keyThread, &tcpConn)  != 0) {
    cout << "Could not spawn key_thread" << endl;
    return 1;
  }

  cont_thread_args_t cont_args;
  cont_args.serial_fd = serial_fd;
  cont_args.tcpConn_p = &tcpConn;

  if (pthread_create(&controller_thread, NULL, controllerThread, &cont_args)  != 0) {
    cout << "Could not spawn controllerThread" << endl;
    return 1;
  }

  if (pthread_create(&heartbeat_thread, NULL, heartbeatThread, &tcpConn) != 0) {
    cout << "Could not spawn heartbeat_thread" << endl;
    return 1;
  }

  //wait until the threads join
  pthread_join(key_thread, NULL);
  pthread_join(heartbeat_thread, NULL);

  close(serial_fd);
  return 0;
}

/*------------------------------*/
/*        HEARTBEATTHREAD       */
/*------------------------------*/

void *heartbeatThread(void *args) {
  TCP *connection = (TCP*) args;

  for (;;) {
    pthread_mutex_lock(&copter_mutex);
#ifdef ENABLE_TCP
    connection->sendData(connection->getSocket(), (char *)copter_setpoints.data,
        PACKET_LENGTH);
#endif
    pthread_mutex_unlock(&copter_mutex);

    usleep(USLEEP_INTERVAL);
  }
}


/*------------------------------*/
/*       CONTROLLERTHREAD       */
/*------------------------------*/

void *controllerThread(void *args) {
  cont_thread_args_t *cont_args = (cont_thread_args_t *) args;

  int serial_fd = cont_args->serial_fd;
  TCP *connection = cont_args->tcpConn_p;

  controller_packet_t controller_packet;

  float prev_pitch, prev_roll, prev_yaw;
  uint8_t prev_throttle;

  for (;;) {
    //fill until newline
    size_t total_bytes_read = 0;
    while (total_bytes_read < CONT_PACKET_LENGTH) {
      size_t bytes_read =
        read(serial_fd, &controller_packet.data[total_bytes_read], 1);

      if (bytes_read == -1)
        return NULL;

      //cout << total_bytes_read << endl;
      //to fix this need COBS
      if (bytes_read != 0)
        total_bytes_read++;
    }

    if ((prev_pitch != controller_packet.pitch) ||
        (prev_throttle != controller_packet.throttle) ||
        (prev_roll != controller_packet.roll) ||
        (prev_yaw != controller_packet.yaw)) {

      pthread_mutex_lock(&copter_mutex);

      copter_setpoints.set_pitch = controller_packet.pitch;
      copter_setpoints.set_yaw = controller_packet.yaw;
      copter_setpoints.set_roll = controller_packet.roll;
      copter_setpoints.throttle = controller_packet.throttle;

#ifdef ENABLE_TCP
      connection->sendData(connection->getSocket(), (char *)copter_setpoints.data,
          PACKET_LENGTH);
#endif
      pthread_mutex_unlock(&copter_mutex);

      /*
      cout << "Pitch " << controller_packet.pitch << "\t"
        << " Roll " << controller_packet.roll << "\t"
        << " Yaw " << controller_packet.yaw << "\t"
        << " Throttle " << (int) controller_packet.throttle << endl;
        */
    }

    for (int i = 0; i < CONT_PACKET_LENGTH; ++i) {
      cout << hex << (int) controller_packet.data[i] << "\t";
    }
    cout << endl;

    prev_pitch = controller_packet.pitch;
    prev_roll = controller_packet.roll;
    prev_yaw = controller_packet.yaw;
    prev_throttle = controller_packet.throttle;

  } //end for
} //end controllerThread

/*------------------------------*/
/*          KEYTHREAD           */
/*------------------------------*/

void *keyThread(void *args) {
  TCP *connection = (TCP*) args;
  for (;;) {
    char input = getch();

    pthread_mutex_lock(&copter_mutex);
    interpretKeys(input);
#ifdef ENABLE_TCP
    connection->sendData(connection->getSocket(), (char *)copter_setpoints.data,
        PACKET_LENGTH);
#endif
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
} //end interpretKeys
