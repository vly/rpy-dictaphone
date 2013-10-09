#include "comm.h"
#include "singleton.h"
#include "base_service.h"

class Client : public Comm
               , public Singleton<Client>
               , public base_service {
  pthread_t input_thread;
  bool isRecording;

public:

  bool isRunning;

  Client();
  ~Client();

  virtual bool setUpService();
  virtual void startMainLoop();
  virtual void stopMainLoop();
  virtual void receiveMessage(const Message& m);
  virtual bool dispatchMessage(Message *m);

private:

  void connectToServer();
  int  sendCommand(char *);
  void startAudioRecording();
  void stopAudioRecording();
};
