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

void	monitor(int count, t_philo *philo)
{
	int	i;
	int get_count;

	i = 0;
	while (i < philo->data->philo_count)
	{
		get_count = get_meal_count(philo, i);
		if (get_count < philo->data->max_eat
			|| philo->data->max_eat == -1)
		{
			if (philo->data->time_die < get_time() - get_meal_time(philo, i))
			{
				pthread_mutex_lock(&philo[i].data->eat_mutex);
				philo[i].data->repeat = false;
				pthread_mutex_unlock(&philo[i].data->eat_mutex);
				philo_print("died", &philo[i], -1);
				break ;
			}
		}
		if (get_count < count)
			count = get_meal_count(philo, i);
		i++;
	}
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
	int		count;
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
	while (get_repeat(philo->data) && count != philo->data->max_eat)
	{
		count = get_meal_count(philo, 0);
		monitor(count, philo);
	}
	philo_exit(philo);
}
