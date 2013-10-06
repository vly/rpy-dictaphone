#include <iostream>
#include "face_detect.h"
#include "maestro.h"

int main(int argc, char** argv) {
  Maestro::GetInstance()->setUpService();
  face_detect::GetInstance()->setUpService();

  while(face_detect::GetInstance()->isMainThreadRunning()){
    printf("running...\n");
    usleep(100);
  }
  return 0;
}


