#ifndef __SYS_LOCK_H__
#define __SYS_LOCK_H__

#if defined(__NEWLIB_GTHR_GENERIC)

/* Use gthr-generic lock hooks.  */

#include <gthr-generic.h>

typedef __gthread_mutex_t _LOCK_T;
typedef __gthread_recursive_mutex_t _LOCK_RECURSIVE_T;
 
#define __LOCK_INIT(class,lock)				\
  class __gthread_mutex_t lock;				\
  static __attribute__ ((__constructor__ (0))) void	\
  __init_##lock (void)					\
  {							\
    __generic_gxx_mutex_init_function (&lock);		\
  }
#define __LOCK_INIT_RECURSIVE(class,lock)			\
  class __gthread_recursive_mutex_t lock;			\
  static __attribute__ ((__constructor__ (0))) void		\
  __init_##lock (void)						\
  {								\
    __generic_gxx_recursive_mutex_init_function (&lock);	\
  }
#define __lock_init(lock) (__generic_gxx_mutex_init_function (&(lock)))
#define __lock_init_recursive(lock) \
  (__generic_gxx_recursive_mutex_init_function (&(lock)))
#define __lock_close(lock) (__generic_gxx_mutex_destroy (&(lock)))
#define __lock_close_recursive(lock) \
  (__generic_gxx_recursive_mutex_destroy (&(lock)))
#define __lock_acquire(lock) (__generic_gxx_mutex_lock (&(lock)))
#define __lock_acquire_recursive(lock) \
  (__generic_gxx_recursive_mutex_lock (&(lock)))
#define __lock_try_acquire(lock) (__generic_gxx_mutex_trylock (&(lock)))
#define __lock_try_acquire_recursive(lock) \
  (__generic_gxx_recursive_mutex_trylock (&(lock)))
#define __lock_release(lock) (__generic_gxx_mutex_unlock (&(lock)))
#define __lock_release_recursive(lock) \
  (__generic_gxx_recursive_mutex_unlock (&(lock)))

#else

/* dummy lock routines for single-threaded aps */

#include <newlib.h>
#include <_ansi.h>

#if !defined(_RETARGETABLE_LOCKING)

typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;

#define __LOCK_INIT(class,lock) static int lock = 0;
#define __LOCK_INIT_RECURSIVE(class,lock) static int lock = 0;
#define __lock_init(lock) ((void) 0)
#define __lock_init_recursive(lock) ((void) 0)
#define __lock_close(lock) ((void) 0)
#define __lock_close_recursive(lock) ((void) 0)
#define __lock_acquire(lock) ((void) 0)
#define __lock_acquire_recursive(lock) ((void) 0)
#define __lock_try_acquire(lock) ((void) 0)
#define __lock_try_acquire_recursive(lock) ((void) 0)
#define __lock_release(lock) ((void) 0)
#define __lock_release_recursive(lock) ((void) 0)

#else

#ifdef __cplusplus
extern "C" {
#endif

struct __lock;
typedef struct __lock * _LOCK_T;
#define _LOCK_RECURSIVE_T _LOCK_T

#define __LOCK_INIT(class,lock) extern struct __lock __lock_ ## lock; \
	class _LOCK_T lock = &__lock_ ## lock
#define __LOCK_INIT_RECURSIVE(class,lock) __LOCK_INIT(class,lock)

extern void __retarget_lock_init(_LOCK_T *lock);
#define __lock_init(lock) __retarget_lock_init(&lock)
extern void __retarget_lock_init_recursive(_LOCK_T *lock);
#define __lock_init_recursive(lock) __retarget_lock_init_recursive(&lock)
extern void __retarget_lock_close(_LOCK_T lock);
#define __lock_close(lock) __retarget_lock_close(lock)
extern void __retarget_lock_close_recursive(_LOCK_T lock);
#define __lock_close_recursive(lock) __retarget_lock_close_recursive(lock)
extern void __retarget_lock_acquire(_LOCK_T lock);
#define __lock_acquire(lock) __retarget_lock_acquire(lock)
extern void __retarget_lock_acquire_recursive(_LOCK_T lock);
#define __lock_acquire_recursive(lock) __retarget_lock_acquire_recursive(lock)
extern int __retarget_lock_try_acquire(_LOCK_T lock);
#define __lock_try_acquire(lock) __retarget_lock_try_acquire(lock)
extern int __retarget_lock_try_acquire_recursive(_LOCK_T lock);
#define __lock_try_acquire_recursive(lock) \
  __retarget_lock_try_acquire_recursive(lock)
extern void __retarget_lock_release(_LOCK_T lock);
#define __lock_release(lock) __retarget_lock_release(lock)
extern void __retarget_lock_release_recursive(_LOCK_T lock);
#define __lock_release_recursive(lock) __retarget_lock_release_recursive(lock)

#ifdef __cplusplus
}
#endif

#endif /* !defined(_RETARGETABLE_LOCKING) */

#endif

#endif /* __SYS_LOCK_H__ */
