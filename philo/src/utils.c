/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:40:03 by cgray             #+#    #+#             */
/*   Updated: 2024/05/28 16:26:56 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	logging(const char *str, t_philo *philo, char flag)
{
	size_t	time;

	if (philo->eaten_enough) //might need mutex
		return;
	mutex(&philo->global->log_mutex, LOCK);
	time = get_time_ms() - philo->global->start_time;
	if (!dinner_done(philo->global))
	{
		if (flag == 'd')
			printf(BRED"%-ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else if (flag == 'e')
			printf(GRN"%-ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else if (flag == 's')
			printf(YEL"%-ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else if (flag == 't')
			printf(BLU"%-ld\t%3d\t%s"RESET"\n", time, philo->pos, str);
		else
			printf("%-ld\t%3d\t%s\n", time, philo->pos, str);
	}
	mutex(&philo->global->log_mutex, UNLOCK);
}

void	display_error(const char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}
