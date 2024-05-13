/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:44:12 by cgray             #+#    #+#             */
/*   Updated: 2024/05/13 15:59:35 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* locks and picks up fork */
void	pickup_fork(t_id *id, int fork_position)
{
	pthread_mutex_lock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 0;
	logging("has taken a fork", id, 'f');
}

/* unlocks and puts fork down */
void	putdown_fork(t_id *id, int fork_position)
{
	pthread_mutex_unlock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 1;
	// logging("has put down a fork", id, 'f');
}

/* if available, grabs left and right forks.
-- if last philo, left fork is fork 1
-- else left fork is id + 1
- call log to eat
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
	// printf("test: %d l%d r%d\n", id->id + 1, id->philo->forks[left], id->philo->forks[right]);
	// if (id->philo->forks[left] && id->philo->forks[right])
	{
		pickup_fork(id, left);
		pickup_fork(id, right);
	}
	// else
	// {
		// logging("no forks available\n", id, 'x');
		// printf("\t\t%d l%d r%d\n", id->id + 1, id->philo->forks[left], id->philo->forks[right]);
		// thinking(id);
		// return (1);
	// }
	logging("is eating", id, 'e');
	// id->philo->time_from_meal = ft_get_time();
	*last_ate = ft_get_time();
	if (id->philo->time_to_eat > id->philo->time_to_die)
		return (0);
	ft_msleep(id->philo->time_to_eat);
	putdown_fork(id, left);
	putdown_fork(id, right);
	id->times_eaten++;
	return (0);
}

/* think time is twice eat time - sleep time
I think the else condition could be removed. */
void	thinking(t_id *id, size_t last_ate)
{
	size_t	time;
	long	think_time;

	time = ft_get_time();
	think_time = 2 * (id->philo->time_to_eat - id->philo->time_to_sleep);
	if (think_time > 0)
	{
		logging("is thinking", id, 't');
		if (perished(id, time - last_ate, think_time))
			return ;
		// printf("%d time from meal: %zu\n",id->id + 1, time - id->philo->time_from_meal);
		ft_msleep(think_time);
		// ft_msleep(id->philo->time_to_eat);
		// else
		// {
		// 	printf("think death\n");
		// 	printf("%zu\n", time - id->philo->time_from_meal);
		// 	id->philo->perished = 1;
		// 	logging("died", id, 'd');
		// }
	}
}

void	sleeping(t_id *id, size_t last_ate)
{
	size_t	time;

	time = ft_get_time();
	logging("is sleeping", id, 's');
	if (perished(id, time - last_ate, id->philo->time_to_sleep))
	{
		return ;
	}
	// printf("%d time from meal: %zu\n",id->id + 1, time - id->philo->time_from_meal);
	ft_msleep(id->philo->time_to_sleep);
	// else
	// {
	// 	printf("sleep death\n");
	// 	printf("%zu\n", time - id->philo->time_from_meal);
	// 	id->philo->perished = 1;
	// 	logging("died", id, 'd');
	// }
}
