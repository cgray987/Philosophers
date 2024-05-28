/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:24:27 by cgray             #+#    #+#             */
/*   Updated: 2024/05/27 16:51:50 by cgray            ###   ########.fr       */
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
	// if (id->keep_running == 1)
	// {
		// pthread_mutex_unlock(&(id->philo->write_mutex));
	// 	return (1);
	// }
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
	// printf("time from meal: %zu ", time_from_meal);
	// logging("did not die", id, 'd');
	pthread_mutex_unlock(&(id->philo->write_mutex));
	return (0);
}

int	continue_routine(t_id *id)
{
	// pthread_mutex_lock((id)->monitor);
	if (id->keep_running == 1)
	{
		pthread_mutex_unlock((id)->monitor);
		return (0);
	}
	// else
	// 	printf("id: %d keep_running: %d\n", id->id + 1, id->keep_running);
	// pthread_mutex_unlock((id)->monitor);
	return (1);
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
	t_id	*id;

	id = (t_id *)philo_id;
	first = 1;
	while (continue_routine(id) == 1)
	{
		if (first == 1 && first_sleep(id, &first))
			return (NULL);
		eating(id);
		sleeping(id);
		thinking(id);
		if (id->keep_running == 1)
			return (NULL);
		// printf("%zu %d time from meal: %zu\n",ft_get_time() - id->philo->start_time, id->id + 1,ft_get_time() - id->last_meal_time);
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

int	has_philo_died(t_id *id)
{
	if (id == NULL || id->philo == NULL)
		return (1);
	pthread_mutex_lock((id)->monitor);
	if (ft_get_time() - id->last_meal_time > (size_t)id->philo->time_to_die)
	{
		// printf("monitor thread found dead philo\n");
		pthread_mutex_unlock((id)->monitor);
		return (1);
	}
	pthread_mutex_unlock((id)->monitor);
	return (0);
}

/* iterate through each id for every philo
check if dead,
stop everything if someone dead */
void	*monitor_routine(void *v_id)
{
	t_id	*id;
	int		num_philos;
	int		i;

	i = 0;
	id = (t_id *)v_id;
	num_philos = id->philo->num_philos;
	// ft_msleep(5);
	if (id->philo == NULL)
	{
		printf("philo is NULL\n");
		return (NULL);
	}
	ft_msleep(5);
	while (1)
	{
		// printf("monitor thread id: %d\n", id[i].id + 1);
		if (has_philo_died(&id[i]))
		{
			id->keep_running = 1;
			logging("died", &id[i], 'd');
			i = 0;
			while (i < num_philos)
			{
				pthread_mutex_lock(id->monitor);
				id[i].keep_running = 1;
				pthread_mutex_unlock(id->monitor);
				i++;
			}
			return (NULL);
		}
		// else
			// printf("monitor thread %d didn't die:\n", id[i].id + 1);
		if (i == num_philos - 1)
		{
			i = 0;
		}
		else
			i++;
	}
	return (NULL);
}

/* Function to initialize, execute, and delete threads for each philo */
void	run_threads(t_id *id, pthread_t *philos_threads,
		pthread_mutex_t *mutexes)
{
	int				i;
	pthread_mutex_t	log_mutex;
	pthread_t		monitor_thread;
	pthread_mutex_t	monitor_mutex;

	// monitor_thread = malloc(sizeof(pthread_t));
	pthread_mutex_init(&log_mutex, NULL);
	i = 0;
	while (i < id->philo->num_philos)
	{
		id[i].log_mutex = &log_mutex;
		id[i].monitor = &monitor_mutex;
		pthread_mutex_init(&mutexes[i++], NULL);
		pthread_mutex_init(&id->philo->write_mutex + i, NULL);
	}
	i = 0;
	while (i < id->philo->num_philos)
	{
		pthread_create(philos_threads + i, NULL, &routine, &id[i]);
		usleep(10);
		i++;
	}
	ft_msleep(5);
	pthread_create(&monitor_thread, NULL, &monitor_routine, id);
	i = 0;
	pthread_join(monitor_thread, NULL);
	while (i < id->philo->num_philos)
		pthread_join(philos_threads[i++], NULL);
	i = 0;
	while (i < id->philo->num_philos)
	{
		pthread_mutex_destroy(&mutexes[i]);
		pthread_mutex_destroy(&id->philo->write_mutex + i);
		pthread_mutex_destroy(id[i].monitor);
		pthread_mutex_destroy(id[i].log_mutex);
		i++;
	}
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
	title(philos);
	if (one_philo(&philos))
	{
		free_philos(&philos, &philos_threads, &mutexes, &id);
		return (0);
	}
	run_threads(id, philos_threads, mutexes);
	free_philos(&philos, &philos_threads, &mutexes, &id);
}
