#include <iostream>
#include <string>

#include "face_detect.h"

#if 0
int main(int argc, char**argv)
{
	std::cout << test_singleton::GetInstance().test() << std::endl;
	return 0;
}
#endif

int main(int argc, char**argv)
{
	face_detect fd = face_detect::GetInstance();
	fd.setUpService();
	fd.startMainLoop();
	
	pthread_join(fd.face_main_thread, NULL);

	return 0;
}
