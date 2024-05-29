/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:58:47 by cgray             #+#    #+#             */
/*   Updated: 2024/05/29 16:45:10 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*                                             🧙3
                                         , - ~ ~ ~ - ,
                                     , '        🍝      ' ,
                                   ,                       ,
                                  ,     🍴2          🍴3    ,
                                 ,                           ,
                            🧙2  , 🍝                     🍝 ,  🧙4
                                 ,                           ,
                                  ,                         ,
                                   ,     🍴1        🍴0    ,
                                     ,          🍝      , '
                                       ' - , _ __ ,  '
                                               🧙1

	🧙1 grabs 🍴1 then 🍴0
	🧙2 grabs 🍴1 then 🍴2
	🧙3 grabs 🍴3 then 🍴2
	🧙4 grabs 🍴3 then 🍴0
*/

/* Philos try to grab same fork as their neighbor,
	avoiding deadlock trap where all philos pick up one fork
		first fork is philo pos + 1, unless last philo, where it is 0 pos
		second fork is philo pos
		UNLESS your position is even, then opposite

	Note: philo->pos starts from 1 while fork_pos starts from 0
 */
static void	assign_forks(t_philo *philo, t_fork *forks, int fork_pos)
{
	int	np;

	np = philo->global->nbr_of_philos;
	philo->first_fork = &forks[(fork_pos + 1) % np];
	philo->second_fork = &forks[fork_pos];
	if (philo->pos % 2 == 0)
	{
		philo->first_fork = &forks[fork_pos];
		philo->second_fork = &forks[(fork_pos + 1) % np];
	}
}

static void	init_philo(t_global *global)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < global->nbr_of_philos)
	{
		philo = global->philos + i;
		philo->pos = i + 1;
		philo->eaten_enough = false;
		philo->meals_count = 0;
		mutex(&philo->philo_mutex, INIT);
		philo->global = global;
		assign_forks(philo, global->forks, i);
	}
}

void	init_data(t_global *global)
{
	long	np;
	int		i;

	np = global->nbr_of_philos;
	global->stop_dinner = false;
	global->thread_sync = false;
	global->threads_running = 0;
	global->philos = cool_malloc(sizeof(t_philo) * np);
	global->forks = cool_malloc(sizeof(t_fork) * np);
	mutex(&global->global_mutex, INIT);
	mutex(&global->log_mutex, INIT);
	i = -1;
	while (++i < np)
	{
		mutex(&global->forks[i].fork, INIT);
		global->forks[i].fork_pos = i;
	}
	init_philo(global);
}
