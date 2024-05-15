/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:24:27 by cgray             #+#    #+#             */
/*   Updated: 2024/05/15 14:15:17 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* returns 0 while alive, 1 if dead
Function to check if enough time has past that a philo will die
if dead, call logging function*/
int	perished(t_id *id, size_t time_from_meal, size_t routine_time)
{
	size_t	ttd;

	ttd = id->philo->time_to_die;
	pthread_mutex_lock(&(id->philo->write_mutex));
	if (id->philo->perished == 1)
		return (pthread_mutex_unlock(&(id->philo->write_mutex)), 1);
	else if (time_from_meal > ttd)
	{
		id->philo->perished = 1;
		pthread_mutex_unlock(&(id->philo->write_mutex));
		logging("died", id, 'd');
		return (1);
	}
	else if (time_from_meal + routine_time > ttd)
	{
		id->philo->perished = 1;
		pthread_mutex_unlock(&(id->philo->write_mutex));
		ft_msleep((id->philo->time_to_die - time_from_meal) + 1);
		logging("died", id, 'd');
		return (1);
	}
	pthread_mutex_unlock(&(id->philo->write_mutex));
	return (0);
}

/* main routine for each philo thread.
Overview:
	-if even position, sleep first
	-eat and call store finish eating time
	-check if enough time to sleep without dying
	-sleep
	-check if enough time to think without dying
	-think
	-check if eaten enough or if dead */
void	*routine(void *philo_id)
{
	int		first;
	size_t	last_ate;
	t_id	*id;
	size_t	think_time;

	id = (t_id *)philo_id;
	first = 1;
	last_ate = ft_get_time();
	think_time = 2 * (id->philo->time_to_eat - id->philo->time_to_sleep);
	while (1)
	{
		if (first == 1 && first_sleep(id, &first, last_ate))
			return (NULL);
		eating(id, &last_ate);
		if (perished(id, ft_get_time() - last_ate, id->philo->time_to_sleep))
			return (NULL);
		sleeping(id, last_ate);
		if (perished(id, ft_get_time() - last_ate, think_time))
			return (NULL);
		thinking(id, last_ate);
		if (eaten_enough_or_die(id, last_ate))
			return (NULL);
	}
	return (NULL);
}

/* edge case where there is only one philo (and
in turn, only one fork.)
wait till die and log death. */
int	one_philo(t_philo *philo)
{
	char	*fork;
	char	*died;
	size_t	time;

	time = ft_get_time();
	fork = "has taken a fork";
	died = "died a lonely death";
	if (philo->num_philos == 1)
	{
		printf("%zu\t%d\t%s\n", ft_get_time() - time, 1, fork);
		ft_msleep(philo->time_to_die);
		printf(BRED"%zu\t%d\t%s\n"RESET, ft_get_time() - time, 1, died);
		return (1);
	}
	return (0);
}

/* Function to initialize, execute, and delete threads for each philo */
void	run_threads(t_id *id, pthread_t *philos_threads,
		pthread_mutex_t *mutexes)
{
	int	i;

	pthread_mutex_init(&id->philo->write_mutex, NULL);
	pthread_mutex_init(&id->write_mutex, NULL);
	i = 0;
	while (i < id->philo->num_philos)
		pthread_mutex_init(&mutexes[i++], NULL);
	i = 0;
	while (i < id->philo->num_philos)
	{
		pthread_create(philos_threads + i, NULL, &routine, &id[i]);
		usleep(10);
		i++;
	}
	i = 0;
	while (i < id->philo->num_philos)
		pthread_join(philos_threads[i++], NULL);
	i = 0;
	while (i < id->philo->num_philos)
		pthread_mutex_destroy(&mutexes[i++]);
}

/* Main function
	-get and check arguments -- store appropriately
	-init memory
	-run one philo edge case
	-call exec thread function
	-free memory */
int	main(int ac, char **av)
{
	t_philo			philos;
	t_id			*id;
	pthread_t		*philos_threads;
	pthread_mutex_t	*mutexes;
	pthread_mutex_t	write_mutex;

	get_args(ac, av, &philos);
	mem_init(&philos, &philos_threads, &mutexes, &id);
	philo_init(&philos, &mutexes, &id, &write_mutex);
	if (one_philo(&philos))
	{
		free_philos(&philos, &philos_threads, &mutexes, &id);
		return (0);
	}
	title(philos);
	run_threads(id, philos_threads, mutexes);
	free_philos(&philos, &philos_threads, &mutexes, &id);
}
