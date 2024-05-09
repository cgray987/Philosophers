/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:01:58 by cgray             #+#    #+#             */
/*   Updated: 2024/05/09 17:46:42 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	logging(char *str, t_id *id, char flag)
{
	size_t	time;

	// pthread_mutex_lock((id->write_mutex));
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
	// pthread_mutex_unlock((id->write_mutex));
}

int	first_sleep(t_id *id, int *first, size_t last_ate)
{
	if (*first == 1 && (id->id) % 2 == 0)
	{
		*first = 0;
		logging("is thinking", id, 't');
		ft_msleep(id->philo->time_to_eat); //ft_msleep(id->philo->time_to_sleep);
		// usleep(10);
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
