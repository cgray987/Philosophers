/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:44:12 by cgray             #+#    #+#             */
/*   Updated: 2024/05/06 16:50:58 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pickup_fork(t_id *id, int fork_position)
{
	// size_t	time;

	pthread_mutex_lock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 0;
	// time = ft_get_time() - id->philo->start_time;
	logging("has taken a fork", id, 'f');
}

void	putdown_fork(t_id *id, int fork_position)
{
	// size_t	time;

	pthread_mutex_unlock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 1;
	// time = ft_get_time() - id->philo->start_time;
	// printf("%zu %d has put down a fork\n", time, id->id + 1);
}

void	eating(t_id *id)
{
	// size_t	time;

	if (id->id == id->philo->num_philos - 1)
	{
		if (id->philo->forks[id->id] && id->philo->forks[1])
		{
			pickup_fork(id, id->id);
			pickup_fork(id, 1);
		}
		else
			return ;
	}
	else
	{
		if (id->philo->forks[id->id] && id->philo->forks[(id->id + 1)])
		{
			pickup_fork(id, id->id);
			pickup_fork(id, id->id + 1);
		}
		else
			return ;
	}
	// time = ft_get_time() - id->philo->start_time;
	// green();
	logging("is eating", id, 'e');
	// reset();
	ft_msleep(id->philo->time_to_eat);
	putdown_fork(id, id->id);
	if (id->id == id->philo->num_philos - 1)
		putdown_fork(id, 1);
	else
		putdown_fork(id, id->id + 1);
	// id->philo->time_from_meal = 0;
	id->times_eaten++;
	// printf("%d has eaten %d times.\n", id->id + 1, id->times_eaten);
}

void	thinking(t_id *id)
{
	size_t	time;

	time = ft_get_time() - id->philo->start_time;
	if ((2 * id->philo->time_to_eat - id->philo->time_to_sleep) > 0)
	{
		// blue();
		logging("is thinking", id, 't');
		// reset();
		// ft_msleep(100);
		if (time - id->philo->time_from_meal > 0)
			ft_msleep(2 * id->philo->time_to_eat - id->philo->time_to_sleep);
		else
		{
			id->philo->perished = 1;
			logging("died", id, 'd');
		}
	}
	else
	{
		id->philo->perished = 1;
		logging("died", id, 'd');
	}
}

void	sleeping(t_id *id)
{
	size_t	time;
	size_t	time_since;

	time = ft_get_time();
	time_since = time - id->philo->start_time;
	// yellow();
	logging("is sleeping", id, 's');
	// reset();
	ft_msleep(id->philo->time_to_sleep);
}
