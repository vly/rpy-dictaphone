#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template<typename T>
class Singleton {
protected:

  static T *_instance;

public:

  static T* GetInstance()
  {
    if (!_instance) _instance = new T();

    return _instance;
  }
};

template<typename T>
T * Singleton<T>::_instance = 0;

#endif