/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaei <smaei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:27:25 by smaei             #+#    #+#             */
/*   Updated: 2023/08/19 12:00:28 by smaei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	data_init(t_data *data, char **argv, pthread_mutex_t *fork_mutex)
{
	data->philo_count = philo_atoi(argv[1]);
	data->time_die = philo_atoi(argv[2]);
	data->time_eat = philo_atoi(argv[3]);
	data->time_sleep = philo_atoi(argv[4]);
	data->start_time = get_time();
	data->max_eat = -1;
	if (argv[5])
		data->max_eat = philo_atoi(argv[5]);
	data->fork_mutex = fork_mutex;
	data->repeat = true;
	pthread_mutex_init(&data->check, NULL);
	pthread_mutex_init(&data->print_message, NULL);
	pthread_mutex_init(&data->eat_mutex, NULL);
}

void	philo_init(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philo[i].id = i;
		philo[i].r_fork = i;
		philo[i].l_fork = (i + 1) % data->philo_count;
		philo[i].meal_count = 0;
		philo[i].data = data;
		philo[i].last_meal_time = get_time();
		pthread_mutex_init(&philo[i].meal, NULL);
		pthread_mutex_init(&data->fork_mutex[i++], NULL);
	}
}

t_philo	*init(char **argv)
{
	int				fork;
	pthread_mutex_t	*fork_mutex;
	t_philo			*philo;
	t_data			*data1;

	fork = philo_atoi(argv[1]);
	fork_mutex = malloc(sizeof(pthread_mutex_t) * fork);
	if (!fork_mutex)
		return (NULL);
	data1 = malloc(sizeof(t_data));
	if (!data1)
		return (NULL);
	data_init(data1, argv, fork_mutex);
	philo = malloc(sizeof(t_philo) * fork);
	if (!philo)
		return (NULL);
	philo_init(data1, philo);
	return (philo);
}
