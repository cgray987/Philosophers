/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:24:41 by cgray             #+#    #+#             */
/*   Updated: 2024/05/29 15:44:55 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* returns current time in microsec */
static size_t	get_time_us(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		display_error("gettimeofday error???");
	return (time.tv_sec * 1e6 + time.tv_usec);
}

int	ft_msleep(size_t ms)
{
	size_t	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < ms)
		usleep(10);
	return (0);
}

/* more precise usleep using spinlock */
void	p_delay(long wait, t_global *global)
{
	long	start;
	long	time_since;
	long	remaining;

	start = get_time_us();
	while ((long)get_time_us() - start < wait*1e3)
	{
		if (dinner_done(global))
			break;
		time_since = get_time_us();
		remaining = wait*1e3 - time_since;
		if (remaining > 1e4)
			usleep(remaining / 2);
		else
		{
			while ((long)get_time_us() - start < wait*1e3)
				;
		}
	}
}

/* returns current time in millisec */
size_t	get_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		display_error("gettimeofday error???");
	return (time.tv_sec * 1e3 + time.tv_usec / 1e3);
}

/* waits until all threads have started by running while loop
	until thread_sync flag is true */
void	wait_for_thread_sync(t_global *global)
{
	while (!get_bool(&global->global_mutex, &global->thread_sync))
		;
}

bool	all_threads_running(t_mutex *mtx, long *threads, long num_of_philos)
{
	bool	ret;

	ret = false;
	mutex(mtx, LOCK);
	if (*threads == num_of_philos)
		ret = true;
	mutex(mtx, UNLOCK);
	return (ret);
}

void	un_sync(t_philo *philo)
{
	if (philo->global->nbr_of_philos % 2 == 0)
	{
		if (philo->pos % 2 == 0)
			p_delay(30, philo->global);
	}
	else
	{
		if (philo->pos % 2)
			thinking(philo, true);
	}
}

