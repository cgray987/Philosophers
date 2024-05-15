/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:01:58 by cgray             #+#    #+#             */
/*   Updated: 2024/05/15 14:15:09 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* ensures forks are picked up/put down in the same order */
void	handle_forks(t_id *id, int left, int right,
			void (*fork_handler)(t_id *, int))
{
	if (left > right)
	{
		fork_handler(id, right);
		fork_handler(id, left);
	}
	else
	{
		fork_handler(id, left);
		fork_handler(id, right);
	}
}

void	drop_forks(t_id *id)
{
	int	left;
	int	right;

	right = id->id;
	if (id->id == id->philo->num_philos - 1)
		left = 0;
	else
		left = id->id + 1;
	if (!id->philo->forks[left])
		pthread_mutex_unlock(&(id->mutexes[left]));
	if (!id->philo->forks[right])
		pthread_mutex_unlock(&(id->mutexes[right]));
}

void	logging(char *str, t_id *id, char flag)
{
	size_t	time;

	pthread_mutex_lock(&(id->write_mutex));
	time = ft_get_time() - id->philo->start_time;
	if (flag == 'd')
		printf(BRED"%5zu\t%3d\t%s\n"RESET, time, id->id + 1, str);
	else if (flag == 'e')
		printf(GRN"%5zu\t%3d\t%s\n"RESET, time, id->id + 1, str);
	else if (flag == 's')
		printf(YEL"%5zu\t%3d\t%s\n"RESET, time, id->id + 1, str);
	else if (flag == 't')
		printf(BLU"%5zu\t%3d\t%s\n"RESET, time, id->id + 1, str);
	else
		printf("%5zu\t%3d\t%s\n", time, id->id + 1, str);
	pthread_mutex_unlock(&(id->write_mutex));
}

int	first_sleep(t_id *id, int *first, size_t last_ate)
{
	if (*first == 1 && (id->id) % 2 == 0)
	{
		*first = 0;
		logging("is thinking", id, 't');
		ft_msleep(id->philo->time_to_eat);
	}
	if (perished(id, ft_get_time() - last_ate, 0))
		return (1);
	return (0);
}

int	eaten_enough_or_die(t_id *id, size_t last_ate)
{
	if (id->times_eaten == id->philo->num_to_eat
		|| perished(id, ft_get_time() - last_ate, 0))
		return (1);
	return (0);
}
