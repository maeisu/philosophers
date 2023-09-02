/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaei <smaei@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:25:36 by smaei             #+#    #+#             */
/*   Updated: 2023/09/02 20:13:21 by smaei            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_isdigit(char n)
{
	if ('0' <= n && n <= '9')
		return (true);
	return (false);
}

int	philo_atoi(char *num)
{
	int			i;
	long long	res;
	long long	number;

	if (!num)
		return (-1);
	i = 0;
	res = 0;
	while (num[i] != '\0')
	{
		if (ft_isdigit(num[i]) == false)
			return (-1);
		res *= 10;
		number = num[i] - '0';
		res += number;
		i++;
	}
	if (res > INT_MAX)
		return (-1);
	return ((int)res);
}

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == 0)
		return (tv.tv_usec / 1000 + tv.tv_sec * 1000);
	else
		return (-1);
}

void	ft_usleep(int wait_time)
{
	long long	now_time;

	now_time = get_time();
	while (1)
	{
		if (get_time() - now_time >= wait_time)
			break ;
		usleep(1000);
	}
}

void	philo_print(char *msg, t_philo *philo, long long time)
{
	if (get_repeat(philo->data) || (msg && msg[0] == 'd'))
	{
		pthread_mutex_lock(&philo->data->print_message);
		if (time == -1)
			printf("%lld %d %s\n",
				get_time() - philo->data->start_time, philo->id, msg);
		else
			printf("%lld %d %s\n",
				time - philo->data->start_time, philo->id, msg);
		pthread_mutex_unlock(&philo->data->print_message);
	}
}
