/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:43:32 by cgray             #+#    #+#             */
/*   Updated: 2024/05/06 16:56:13 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philos(t_philo *philos, pthread_t **philos_threads,
			pthread_mutex_t **mutexes, t_id **id)
{
	free(*philos_threads);
	free(*mutexes);
	free(*id);
	free(philos->forks);
}

int	ft_msleep(size_t ms)
{
	size_t	start;

	start = ft_get_time();
	while ((ft_get_time() - start) < ms)
		usleep(50);
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

void	title(t_philo philos)
{
	char	*title[7];
	int		i;

	title[0] = " _______  __   __  ___  ___      _______  _______";
	title[1] = "|       ||  | |  ||   ||   |    |       ||       |";
	title[2] = "|    _  ||  |_|  ||   ||   |    |   _   ||  _____|";
	title[3] = "|   |_| ||       ||   ||   |    |  | |  || |_____";
	title[4] = "|    ___||       ||   ||   |___ |  |_|  ||_____  |";
	title[5] = "|   |    |   _   ||   ||       ||       | _____| |";
	title[6] = "|___|    |__| |__||___||_______||_______||_______|";
	i = 0;
	while (i < 7)
		printf(RED"%s\n", title[i++]);
	printf(RESET"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("|\tNumber of philos: %d\t\t|\n", philos.num_philos);
	// red();
	printf("|\tTime to die: %zu ms\t\t|\n", philos.time_to_die);
	// green();
	printf("|\tTime to eat: %zu ms\t\t|\n", philos.time_to_eat);
	// yellow();
	printf("|\tTime to sleep: %zu ms\t\t|\n", philos.time_to_sleep);
	// reset();
	if (philos.num_to_eat != -1)
		printf("|\tNumber of times to eat: %d\t|\n", philos.num_to_eat);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	printf("time\tphilo\taction\n");
}
