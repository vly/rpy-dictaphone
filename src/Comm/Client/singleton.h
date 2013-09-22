/**
 * Management the single service instance in the service
 * Created by Alfred yang
 * 13-09-2013
 */

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template<typename T>
class Singleton
{
protected:
	static T* _instance;
public:
	static T* GetInstance()
	{
		if(!_instance)
			_instance = new T();
		
		return _instance;
	}
};

template<typename T>
T* Singleton<T>:: _instance = 0;

#endif //__SINGLETON_H__
