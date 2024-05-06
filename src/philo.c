/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:24:27 by cgray             #+#    #+#             */
/*   Updated: 2024/05/06 16:54:27 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	logging(char *str, t_id *id, char flag)
{
	size_t	time;

	pthread_mutex_lock((id->write_mutex));
	time = ft_get_time() - id->philo->start_time;
	if (flag == 'd')
		printf(RED"%zu\t%d\t%s\n"RESET, time, id->id + 1, str);
	else if (flag == 'e')
		printf(GRN"%zu\t%d\t%s\n"RESET, time, id->id + 1, str);
	else if (flag == 's')
		printf(YEL"%zu\t%d\t%s\n"RESET, time, id->id + 1, str);
	else if (flag == 't')
		printf(BLU"%zu\t%d\t%s\n"RESET, time, id->id + 1, str);
	else
		printf("%zu\t%d\t%s\n", time, id->id + 1, str);
	pthread_mutex_unlock((id->write_mutex));
}

/* returns 0 while alive, 1 if dead */
int	perished(t_id *id, size_t time_from_meal, size_t routine_time)
{
	size_t	time;

	time = ft_get_time();
	if (id->philo->perished == 1)
		return (1);
	if (time_from_meal >= (size_t)id->philo->time_to_die)
	{
		// red();
		id->philo->perished = 1;
		logging("died", id, 'd');
		// reset();
		return (1);
	}
		if (time_from_meal + routine_time >= (size_t)id->philo->time_to_die)
	{
		// red();
		ft_msleep(id->philo->time_to_die - (ft_get_time() - id->philo->start_time));
		id->philo->perished = 1;
		logging("died", id, 'd');
		// reset();
		return (1);
	}
	return (0);
}

void	*routine(void *philo_id)
{
	int		first;
	size_t	time;
	size_t	last_ate;
	t_id	*id;
	size_t	think_time;

	id = (t_id *)philo_id;
	first = 1;
	last_ate = ft_get_time();
	time = ft_get_time();
	think_time = 2 * id->philo->time_to_eat - id->philo->time_to_sleep;
	while (!id->philo->perished)
	{
		if (id->id % 2 == 0 && first == 1) // even guys sleep first
		{
			sleeping(id);
			first = 0;
		}
		if (perished(id, ft_get_time() - last_ate, 0))
			return (NULL);
		eating(id);
		last_ate = ft_get_time();
		if (perished(id, ft_get_time() - last_ate, id->philo->time_to_sleep))
			return (NULL);
		sleeping(id);
		if (perished(id, ft_get_time() - last_ate, think_time))
			return (NULL);
		// if ((id->philo->time_to_die - (ft_get_time() - last_ate + think_time)) < 0)
		// {
		// 	ft_msleep(id->philo->time_to_die - (ft_get_time() - id->philo->start_time));
		// 	return (NULL); //ft_msleep(id->philo->time_to_die - ft_get_time() - id->philo->start_time)
		// }
		thinking(id);
		if (id->times_eaten == id->philo->num_to_eat
			|| perished(id, ft_get_time() - last_ate, 0))
		{
			// printf("\n%d has eaten %d times.\n", id->id + 1, id->times_eaten);
			return (NULL);
		}
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo			philos;
	t_id			*id;
	pthread_t		*philos_threads;
	pthread_mutex_t	*mutexes;
	pthread_mutex_t	write_mutex;
	int				i;


	get_args(ac, av, &philos);
	mem_init(&philos, &philos_threads, &mutexes, &id);
	philo_init(&philos, &mutexes, &write_mutex, &id);
	title(philos);
	if (philos.num_philos == 1)
	{
		printf("%d %d has taken a fork\n", 0, 1);
		ft_msleep(philos.time_to_die);
		// red();
		printf("%zu %d died\n", philos.time_to_die, 1);
		// reset();
		free_philos(&philos, &philos_threads, &mutexes, &id);
		return (0);
	}
	i = 0;
	while (i < philos.num_philos)
	{
		pthread_mutex_init(&mutexes[i], NULL);
		pthread_create(philos_threads + i, NULL, &routine, &id[i]);
		usleep(1);
		i++;
	}
	i = 0;
	while (i < philos.num_philos)
	{
		pthread_join(philos_threads[i], NULL);
		i++;
	}
	i = 0;
	while (i < philos.num_philos)
		pthread_mutex_destroy(&mutexes[i++]);
	free_philos(&philos, &philos_threads, &mutexes, &id);

}
