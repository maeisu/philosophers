/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaei <smaei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 12:13:04 by smaei             #+#    #+#             */
/*   Updated: 2023/09/02 20:07:18 by smaei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <time.h>
# include <limits.h>

typedef enum s_status{
	SLEEP,
	EAT,
	THINK,
}				t_status;

typedef struct s_data
{
	int				time_eat;
	int				time_sleep;
	int				time_die;
	int				max_eat;
	int				philo_count;
	long long		start_time;
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	print_message;
	pthread_mutex_t	check;
	bool			repeat;
}			t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	int				r_fork;
	int				l_fork;
	int				meal_count;
	long long		last_meal_time;
	t_data			*data;
	pthread_mutex_t	meal;
}				t_philo;

void		data_init(t_data *data, char **argv, pthread_mutex_t *fork_mutex);
void		philo_init(t_data *data, t_philo *philo);
t_philo		*init(char **argv);
bool		get_repeat(t_data *data);
int			get_meal_count(t_philo *philo);
void		loop_act(t_philo *philo);
void		philo_act(void *arg);
bool		ft_isdigit(char n);
int			philo_atoi(char *num);
long long	get_time(void);
void		ft_usleep(int wait_time);
void		philo_print(char *msg, t_philo *philo, long long time);

#endif