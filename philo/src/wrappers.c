/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:59:36 by cgray             #+#    #+#             */
/*   Updated: 2024/05/29 14:09:49 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



/* might want to copy the actual error codes/messages
	if running into mutex errors, but for now just exit
 */
static void	mutex_error(int	status, t_thread_codes code)
{
	if (status == 0)
		return ;
	if (status == EINVAL && (LOCK == code || UNLOCK == code))
		display_error("The value specified by mutex is invalid");
	else if (status == EINVAL && INIT == code)
		display_error("The value specified by attr is invalid.");
	else if (status == EDEADLK)
	{
		display_error("A deadlock would occur if the thread "
			"blocked waiting for mutex.");
	}
	else if (status == EPERM)
		display_error("The current thread does not hold a lock on mutex.");
	else if (status == ENOMEM)
	{
		display_error("The process cannot allocate enough memory"
			" to create another mutex.");
	}
	else if (status == EBUSY)
		display_error("Mutex is locked");
	else
		display_error("Mutex Failure???");
}

static void	thread_error(int status, t_thread_codes code)
{
	if (status == 0)
		return ;
	if (status == EAGAIN)
		display_error("No resources to create another thread");
	else if (status == EPERM)
		display_error("The caller does not have appropriate permission\n");
	else if (status == EINVAL && CREATE == code)
		display_error("The value specified by attr is invalid.");
	else if (status == EINVAL && (JOIN == code || DETACH == code))
		display_error("The value specified by thread is not joinable\n");
	else if (status == ESRCH)
	{
		display_error("No thread could be found corresponding to that"
			"specified by the given thread ID, thread.");
	}
	else if (status == EDEADLK)
	{
		display_error("A deadlock was detected or the value of"
			"thread specifies the calling thread.");
	}
	else
		display_error("Thread Failure???");
}

/* malloc wrapper to exit if allocation fails */
void	*cool_malloc(size_t bytes)
{
	void	*alloc;

	alloc = malloc(bytes);
	if (alloc == NULL)
		display_error("Malloc failed?????");
	return (alloc);
}

/* mutex wrapper that will exit if thrown error code
thanks Oceano <3
*/
void	mutex(t_mutex *mutex, t_thread_codes code)
{
	if (code == LOCK)
		mutex_error(pthread_mutex_lock(mutex), code);
	else if (code == UNLOCK)
		mutex_error(pthread_mutex_unlock(mutex), code);
	else if (code == INIT)
		mutex_error(pthread_mutex_init(mutex, NULL), code);
	else if (code == DESTROY)
		mutex_error(pthread_mutex_destroy(mutex), code);
	else
		display_error("Mutex handle misuse");
}

/* thread function wrapper that will exit if thrown error code
	Parameters:
		- thread,
		- function ptr to routine functions
		- global data struct (to be cast as t_global)
		- thread code name
*/
void	thread(pthread_t *thread, void *(*routine)(void *),
		void *global, t_thread_codes code)
{
	if (code == CREATE)
		thread_error(pthread_create(thread, NULL, routine, global), code);
	else if (code == JOIN)
		thread_error(pthread_join(*thread, NULL), code);
	else if (code == DETACH)
		thread_error(pthread_detach(*thread), code);
	else
		display_error("Thread handle misuse");
}
