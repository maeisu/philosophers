/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_act.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaei <smaei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:41:08 by smaei             #+#    #+#             */
/*   Updated: 2023/09/02 12:57:35 by smaei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	loop_act(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork_mutex[philo->r_fork]);
	philo_print("has taken a fork", philo, -1);
	pthread_mutex_lock(&philo->data->fork_mutex[philo->l_fork]);
	philo_print("has taken a fork", philo, -1);
	set_meal_time(philo);
	set_meal_count(philo, 1);
	philo_print("is eating", philo, get_meal_time(philo, 0));
	ft_usleep(philo->data->time_eat);
	pthread_mutex_unlock(&philo->data->fork_mutex[philo->r_fork]);
	pthread_mutex_unlock(&philo->data->fork_mutex[philo->l_fork]);
	philo_print("is sleeping", philo, -1);
	ft_usleep(philo->data->time_sleep);
	philo_print("is thinking", philo, -1);
}

void	philo_act(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		usleep(200);
	if (philo->data->philo_count == 1)
	{
		pthread_mutex_lock(&philo->data->fork_mutex[philo->r_fork]);
		philo_print("has taken a fork", philo, -1);
		pthread_mutex_unlock(&philo->data->fork_mutex[philo->r_fork]);
	}
	else
	{
		while (get_repeat(philo->data)
			&& philo->data->max_eat != get_meal_count(philo, 0))
			loop_act(philo);
	}
}
