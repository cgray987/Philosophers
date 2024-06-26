/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by cgray             #+#    #+#             */
/*   Updated: 2024/05/30 16:59:54 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> //printf
# include <stdlib.h> //malloc/free
# include <unistd.h> //usleep
# include <stdbool.h> //true/false
# include <pthread.h> //mutexes/threads
# include <sys/time.h> //gettimeofday
# include <limits.h> //INTMAX
# include <errno.h> //mutex/thread error codes

# define RED "\e[0;31m" //pretty colors
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define BRED "\e[1;31m"
# define BWHT "\e[1;37m"
# define RESET "\e[0m"

//STRUCTS

typedef pthread_mutex_t	t_mutex; //rename for conciseness
typedef struct s_philo	t_philo;

typedef enum e_thread_codes //used for mutex/thread error handling
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_thread_codes;

typedef struct s_fork
{
	t_mutex		fork;
	int			fork_pos;
}				t_fork;

typedef struct s_global
{
	long		nbr_of_philos;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		nbr_of_meals;
	long		start_time;
	bool		stop_dinner;
	bool		thread_sync;
	long		threads_running;
	pthread_t	organ_farmer;
	t_mutex		global_mutex;
	t_mutex		log_mutex;
	t_fork		*forks;		//array of forks
	t_philo		*philos;	//array of philos
}				t_global;

typedef struct s_philo
{
	int			pos;
	long		meals_count;
	bool		eaten_enough;
	long		meal_timestamp;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	philo_thread_id;
	t_mutex		philo_mutex;
	t_global	*global;	//pointer to global/input data
}				t_philo;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~virtual_dinner.c~~~~~~~~~~~~~~~~~~~~~~~ */
void	*one_philo(void *v_philo);
void	start_sim(t_global *global);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~routines.c~~~~~~~~~~~~~~~~~~~~~~~ */
void	eating(t_philo *philo);
void	thinking(t_philo *philo, bool first);
void	sleeping(t_philo *philo);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~get_and_set.c~~~~~~~~~~~~~~~~~~~~~~~ */
void	set_bool(t_mutex *mtx, bool *dest, bool value);
bool	get_bool(t_mutex *mtx, bool *value);
void	set_long(t_mutex *mtx, long *dest, long value);
long	get_long(t_mutex *mtx, long *value);
void	increase_long(t_mutex *mtx, long *value);
bool	dinner_done(t_global *global);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~sim_check.c~~~~~~~~~~~~~~~~~~~~~~~~~~ */
bool	all_threads_running(t_mutex *mtx, long *threads, long num_of_philos);
void	wait_for_thread_sync(t_global *global);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~sync.c~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	p_delay(long wait_us, t_global *global);
size_t	get_time_ms(void);
void	un_sync(t_philo *philo);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~wrappers.c~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	*cool_malloc(size_t bytes);
void	mutex(t_mutex *mutex, t_thread_codes code);
void	thread(pthread_t *thread, void *(*routine)(void *),
			void *global, t_thread_codes code);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~init.c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	init_data(t_global *global);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~input.c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	get_input(t_global *global, int ac, char **av);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~utils.c~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void	logging(const char *str, t_philo *philo, char flag);
void	display_error(const char *str);
void	free_philos(t_global *global);

#endif
