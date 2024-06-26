/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:20:46 by cgray             #+#    #+#             */
/*   Updated: 2024/05/30 17:08:23 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
lock forks and log,
set time philo started eating,
increment meals count,
unlock forks
use to print philo has eaten enough:
		printf(BCYN"%-6ld\t%3d\thas eaten %ld times\n"RESET,
			get_time_ms() - philo->global->start_time,
			philo->pos, philo->meals_count);
 */
void	eating(t_philo *philo)
{
	mutex(&philo->first_fork->fork, LOCK);
	logging("has taken a fork", philo, 'f');
	mutex(&philo->second_fork->fork, LOCK);
	logging("has taken a fork", philo, 'f');
	set_long(&philo->philo_mutex, &philo->meal_timestamp, get_time_ms());
	philo->meals_count++;
	logging("is eating", philo, 'e');
	p_delay(philo->global->time_to_eat, philo->global);
	if (philo->global->nbr_of_meals > 0
		&& philo->global->nbr_of_meals == philo->meals_count)
	{
		set_bool(&philo->philo_mutex, &philo->eaten_enough, true);
	}
	mutex(&philo->first_fork->fork, UNLOCK);
	mutex(&philo->second_fork->fork, UNLOCK);
}

/* first flag used only for un sync routine to not print
'is thinking' before regular routines.
if odd group, use small mandatory think time to make sure
philos don't immediately eat twice in a row */
void	thinking(t_philo *philo, bool first)
{
	long	t_think;

	if (!first)
		logging("is thinking", philo, 't');
	if (philo->global->nbr_of_philos % 2 == 0)
		return ;
	t_think = (philo->global->time_to_eat * 2) - philo->global->time_to_sleep;
	if (t_think < 0)
		t_think = 1;
	p_delay(t_think * .5, philo->global);
}

/* 😴 */
void	sleeping(t_philo *philo)
{
	logging("is sleeping", philo, 's');
	p_delay(philo->global->time_to_sleep, philo->global);
}
