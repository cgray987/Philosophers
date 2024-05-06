/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 12:13:19 by cgray             #+#    #+#             */
/*   Updated: 2024/05/06 16:55:50 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Read values from arguments
	-av[1]: number of philos
	-av[2]: time to die
	-av[3]: time to eat
	-av[4]: time to sleep
	-optional
		-av[5]: num of times philos must eat

		-atoi each number into struct
		-each num > 0

init memory and struct

create mutex lock and thread for each philo
	-run philo routine, breaking if any philo dies
		-will eat, sleep, and think
			-eat
				-pick up right fork - mutex lock
				-pick up left fork - mutex lock
					-print both
				-eat for input sec
				-put down both forks by unlocking mutexes
			-sleep for input sec, print sleep msg
			-display think msg
	-monitor
		-checks until philo dies or ate all needed meals
		-breaks loop if either case

destroy mutexes/free memory
 */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h> //mem fns
# include <stdio.h> //printf
# include <unistd.h> //usleep,
# include <sys/time.h> //gettimeofday
# include <sys/wait.h>
# include <pthread.h> //pthread, mutex
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
#define BRED "\e[1;31m"
# define RESET "\e[0m"
typedef struct s_philo
{
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	size_t			time_from_meal;
	size_t			start_time;
	int				num_to_eat;
	int				num_philos;
	int				*forks;
	int				perished;
}			t_philo;

typedef struct s_id
{
	t_philo			*philo;
	int				id;
	int				times_eaten;
	pthread_mutex_t	*mutexes;
	pthread_mutex_t	*write_mutex;
}			t_id;

void	logging(char *str, t_id *id, char flag);
int		perished(t_id *id, size_t time_from_meal, size_t routine_time);
void	*routine(void *id);
void	pickup_fork(t_id *id, int fork_position);
void	putdown_fork(t_id *id, int fork_position);
void	eating(t_id *id);
void	thinking(t_id *id);
void	sleeping(t_id *id);
int		ft_msleep(size_t ms);
size_t	ft_get_time(void);
long	ft_atol(const char *string);
void	title(t_philo philos);
void	philo_init(t_philo *philos, pthread_mutex_t **mutexes,
			pthread_mutex_t *write_mutex, t_id **philos_id);
void	mem_init(t_philo *philos, pthread_t **philos_threads,
			pthread_mutex_t **mutexes, t_id **philos_id);
void	arg_error(void);
void	get_args(int ac, char **av, t_philo *philos);
void	free_philos(t_philo *philos, pthread_t **philos_threads,
			pthread_mutex_t **mutexes, t_id **id);

#endif
