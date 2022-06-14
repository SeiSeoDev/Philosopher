/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:54:45 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/07 17:31:03 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_full(t_philo *philo)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&(philo->full.mutex));
	if (philo->full.data == 1)
		ret = 1;
	pthread_mutex_unlock(&(philo->full.mutex));
	return (ret);
}

int	check_all_full(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		if (check_full(&env->philo[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	check_death(t_philo *philo)
{
	int	ret;

	ret = -1;
	while (ret == -1)
	{
		pthread_mutex_lock(&(philo->arg->dead.mutex));
		if (philo->arg->dead.data == 1)
		{
			ret = 1;
			philo->arg->dead.data = 1;
		}
		else
			ret = 0;
		pthread_mutex_unlock(&(philo->arg->dead.mutex));
	}
	return (ret);
}
