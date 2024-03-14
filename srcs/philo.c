/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:24:27 by cgray             #+#    #+#             */
/*   Updated: 2024/03/14 16:55:42 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* returns 1 while alive, 0 if dead */
int	perished(t_id *id, int time_from_meal)
{
	int	time;

	time = ft_get_time();
	if (id->philo->perished == 1)
		return (0);
	if (time_from_meal >= id->philo->time_to_die)
	{
		printf("%u %d has died.\n", time - id->philo->start_time, id->id);
		id->philo->perished = 1;
		return (0);
	}
	return (1);
}

void	*routine(t_id *id)
{
	int	first;

	first = 1;
	while (!perished(id->philo->perished, id->philo->time_from_meal))
	{
		if (id->id % 2 == 0 && first == 1) // =even guys sleep first
		{
			sleeping(id);
			first = 0;
		}
		
	}
}

void	pickup_fork(t_id *id, int fork_position)
{
	int	time;

	pthread_mutex_lock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 0;
	time = ft_get_time() - id->philo->start_time;
	printf("%u %d has taken a fork.\n", time, id->id + 1);
}

void	putdown_fork(t_id *id, int fork_position)
{
	int	time;

	pthread_mutex_unlock(&(id->mutexes[fork_position]));
	id->philo->forks[fork_position] = 1;
	time = ft_get_time() - id->philo->start_time;
	printf("%u %d has set fork down.\n", time, id->id + 1);
}

void	eating(t_id *id)
{
	int	time;

	pickup_fork(id, id->id);
	if (id->id == id->philo->num_philos - 1)
		pickup_fork(id, 1);
	else
		pickup_fork(id, id->id + 1);
	time = ft_get_time() - id->philo->start_time;
	printf("%u %d is eating.\n", time, id->id + 1);
	ft_msleep(id->philo->time_to_eat);
	putdown_fork(id, id->id);
	if (id->id == id->philo->num_philos - 1)
		putdown_fork(id, 1);
	else
		putdown_fork(id, id->id + 1);
	id->philo->times_eaten++;
}

void	thinking(t_id *id)
{
	size_t	time;
	size_t	time_since;

	time = ft_get_time();
	time_since = time - id->philo->start_time;
	printf("%u %d is thinking\n", time_since, id->id + 1);
	//ft_msleep(id->philo->time_to_eat - id->philo->time_to_sleep);
}

void	sleeping(t_id *id)
{
	size_t	time;
	size_t	time_since;

	time = ft_get_time();
	time_since = time - id->philo->start_time;
	printf("%u %d is sleeping\n", time_since, id->id + 1);
	ft_msleep(id->philo->time_to_sleep);
}

int	ft_msleep(size_t ms)
{
	size_t	start;

	start = ft_get_time();
	while ((ft_get_time - start) < ms)
		usleep(500);
	return (0);
}

/* returns current time in ms */
int	ft_get_time(void)
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
		(*philos_id)[i].t_eat = 0;
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
	*philos_threads = malloc(np * sizeof(t_id));
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
	t_philo	philos;

	get_args(ac, av, &philos);
}
