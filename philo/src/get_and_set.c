/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_and_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:11:01 by cgray             #+#    #+#             */
/*   Updated: 2024/05/30 16:57:51 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* sets value of ptr to bool using a mutex */
void	set_bool(t_mutex *mtx, bool *dest, bool value)
{
	mutex(mtx, LOCK);
	*dest = value;
	mutex(mtx, UNLOCK);
}

/* reads a ptr to a bool using a mutex */
bool	get_bool(t_mutex *mtx, bool *value)
{
	bool	ret;

	mutex(mtx, LOCK);
	ret = *value;
	mutex(mtx, UNLOCK);
	return (ret);
}

/* sets value of ptr to bool using a mutex */
void	set_long(t_mutex *mtx, long *dest, long value)
{
	mutex(mtx, LOCK);
	*dest = value;
	mutex(mtx, UNLOCK);
}

/* reads a ptr to a long using a mutex */
long	get_long(t_mutex *mtx, long *value)
{
	long	ret;

	mutex(mtx, LOCK);
	ret = *value;
	mutex(mtx, UNLOCK);
	return (ret);
}

void	increase_long(t_mutex *mtx, long *value)
{
	mutex(mtx, LOCK);
	(*value)++;
	mutex(mtx, UNLOCK);
}
