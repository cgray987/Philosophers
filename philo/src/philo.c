/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:34:49 by cgray             #+#    #+#             */
/*   Updated: 2024/05/29 17:16:02 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* norminette split */
void	subtitle(t_global *global)
{
	printf(RESET"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("|\tNumber of philos: %ld\t\t|\n", global->nbr_of_philos);
	printf("|\tTime to die: "RED"%ld ms"RESET"\t\t|\n", global->time_to_die);
	printf("|\tTime to eat: "GRN"%ld ms"RESET"\t\t|\n", global->time_to_eat);
	printf("|\tTime to sleep: "YEL"%ld ms"RESET"\t\t|\n"RESET,
		global->time_to_sleep);
	if (global->nbr_of_meals != -1)
		printf(BWHT"|\tNumber of times to eat: "GRN"%ld"RESET"\t|\n",
			global->nbr_of_meals);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	printf(BWHT"time\tphilo\taction"RESET"\n");
}

/* a silly title */
void	title(t_global *global)
{
	char	*title;

	title = " ██▓███  ██░ ██ ██▓██▓    ▒█████   ██████\n"
		"▓██░  ██▓██░ ██▓██▓██▒   ▒██▒  ██▒██    ▒\n"
		"▓██░ ██▓▒██▀▀██▒██▒██░   ▒██░  ██░ ▓██▄\n"
		"▒██▄█▓▒ ░▓█ ░██░██▒██░   ▒██   ██░ ▒   ██▒\n"
		"▒██▒ ░  ░▓█▒░██░██░██████░ ████▓▒▒██████▒▒\n"
		"▒▓▒░ ░  ░▒ ░░▒░░▓ ░ ▒░▓  ░ ▒░▒░▒░▒ ▒▓▒ ▒ ░\n"
		"░▒ ░     ▒ ░▒░ ░▒ ░ ░ ▒  ░ ░ ▒ ▒░░ ░▒  ░ ░\n"
		"░░       ░  ░░ ░▒ ░ ░ ░  ░ ░ ░ ▒ ░  ░  ░\n"
		"         ░  ░  ░░     ░  ░   ░ ░       ░\n";
	printf(RED"%s"RESET"\n", title);
	subtitle(global);
}

/*
	./philo 5 800 200 200 [5]
	./philo #_of_philos ttd tte tts [#_of_meals]
 */
int	main(int ac, char **av)
{
	t_global	global;

	if (ac == 5 || ac == 6)
	{
		get_input(&global, ac, av);
		init_data(&global);
		title(&global);
		if (global.nbr_of_philos == 1)
			one_philo(&global.philos[0]);
		else
			start_sim(&global);
		free_philos(&global);
	}
	else
	{
		display_error(NULL);
	}
}
