/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:34:49 by cgray             #+#    #+#             */
/*   Updated: 2024/05/28 16:23:37 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	./philo 5 800 200 200 [5]
 */
int	main(int ac, char **av)
{
	t_global	global;

	if (ac == 5 || ac == 6)
	{
		get_input(&global, ac, av);

		init_data(&global);

		start_sim(&global);

		// free_philos(&global);
	}
	else
	{
		display_error(INPUT_ERROR);
	}
}
