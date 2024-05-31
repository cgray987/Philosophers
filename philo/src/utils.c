/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:40:03 by cgray             #+#    #+#             */
/*   Updated: 2024/05/30 16:53:24 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* writes to terminal using mutex
flags are just for colors
d - died
e - is eating
s - is sleeping
t - is thinking */
void	logging(const char *str, t_philo *philo, char flag)
{
	long	time;

	if (get_bool(&philo->philo_mutex, &philo->eaten_enough))
		return ;
	mutex(&philo->global->log_mutex, LOCK);
	time = get_time_ms() - philo->global->start_time;
	if (!dinner_done(philo->global))
	{
		if (flag == 'd')
			printf(BRED"%-5ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else if (flag == 'e')
			printf(GRN"%-5ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else if (flag == 's')
			printf(YEL"%-5ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else if (flag == 't')
			printf(BLU"%-5ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else
			printf("%-5ld\t%3d\t%s\n", time, philo->pos, str);
	}
	mutex(&philo->global->log_mutex, UNLOCK);
}

/* if no string given, prints formatted input error msg
otherwise, print string and exit failure */
void	display_error(const char *str)
{
	if (!str)
	{
		printf(BWHT"Bad arguments.\n"RESET
			"Usage: ./philo A B C D [E]\n"
			"Where \tA = number of philosophers\n"
			"\tB = time to die\n"
			"\tC = time to eat\n"
			"\tD = time to sleep\n"
			YEL"\t[E] = number of times each philosopher must eat "
			"(optional)\n"RESET
			"All arguments must be positive integers.\n");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}

/* called in main function, iterates thru global destroying each philo mutex
frees all global mutexes and fork/philo arrays*/
void	free_philos(t_global *global)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < global->nbr_of_philos)
	{
		philo = global->philos + i;
		mutex(&philo->philo_mutex, DESTROY);
	}
	mutex(&global->log_mutex, DESTROY);
	mutex(&global->global_mutex, DESTROY);
	free(global->forks);
	free(global->philos);
}
