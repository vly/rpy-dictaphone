#include "maestro.h"
#define NA_STEP_VALUE 150

int Maestro::goHome(Servo *servo) {
  unsigned char command[] = { 0xA2, servo->getChannel() };

  if (write(_fd, command, sizeof(command)) == -1) {
    perror("error writing");
    return -1;
  }

  servo->setPos(servo->getHome());

  return 0;
}

int Maestro::getPosition(Servo *servo) {
  return (int)servo->getPos();
}

int Maestro::setPosition(Servo *servo, unsigned short target) {
  if (target > servo->getMax()) target = servo->getMax();

  if (target < servo->getMin()) target = servo->getMin();
  unsigned char command[] =
  { 0x84, servo->getChannel(), static_cast<unsigned char>(target & 0x7F),
    static_cast<unsigned char>(target >> 7 & 0x7F) };

  if (write(_fd, command, sizeof(command)) == -1) {
    perror("error writing");
    return -1;
  }

  servo->setPos(target);
  return 0;
}

int Maestro::stepUp(Servo *servo) {
  return setPosition(servo, getPosition(servo) + NA_STEP_VALUE);
}

int Maestro::stepDown(Servo *servo) {
  return setPosition(servo, getPosition(servo) - NA_STEP_VALUE);
}

#define KEYCONTROL 0

#if KEYCONTROL
# include <termios.h>
# include <unistd.h>
#endif // if KEYCONTROL

void Maestro::openDevice()
{
  _fd = open(_device, O_RDWR | O_NOCTTY);

  if (_fd == -1) {
    perror(_device);
    throw MaestroException("Invalid Device");
  }
}

bool Maestro::setUpService()
{
  openDevice();
  horizontalServo = new Servo(0, 3968, 9216, 6104);
  verticalServo   = new Servo(1, 3968, 9216, 8220);

  this->goHome(horizontalServo);
  this->goHome(verticalServo);
}

void Maestro::startMainLoop()
{}

void Maestro::stopMainLoop()
{}

void Maestro::receiveMessage(const Message& m)
{}

#undef NA_STEP_VALUE
