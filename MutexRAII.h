//
// Created by lianyi on 18-3-19.
//

#ifndef MYSPIDER4_MUTEXRAII_H
#define MYSPIDER4_MUTEXRAII_H

#include <pthread.h>

template<typename _Mutex>
class MutexRAII {


public:
    typedef _Mutex mutex_type;

    explicit MutexRAII(mutex_type& __m) : _M_device(__m)
    { pthread_mutex_lock(&_M_device); }

//    lock_guard(mutex_type& __m, adopt_lock_t) : _M_device(__m)
//    { } // calling thread owns mutex
    ~MutexRAII()
    { pthread_mutex_unlock(&_M_device); }

    MutexRAII(const MutexRAII&) = delete;
    MutexRAII& operator=(const MutexRAII&) = delete;

private:
    mutex_type&  _M_device;

};


#endif //MYSPIDER4_MUTEXRAII_H
