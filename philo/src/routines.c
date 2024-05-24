/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:44:12 by cgray             #+#    #+#             */
/*   Updated: 2024/05/23 17:47:25 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* locks and picks up fork */
void	pickup_fork(t_id *id, int fork_position)
{
	pthread_mutex_lock(&(id->mutexes[fork_position]));
	pthread_mutex_lock(&(id->philo->write_mutex));
	id->philo->forks[fork_position] = 0;
	logging("has taken a fork", id, 'f');
	pthread_mutex_unlock(&(id->philo->write_mutex));
}

/* unlocks and puts fork down */
void	putdown_fork(t_id *id, int fork_position)
{
	pthread_mutex_unlock(&(id->mutexes[fork_position]));
	pthread_mutex_lock(&(id->philo->write_mutex));
	id->philo->forks[fork_position] = 1;
	pthread_mutex_unlock(&(id->philo->write_mutex));
}

/* if available, grabs left and right forks.
-- if last philo, left fork is fork 1
-- else left fork is id + 1
- call log to eat
- check if time to eat > time to die
- put down forks
- increment times eaten */
int	eating(t_id *id, size_t *last_ate)
{
	int	left;
	int	right;

	right = id->id;
	if (id->id == id->philo->num_philos - 1)
		left = 0;
	else
		left = id->id + 1;
	handle_forks(id, left, right, pickup_fork);
	logging("is eating", id, 'e');
	*last_ate = ft_get_time();
	pthread_mutex_lock(&(id->philo->write_mutex));
	id->philo->time_from_meal = *last_ate;
	pthread_mutex_unlock(&(id->philo->write_mutex));
	if (id->philo->time_to_eat > id->philo->time_to_die)
	{
		handle_forks(id, left, right, putdown_fork);
		ft_msleep(id->philo->time_to_die);
		return (1);
	}
	ft_msleep(id->philo->time_to_eat);
	handle_forks(id, left, right, putdown_fork);
	id->times_eaten++;
	return (0);
}

/* think routine, think_time can be zero
checks if will die before msleep think_time*/
void	thinking(t_id *id, size_t last_ate)
{
	size_t	time;
	long	think_time;

	time = ft_get_time();
	last_ate = time;
	think_time = 2 * (id->philo->time_to_eat - id->philo->time_to_sleep);
	// if (perished(id, time - last_ate, think_time))
	// 	return ;
	logging("is thinking", id, 't');
}

/* sleep routine, checks if will die before msleep */
void	sleeping(t_id *id, size_t last_ate)
{
	size_t	time;

	time = ft_get_time();
	last_ate = time;
	logging("is sleeping", id, 's');
	// if (perished(id, time - last_ate, id->philo->time_to_sleep))
	// 	return ;
	ft_msleep(id->philo->time_to_sleep);
}
