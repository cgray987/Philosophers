/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:24:27 by cgray             #+#    #+#             */
/*   Updated: 2024/05/09 17:50:07 by cgray            ###   ########.fr       */
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
	if (id->philo->perished == 1)
	{
		// logging("died (maybe twice)", id, 'd');
		return (1);
	}
	else if (time_from_meal > ttd)
	{
		id->philo->perished = 1;
		logging("died", id, 'd');
		printf("%d\ttfm: %zu\n",id->id + 1, time_from_meal);
		return (1);
	}
	else if (time_from_meal + routine_time > ttd)
	{
		// printf("%zu %d going to die in %zu\n", ft_get_time() - id->philo->start_time, id->id + 1, ttd - time_from_meal);
		// printf("%zu %d going to die in %zu\n", ft_get_time() - id->philo->start_time, id->id + 1, ttd - time_from_meal);
		// ft_msleep(id->philo->time_to_die - (ft_get_time() - id->philo->start_time));
		id->philo->perished = 1;
		ft_msleep((id->philo->time_to_die - time_from_meal));
		logging("died", id, 'd');
		printf("coulnd't do routine\n");
		printf("%d\ttfm: %zu\n",id->id + 1, time_from_meal);
		return (1);
	}
	// printf("%d didn't die\n", id->id + 1);
	// printf("time from meal: %zu\n", time_from_meal);
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
	while (!id->philo->perished)
	{
		if (first == 1 && first_sleep(id, &first, last_ate))
			return (NULL);
		eating(id, &last_ate);
		// {
		// 	thinking(id);
		// 	eating(id, &last_ate);
		// }
		// last_ate = ft_get_time();
		// id->philo->time_from_meal = last_ate;
		if (perished(id, ft_get_time() - last_ate, id->philo->time_to_sleep))
			return (NULL);
		sleeping(id, last_ate);
		if (perished(id, ft_get_time() - last_ate, think_time))
			return (NULL);
		thinking(id, last_ate);
		if (eaten_enough_or_die(id, last_ate))
			return (NULL);
		// printf("%d made through routine\n", id->id + 1);
	}
	return (NULL);
}

/* edge case where there is only one philo (and
in turn, only one fork.)
wait till die and log death. */
int	one_philo(t_id *id)
{
	if (id->philo->num_philos == 1)
	{
		logging("has taken a fork", id, 'f');
		ft_msleep(id->philo->time_to_die);
		logging("died a lonely death", id, 'd');
		return (1);
	}
	return (0);
}

/* Function to initialize, execute, and delete threads for each philo */
void	run_threads(t_id *id, pthread_t *philos_threads,
		pthread_mutex_t *mutexes)
{
	int	i;

	i = 0;
	while (i < id->philo->num_philos)
	{
		pthread_mutex_init(&mutexes[i], NULL);
		pthread_create(philos_threads + i, NULL, &routine, &id[i]);
		usleep(10);
		i++;
	}
	i = 0;
	while (i < id->philo->num_philos)
	{
		pthread_join(philos_threads[i], NULL);
		i++;
	}
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
	philo_init(&philos, &mutexes, &write_mutex, &id);
	title(philos);
	if (one_philo(id))
	{
		free_philos(&philos, &philos_threads, &mutexes, &id);
		return (0);
	}
	run_threads(id, philos_threads, mutexes);
	free_philos(&philos, &philos_threads, &mutexes, &id);
}
