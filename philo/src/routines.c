/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:20:46 by cgray             #+#    #+#             */
/*   Updated: 2024/05/29 15:43:53 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *philo)
{
	mutex(&philo->first_fork->fork, LOCK);
	logging("has taken a fork", philo, 'f');
	mutex(&philo->second_fork->fork, LOCK);
	logging("has taken a fork", philo, 'f');
	set_long(&philo->philo_mutex, &philo->meal_timestamp, get_time_ms());
	logging("is eating", philo, 'e');
	philo->meals_count++;
	printf("%d meals count: %ld\n", philo->pos, philo->meals_count);
	p_delay(philo->global->time_to_eat, philo->global);
	// ft_msleep(philo->global->time_to_eat);
	if (philo->global->nbr_of_meals > 0 && philo->global->nbr_of_meals == philo->meals_count)
		set_bool(&philo->philo_mutex, &philo->eaten_enough, true);
	mutex(&philo->first_fork->fork, UNLOCK);
	mutex(&philo->second_fork->fork, UNLOCK);
}

void	thinking(t_philo *philo, bool first)
{
	long	t_think;

	if (!first)
		logging("is thinking", philo, 't');
	if (philo->global->nbr_of_meals % 2 == 0)
		return ;
	t_think = (philo->global->time_to_eat * 2) - philo->global->time_to_sleep;
	if (t_think < 0)
		t_think = 0;
	p_delay(t_think *.5, philo->global);
}

void	sleeping(t_philo *philo)
{
	logging("is sleeping", philo, 's');
	p_delay(philo->global->time_to_sleep, philo->global);
	// ft_msleep(philo->global->time_to_sleep);
}
