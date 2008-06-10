#ifndef SWITCH_HEADER
#define SWITCH_HEADER

/***************************************************************************
 *            switch.h
 *	kind of semaphore
 *  Sat Aug 24 23:53:59 2002
 *  Copyright  2002  Roman Dementiev
 *  dementiev@mpi-sb.mpg.de
 ****************************************************************************/

#include <stxxl/bits/noncopyable.h>
#include "stxxl/bits/common/utils.h"

#ifdef STXXL_BOOST_THREADS
 #include <boost/thread/mutex.hpp>
 #include <boost/thread/condition.hpp>
#else
 #include <pthread.h>
#endif


__STXXL_BEGIN_NAMESPACE

//#define onoff_switch Switch

class onoff_switch : private noncopyable
{
#ifdef STXXL_BOOST_THREADS
    boost::mutex mutex;
    boost::condition cond;
#else
    pthread_mutex_t mutex;
    pthread_cond_t cond;
#endif
    bool _on;

public:
    onoff_switch (bool flag = false) : _on (flag)
    {
#ifndef STXXL_BOOST_THREADS
        check_pthread_call(pthread_mutex_init(&mutex, NULL));
        check_pthread_call(pthread_cond_init(&cond, NULL));
#endif
    }
    ~onoff_switch ()
    {
#ifndef STXXL_BOOST_THREADS
        int res = pthread_mutex_trylock (&mutex);

        if (res == 0 || res == EBUSY) {
            check_pthread_call(pthread_mutex_unlock(&mutex));
        } else
            stxxl_function_error(resource_error);
        check_pthread_call(pthread_mutex_destroy(&mutex));


        check_pthread_call(pthread_cond_destroy(&cond));
#endif
    }
    void on ()
    {
#ifdef STXXL_BOOST_THREADS
        boost::mutex::scoped_lock Lock(mutex);
        _on = true;
        Lock.unlock();
        cond.notify_one();
#else
        check_pthread_call(pthread_mutex_lock(&mutex));
        _on = true;
        check_pthread_call(pthread_mutex_unlock(&mutex));
        check_pthread_call(pthread_cond_signal(&cond));
#endif
    }
    void off ()
    {
#ifdef STXXL_BOOST_THREADS
        boost::mutex::scoped_lock Lock(mutex);
        _on = false;
        Lock.unlock();
        cond.notify_one();
#else
        check_pthread_call(pthread_mutex_lock(&mutex));
        _on = false;
        check_pthread_call(pthread_mutex_unlock(&mutex));
        check_pthread_call(pthread_cond_signal(&cond));
#endif
    }
    void wait_for_on ()
    {
#ifdef STXXL_BOOST_THREADS
        boost::mutex::scoped_lock Lock(mutex);
        if (!_on)
            cond.wait(Lock);

#else
        check_pthread_call(pthread_mutex_lock(&mutex));
        if (!_on)
            check_pthread_call(pthread_cond_wait(&cond, &mutex));

        check_pthread_call(pthread_mutex_unlock(&mutex));
#endif
    }
    void wait_for_off ()
    {
#ifdef STXXL_BOOST_THREADS
        boost::mutex::scoped_lock Lock(mutex);
        if (_on)
            cond.wait(Lock);

#else
        check_pthread_call(pthread_mutex_lock(&mutex));
        if (_on)
            check_pthread_call(pthread_cond_wait(&cond, &mutex));

        check_pthread_call(pthread_mutex_unlock(&mutex));
#endif
    }
    bool is_on ()
    {
#ifdef STXXL_BOOST_THREADS
        boost::mutex::scoped_lock Lock(mutex);
        return _on;
#else
        bool res;
        check_pthread_call(pthread_mutex_lock(&mutex));
        res = _on;
        check_pthread_call(pthread_mutex_unlock(&mutex));
        return res;
#endif
    }
};

__STXXL_END_NAMESPACE

#endif
