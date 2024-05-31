/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:23:03 by cgray             #+#    #+#             */
/*   Updated: 2024/05/30 17:09:42 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* used for running sim/monitor thread */
bool	dinner_done(t_global *global)
{
	return (get_bool(&global->global_mutex, &global->stop_dinner));
}

/* waits until all threads have started by spinlock
	until thread_sync flag is true

	note, no longer used because helgrind doesn't like spinlocks
*/
void	wait_for_thread_sync(t_global *global)
{
	while (!get_bool(&global->global_mutex, &global->thread_sync))
		;
}
