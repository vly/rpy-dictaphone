#ifndef __maestro__
#define __maestro__

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "singleton.h"
#include "base_service.h"

// Source:
// http://stackoverflow.com/questions/134569/c-exception-throwing-stdstring
struct MaestroException : public std::exception
{
  std::string s;
  MaestroException(std::string ss) : s(ss) {}

  ~MaestroException() throw() {}

  const char* what() const throw() {
    return s.c_str();
  }
};

#ifndef __servo__
# define __servo__

class Servo {
private:

  unsigned char  _channel;
  unsigned int   _min;
  unsigned int   _max;
  unsigned short _pos;
  unsigned short _home;

public:

  Servo(unsigned char c, unsigned int mn, unsigned int mx,
        unsigned int hm) : _channel(c), _min(mn), _max(mx), _home(hm) {}

  unsigned char getChannel() {
    return _channel;
  }

  unsigned short getHome() {
    return _home;
  }

  unsigned short getMin() {
    return _min;
  }

  unsigned short getMax() {
    return _max;
  }

  unsigned short getPos() {
    return _pos;
  }

  void setPos(unsigned short p) {
    _pos = p;
  }
};

#endif /* defined(__servo__) */

#ifdef __APPLE__
# define NA_DEVICE "/dev/cu.usbmodem00065291"
#else
# define NA_DEVICE "/dev/ttyACM0"
#endif // ifdef __APPLE__

class Maestro
  : public Singleton<Maestro>
    , public base_service {
private:

  const char *_device;
  int         _fd;

public:

  Servo *horizontalServo;
  Servo *verticalServo;

public:

  Maestro() : _device(NA_DEVICE) {}

  ~Maestro() {
    close(_fd);
  }

  int          getError(Servo *servo);
  int          goHome(Servo *);
  int          getPosition(Servo *);
  int          setPosition(Servo    *,
                           unsigned short);
  int          stepUp(Servo *);
  int          stepDown(Servo *);

  virtual bool setUpService();
  virtual void startMainLoop();
  virtual void stopMainLoop();
  virtual void receiveMessage(const Message& m);

private:

  void openDevice();
};

#endif // ifndef __maestro__
