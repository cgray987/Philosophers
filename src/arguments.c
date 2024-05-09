/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:42:26 by cgray             #+#    #+#             */
/*   Updated: 2024/05/08 17:06:19 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_init(t_philo *philos, pthread_mutex_t **mutexes,
			pthread_mutex_t *write_mutex, t_id **philos_id)
{
	int	i;

	i = 0;
	while (i < philos->num_philos)
	{
		philos->forks[i] = 1;
		// printf("forks[%d] = %d\n", i, philos->forks[i]);
		(*philos_id)[i].philo = philos;
		(*philos_id)[i].id = i;
		(*philos_id)[i].times_eaten = 0;
		(*philos_id)[i].mutexes = *mutexes;
		(*philos_id)[i].write_mutex = write_mutex;
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
	*mutexes = (pthread_mutex_t *)malloc(np * sizeof(pthread_mutex_t));
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
		philos->num_to_eat = (int)ft_atol(av[5]);
		if (philos->num_to_eat < 0)
			arg_error();
	}
	else
		philos->num_to_eat = -1;
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
