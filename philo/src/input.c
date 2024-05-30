/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:46:29 by cgray             #+#    #+#             */
/*   Updated: 2024/05/30 15:29:56 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	return (0);
}

static int	check_av_for_non_digit(int ac, char **av)
{
	int	i;

	i = 0;
	while (--ac >= 1)
	{
		while (av[ac][i])
		{
			if (!ft_isdigit(av[ac][i]))
			{
				return (1);
			}
			i++;
		}
	}
	return (0);
}

/* converts given string to a long
	digits in string can be proceeded by spaces/linefeeds,
	'-' or '+' changes sign of number.
	stops after the first non-digit after string digits */
static long	ft_atol(const char *string)
{
	long	num;
	int		neg;

	num = 0;
	neg = 1;
	while (*string == ' ' || (*string >= 9 && *string <= 13))
		string++;
	if (*string == '-' || *string == '+')
	{
		if (*string == '-')
			neg = -1;
		string++;
	}
	while (*string >= '0' && *string <= '9')
	{
		num = num * 10 + *string - '0';
		string++;
	}
	return (num * neg);
}

/*
	check for non digits in input
	assign input data to global struct
		./philo #_of_philos ttd tte tts [#_of_meals]
 */
void	get_input(t_global *global, int ac, char **av)
{
	if (check_av_for_non_digit(ac, av))
		display_error(NULL);
	global->nbr_of_philos = ft_atol(av[1]);
	global->time_to_die = ft_atol(av[2]) * 1e3;
	global->time_to_eat = ft_atol(av[3]) * 1e3;
	global->time_to_sleep = ft_atol(av[4])* 1e3;
	if (global->nbr_of_philos < 1 || global->time_to_die < 0
		|| global->time_to_eat < 0 || global->time_to_sleep < 0)
		display_error(NULL);
	if (ac == 6)
	{
		global->nbr_of_meals = ft_atol(av[5]);
		if (global->nbr_of_meals < 0)
			display_error(NULL);
	}
	else
		global->nbr_of_meals = -1;
}
