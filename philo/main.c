/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaei <smaei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:42:49 by smaei             #+#    #+#             */
/*   Updated: 2023/09/02 20:10:20 by smaei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_exit(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->philo_count)
		pthread_join(philo[i++].thread_id, NULL);
	pthread_mutex_destroy(&philo->data->eat_mutex);
	pthread_mutex_destroy(&philo->data->check);
	pthread_mutex_destroy(&philo->data->print_message);
	i = 0;
	while (i < philo->data->philo_count)
		pthread_mutex_destroy(&philo->data->fork_mutex[i++]);
	pthread_mutex_destroy(&philo->meal);
	pthread_mutex_destroy(&philo->meal_time);
	free(philo->data->fork_mutex);
	free(philo->data);
	free(philo);
}

int get(t_philo *philo)
{
	int res;

	pthread_mutex_lock(&philo->data->check);
	res = philo->data->finish;
	pthread_mutex_unlock(&philo->data->check);
	return (res);
}

bool	monitor(t_philo *philo, int i)
{
	int count;

	count = INT_MAX;
	while (i < philo->data->philo_count)
	{
		if (philo->data->max_eat != -1 && get(philo) >= philo->data->philo_count)
		{
			pthread_mutex_lock(&philo[i].data->eat_mutex);
			philo[i].data->repeat = false;
			pthread_mutex_unlock(&philo[i].data->eat_mutex);
			return (false);
		}
		if (philo->data->time_die < get_time() - get_meal_time(philo, i))
		{
			pthread_mutex_lock(&philo[i].data->eat_mutex);
			philo[i].data->repeat = false;
			pthread_mutex_unlock(&philo[i].data->eat_mutex);
			philo_print("died", &philo[i], -1);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	input_error(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		printf("Input error\n");
		return (true);
	}
	i = 1;
	while (i < argc)
	{
		if (philo_atoi(argv[i++]) == -1)
		{
			printf("Input error\n");
			return (true);
		}
	}
	return (false);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;
	int		i;

	if (input_error(argc, argv))
		return (1);
	philo = init(argv);
	if (!philo)
		return (1);
	i = 0;
	while (i < philo->data->philo_count)
	{
		pthread_create(&philo[i].thread_id, NULL,
			(void *)philo_act, (void *)&philo[i]);
		i++;
	}
	while (monitor(philo, 0));
	philo_exit(philo);
}
