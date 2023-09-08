/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaei <smaei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:28:47 by smaei             #+#    #+#             */
/*   Updated: 2023/08/19 15:20:38 by smaei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	get_repeat(t_data *data)
{
	bool	res;

	pthread_mutex_lock(&data->eat_mutex);
	res = data->repeat;
	pthread_mutex_unlock(&data->eat_mutex);
	return (res);
}

int	get_meal_count(t_philo *philo, int i)
{
	int	res;

	pthread_mutex_lock(&philo[i].meal);
	res = philo[i].meal_count;
	pthread_mutex_unlock(&philo[i].meal);
	return (res);
}

int	set_meal_count(t_philo *philo, int i)
{
	int res;

	pthread_mutex_lock(&philo[0].meal);
	philo[0].meal_count += i;
	res = philo[0].meal_count;
	pthread_mutex_unlock(&philo[0].meal);
	return (res);
}

long long	get_meal_time(t_philo *philo, int i)
{
	long long	res;

	pthread_mutex_lock(&philo[i].meal_time);
	res = philo[i].last_meal_time;
	pthread_mutex_unlock(&philo[i].meal_time);
	return (res);
}

void	set_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo[0].meal_time);
	philo[0].last_meal_time = get_time();
	pthread_mutex_unlock(&philo[0].meal_time);
}

