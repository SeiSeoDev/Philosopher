/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:34:49 by dasanter          #+#    #+#             */
/*   Updated: 2022/02/07 17:52:50 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	pair_one(t_philo *philo)
{
	int	i;

	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&(philo->fork.mutex));
		i = philo->fork.data == 1;
		if (i)
			philo->fork.data = 0;
		pthread_mutex_unlock(&(philo->fork.mutex));
	}
	else
	{
		pthread_mutex_lock(&(philo->next_fork->mutex));
		i = philo->next_fork->data == 1;
		if (i)
			philo->next_fork->data = 0;
		pthread_mutex_unlock(&(philo->next_fork->mutex));
	}
	return (i);
}

int	not_pair_one(t_philo *philo)
{
	int	i;

	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&(philo->fork.mutex));
		i = philo->fork.data == 1;
		if (i)
			philo->fork.data = 0;
		pthread_mutex_unlock(&(philo->fork.mutex));
	}
	else
	{
		pthread_mutex_lock(&(philo->next_fork->mutex));
		i = philo->next_fork->data == 1;
		if (i)
			philo->next_fork->data = 0;
		pthread_mutex_unlock(&(philo->next_fork->mutex));
	}
	return (i);
}
