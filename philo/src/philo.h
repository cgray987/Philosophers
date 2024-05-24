/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 12:13:19 by cgray             #+#    #+#             */
/*   Updated: 2024/05/24 20:47:11 by cgray            ###   ########.fr       */
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
# include <pthread.h> //pthread, mutex
# define RED "\e[0;31m" //pretty colors
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define BRED "\e[1;31m"
# define BWHT "\e[1;37m"
# define RESET "\e[0m"

/* Struct for individual philosopher info
write_mutex is used for checking/changing perished flag and logging*/
typedef struct s_philo
{
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_from_meal;
	long			start_time;
	int				num_to_eat;
	int				num_philos;
	int				*forks;
	int				perished;
	pthread_mutex_t	write_mutex;
}			t_philo;

/* Struct containing all philos and mutexes
write_mutex is used for checking/changing times_eaten*/
typedef struct s_id
{
	t_philo			*philo;
	int				id;
	int				keep_running;
	int				times_eaten;
	pthread_mutex_t	*mutexes;
	pthread_mutex_t	*log_mutex;
}			t_id;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~PHILO.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int		perished(t_id *id, size_t time_from_meal, size_t routine_time);
void	*routine(void *id);
void	run_threads(t_id *id, pthread_t *philos_threads,
			pthread_mutex_t *mutexes);
int		one_philo(t_philo *philo);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ROUTINES.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	pickup_fork(t_id *id, int fork_position);
void	putdown_fork(t_id *id, int fork_position);
int		eating(t_id *id, size_t *last_ate);
void	thinking(t_id *id, size_t last_ate);
void	sleeping(t_id *id, size_t last_ate);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ROUTINE_UTILS.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	handle_forks(t_id *id, int left, int right,
			void (*fork_handler)(t_id *, int));
void	drop_forks(t_id *id);
void	logging(char *str, t_id *id, char flag);
int		first_sleep(t_id *id, int *first, size_t last_ate);
int		eaten_enough_or_die(t_id *id, size_t last_ate);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ARGUMENTS.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	philo_init(t_philo *philos, pthread_mutex_t **mutexes,
			t_id **philos_id, pthread_mutex_t *write_mutex);
void	mem_init(t_philo *philos, pthread_t **philos_threads,
			pthread_mutex_t **mutexes, t_id **philos_id);
void	arg_error(void);
void	get_args(int ac, char **av, t_philo *philos);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~ARG_UTILS.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int		ft_isdigit(int c);
int		check_av_for_non_digit(int ac, char **av);
long	ft_atol(const char *string);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~UTILS.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	free_philos(t_philo *philos, pthread_t **philos_threads,
			pthread_mutex_t **mutexes, t_id **id);
int		ft_msleep(size_t ms);
size_t	ft_get_time(void);
void	title(t_philo philos);
void	subtitle(t_philo philos);

#endif
