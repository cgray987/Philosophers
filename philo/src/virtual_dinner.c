/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtual_dinner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:57:53 by cgray             #+#    #+#             */
/*   Updated: 2024/05/29 18:04:44 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	perished(t_philo *philo)
{
	long	time_since_meal;
	long	ttd;

	if (get_bool(&philo->philo_mutex, &philo->eaten_enough))
		return (false);
	ttd = philo->global->time_to_die;
	time_since_meal = get_time_ms()
		- get_long(&philo->philo_mutex, &philo->meal_timestamp);
	if (time_since_meal > ttd)
		return (true);
	return (false);
}

static void	*organ_farmer(void *v_global)
{
	t_global	*global;
	int			i;

	global = (t_global *)v_global;
	while (!all_threads_running(&global->global_mutex,
			&global->threads_running, global->nbr_of_philos))
		;
	while (!dinner_done(global))
	{
		i = -1;
		while (++i < global->nbr_of_philos)
		{
			if (perished(global->philos + i))
			{
				logging("has died", global->philos + i, 'd');
				set_bool(&global->global_mutex, &global->stop_dinner, true);
			}
		}
	}
	return (NULL);
}

/* wait for all threads to get started
increase number of threads when each philo starts dinner routine

go thru eat/sleep/think routines if not dead or full */
static void	*dinner_routine(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	wait_for_thread_sync(philo->global);
	set_long(&philo->philo_mutex, &philo->meal_timestamp, get_time_ms());
	increase_long(&philo->global->global_mutex,
		&philo->global->threads_running);
	un_sync(philo);
	while (!dinner_done(philo->global))
	{
		if (get_bool(&philo->philo_mutex, &philo->eaten_enough))
			break ;
		eating(philo);
		sleeping(philo);
		thinking(philo, false);
	}
	return (NULL);
}

void	*one_philo(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	set_long(&philo->global->global_mutex,
		&philo->global->start_time, get_time_ms());
	logging("has taken a fork", philo, 'f');
	p_delay(philo->global->time_to_die, philo->global);
	logging("has died", philo, 'd');
	return (NULL);
}

void	start_sim(t_global *global)
{
	int	i;

	i = -1;
	if (global->nbr_of_meals == 0)
		return ;
	else
	{
		while (++i < global->nbr_of_philos)
		{
			thread(&global->philos[i].philo_thread_id,
				dinner_routine, &global->philos[i], CREATE);
			usleep(100);
		}
	}
	thread(&global->organ_farmer, organ_farmer, global, CREATE);
	set_long(&global->global_mutex, &global->start_time, get_time_ms());
	set_bool(&global->global_mutex, &global->thread_sync, true);
	i = -1;
	while (++i < global->nbr_of_philos)
		thread(&global->philos[i].philo_thread_id, NULL, NULL, JOIN);
	set_bool(&global->global_mutex, &global->stop_dinner, true);
	usleep(100);
	thread(&global->organ_farmer, NULL, NULL, JOIN);
}
