/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:24:27 by cgray             #+#    #+#             */
/*   Updated: 2024/03/18 17:48:43 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* returns 1 while alive, 0 if dead */
int	perished(t_id *id, size_t time_from_meal)
{
	size_t	time;

	time = ft_get_time();
	if (id->philo->perished == 1)
		return (0);
	if (time_from_meal >= id->philo->time_to_die)
	{
		printf("%zu %d died.\n", time - id->philo->start_time, id->id);
		id->philo->perished = 1;
		return (0);
	}
	return (1);
}

void	*routine(void *philo_id)
{
	int		first;
	int		time;
	size_t	last_ate;
	t_id	*id;

	id = (t_id *)philo_id;
	first = 1;
	while (!id->philo->perished)
	{
		if (id->id % 2 == 0 && first == 1) // even guys sleep first
		{
			sleeping(id);
			first = 0;
		}
		if (perished(id, last_ate))
			return (NULL);
		eating(id);
		last_ate = ft_get_time();
		sleeping(id);
		if (perished(id, last_ate))
			return (NULL);
		thinking(id);
		time = ft_get_time();
		if (id->times_eaten == id->philo->times_eaten
			|| !perished(id, time - last_ate))
			return (NULL);
	}
	return (NULL);
}

void	pickup_fork(t_id *id, int fork_position)
{
	size_t	time;

	pthread_mutex_lock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 0;
	time = ft_get_time() - id->philo->start_time;
	printf("%zu %d has taken a fork.\n", time, id->id + 1);
}

void	putdown_fork(t_id *id, int fork_position)
{
	size_t	time;

	pthread_mutex_unlock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 1;
	time = ft_get_time() - id->philo->start_time;
	printf("%zu %d has set fork down.\n", time, id->id + 1);
}

void	eating(t_id *id)
{
	size_t	time;

	pickup_fork(id, id->id);
	if (id->id == id->philo->num_philos - 1)
		pickup_fork(id, 1);
	else
		pickup_fork(id, id->id + 1);
	time = ft_get_time() - id->philo->start_time;
	printf("%zu %d is eating.\n", time, id->id + 1);
	ft_msleep(id->philo->time_to_eat);
	// id->philo->time_from_meal = 0;
	putdown_fork(id, id->id);
	if (id->id == id->philo->num_philos - 1)
		putdown_fork(id, 1);
	else
		putdown_fork(id, id->id + 1);
	id->times_eaten++;
}

void	thinking(t_id *id)
{
	size_t	time;
	size_t	time_since;

	time = ft_get_time();
	time_since = time - id->philo->start_time;
	printf("%zu %d is thinking\n", time_since, id->id + 1);
	// ft_msleep(id->philo->time_to_eat - id->philo->time_to_sleep);
}

void	sleeping(t_id *id)
{
	size_t	time;
	size_t	time_since;

	time = ft_get_time();
	time_since = time - id->philo->start_time;
	printf("%zu %d is sleeping\n", time_since, id->id + 1);
	ft_msleep(id->philo->time_to_sleep);
}

int	ft_msleep(size_t ms)
{
	size_t	start;

	start = ft_get_time();
	while ((ft_get_time() - start) < ms)
		usleep(500);
	return (0);
}

/* returns current time in ms */
size_t	ft_get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("gettimeofday error.\n");
		exit(EXIT_FAILURE);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

/* converts given string to a long
	digits in string can be proceeded by spaces/linefeeds,
	'-' or '+' changes sign of number.
	stops after the first non-digit after string digits */
long	ft_atol(const char *string)
{
	long	num;
	int		neg;

	num = 0;
	neg = 1;
	while (*string == ' ' || (*string >= 9 && *string <= 13))
		string++;
	if (*string == '-' || *string == '+')
	{
		if (*string == '-')
			neg = -1;
		string++;
	}
	while (*string >= '0' && *string <= '9')
	{
		num = num * 10 + *string - '0';
		string++;
	}
	return (num * neg);
}

void	philo_init(t_philo *philos, pthread_mutex_t **mutexes,
			t_id **philos_id)
{
	int	i;

	i = 0;
	while (i < philos->num_philos)
	{
		philos->forks[i] = 1;
		(*philos_id)[i].philo = philos;
		(*philos_id)[i].id = i;
		(*philos_id)[i].times_eaten = 0;
		(*philos_id)[i].mutexes = *mutexes;
		i++;
	}
}

/* Initialize and malloc t_philo and number of threads/mutexes */
void	mem_init(t_philo *philos, pthread_t **philos_threads,
			pthread_mutex_t **mutexes, t_id **philos_id)
{
	int	np;

	np = philos->num_philos;
	philos->start_time = ft_get_time();
	*philos_threads = malloc(np * sizeof(pthread_t));
	*mutexes = malloc(np * sizeof(int));
	philos->forks = malloc(np * sizeof(int));
	*philos_id = malloc(np * sizeof(t_id));
	philos->perished = 0;
}

void	arg_error(void)
{
	printf("Bad arguments.\n");
	exit(EXIT_FAILURE);
}

/* reads arguments and places them into struct, checking if valid input */
void	get_args(int ac, char **av, t_philo *philos)
{
	if (ac != 5 && ac != 6)
		arg_error();
	philos->num_philos = (int)ft_atol(av[1]);
	philos->time_to_die = ft_atol(av[2]);
	philos->time_to_eat = ft_atol(av[3]);
	philos->time_to_sleep = ft_atol(av[4]);
	if (philos->num_philos < 1 || philos->time_to_die < 0
		|| philos->time_to_eat < 0 || philos->time_to_sleep < 0)
		arg_error();
	if (ac == 6)
	{
		philos->times_eaten = (int)ft_atol(av[5]);
		if (philos->times_eaten < 0)
			arg_error();
	}
	else
		philos->times_eaten = -1;
}

int	main(int ac, char **av)
{
	t_philo			philos;
	t_id			*id;
	pthread_t		*philos_threads;
	pthread_mutex_t	*mutexes;
	int				i;


	mem_init(&philos, &philos_threads, &mutexes, &id);
	get_args(ac, av, &philos);
	philo_init(&philos, &mutexes, &id);
	i = 0;
	while (i < philos.num_philos)
	{
		pthread_mutex_init(&mutexes[i], NULL);
		pthread_create(philos_threads + i, NULL, &routine, &id[i]);
		i++;
	}
	i = 0;
	while (i < philos.num_philos)
		pthread_join(philos_threads[i++], NULL);
	i = 0;
	while (i < philos.num_philos)
		pthread_mutex_destroy(&mutexes[i++]);
}
