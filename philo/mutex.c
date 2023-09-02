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

int	get_meal_count(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo[0].meal);
	res = philo[0].meal_count;
	pthread_mutex_unlock(&philo[0].meal);
	return (res);
}
