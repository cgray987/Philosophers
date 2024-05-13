/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:43:32 by cgray             #+#    #+#             */
/*   Updated: 2024/05/13 15:33:29 by cgray            ###   ########.fr       */
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

void	title(t_philo philos)
{
	char	*title2[9];
	int		i;

	title2[0] = " ██▓███  ██░ ██ ██▓██▓    ▒█████   ██████";
	title2[1] = "▓██░  ██▓██░ ██▓██▓██▒   ▒██▒  ██▒██    ▒";
	title2[2] = "▓██░ ██▓▒██▀▀██▒██▒██░   ▒██░  ██░ ▓██▄";
	title2[3] = "▒██▄█▓▒ ░▓█ ░██░██▒██░   ▒██   ██░ ▒   ██▒";
	title2[4] = "▒██▒ ░  ░▓█▒░██░██░██████░ ████▓▒▒██████▒▒";
	title2[5] = "▒▓▒░ ░  ░▒ ░░▒░░▓ ░ ▒░▓  ░ ▒░▒░▒░▒ ▒▓▒ ▒ ░";
	title2[6] = "░▒ ░     ▒ ░▒░ ░▒ ░ ░ ▒  ░ ░ ▒ ▒░░ ░▒  ░ ░";
	title2[7] = "░░       ░  ░░ ░▒ ░ ░ ░  ░ ░ ░ ▒ ░  ░  ░";
	title2[8] = "         ░  ░  ░░     ░  ░   ░ ░       ░";
	i = 0;
	while (i < 9)
		printf(RED"%s\n", title2[i++]);
	subtitle(philos);
}

void	subtitle(t_philo philos)
{
	printf(RESET"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("|\tNumber of philos: %d\t\t|\n", philos.num_philos);
	printf("|\tTime to die: "RED"%zu ms"RESET"\t\t|\n", philos.time_to_die);
	printf("|\tTime to eat: "GRN"%zu ms"RESET"\t\t|\n", philos.time_to_eat);
	printf("|\tTime to sleep: "YEL"%zu ms"RESET"\t\t|\n"RESET,
		philos.time_to_sleep);
	if (philos.num_to_eat != -1)
		printf("|\tNumber of times to eat: "GRN"%d"RESET"\t|\n",
			philos.num_to_eat);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	printf(BWHT"time\tphilo\taction\n"RESET);
}

/*
	title[0] = " _______  __   __  ___  ___      _______  _______";
	title[1] = "|       ||  | |  ||   ||   |    |       ||       |";
	title[2] = "|    _  ||  |_|  ||   ||   |    |   _   ||  _____|";
	title[3] = "|   |_| ||       ||   ||   |    |  | |  || |_____";
	title[4] = "|    ___||       ||   ||   |___ |  |_|  ||_____  |";
	title[5] = "|   |    |   _   ||   ||       ||       | _____| |";
	title[6] = "|___|    |__| |__||___||_______||_______||_______|"; */
