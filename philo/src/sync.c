/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:24:41 by cgray             #+#    #+#             */
/*   Updated: 2024/05/30 17:02:51 by cgray            ###   ########.fr       */
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

/* more precise usleep using spinlock
note, wait is in microseconds */
void	p_delay(long wait_us, t_global *global)
{
	long	start;
	long	time_since;
	long	remaining;

	start = get_time_us();
	while ((long)get_time_us() - start < wait_us)
	{
		if (dinner_done(global))
			break ;
		time_since = get_time_us() - start;
		remaining = wait_us - time_since;
		if (remaining > 1e4)
			usleep(remaining / 2);
		else
		{
			while ((long)get_time_us() - start < wait_us)
			{
				;
			}
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

/* used by monitor to wait until all philo threads have started */
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

/* used for fairness, making sure philos don't eat twice in a row

if even group delay 30 ms to stagger start
if odd group, start thinking routine before going into eat routine */
void	un_sync(t_philo *philo)
{
	if (philo->global->nbr_of_philos % 2 == 0)
	{
		if (philo->pos % 2 == 0)
			p_delay(3e4, philo->global);
	}
	else
	{
		if (philo->pos % 2)
			thinking(philo, true);
	}
}
