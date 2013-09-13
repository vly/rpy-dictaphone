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
	static T _instance;
	
public:
	static T& GetInstance()
	{
		return _instance;
	}
};

template<typename T>
T Singleton<T>:: _instance;

#endif //__SINGLETON_H__
