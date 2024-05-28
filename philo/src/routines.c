/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:20:46 by cgray             #+#    #+#             */
/*   Updated: 2024/05/28 15:38:24 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *philo)
{
	mutex(&philo->first_fork->fork, LOCK);
	logging("has taken a fork", philo, 'f');
	mutex(&philo->second_fork->fork, LOCK);
	logging("has taken a fork", philo, 'f');
	set_long(&philo->philo_mutex, &philo->meals_count, get_time_ms());
	logging("is eating", philo, 'e');
	philo->meals_count++;
	p_delay(philo->global->time_to_eat, philo->global);
	mutex(&philo->first_fork->fork, UNLOCK);
	mutex(&philo->second_fork->fork, UNLOCK);
	if (philo->global->nbr_of_meals > 0
		&& philo->global->nbr_of_meals == philo->meals_count)
		set_bool(&philo->philo_mutex, &philo->eaten_enough, true);
}

void	thinking(t_philo *philo)
{
	logging("is thinking", philo, 't');
}

void	sleeping(t_philo *philo)
{
	logging("is sleeping", philo, 's');
	p_delay(philo->global->time_to_sleep, philo->global);
}
