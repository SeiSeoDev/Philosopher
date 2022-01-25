/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 14:30:49 by dasanter          #+#    #+#             */
/*   Updated: 2022/01/12 13:25:56 by dasanter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int		print(int id, char *msg, t_philo *philo)
{
	long int	time;
	int ret;

	ret = 0;
	while (!ret && !check_death(philo))
	{
		pthread_mutex_lock(&(philo->arg->print.mutex));
    	ret = philo->arg->print.data == 1;
		if (ret)
			philo->arg->print.data = 0;
    	pthread_mutex_unlock(&(philo->arg->print.mutex));
	}
	time = actual_time();
	if (time < 0)
		return (0);
	if (!check_death(philo))
	{	printf("%ld : philo %d %s\n", time - philo->arg->time, id + 1, msg);
	pthread_mutex_lock(&(philo->arg->print.mutex));
	philo->arg->print.data = 1;
	pthread_mutex_unlock(&(philo->arg->print.mutex));
	}
	return (1);
}

int		check_death(t_philo *philo)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&(philo->arg->dead.mutex));
	if (philo->arg->dead.data == 1)
	{
		ret = 1;
	}
	pthread_mutex_unlock(&(philo->arg->dead.mutex));
	return (ret);
}

int		time_sleep(t_philo *philo)
{
	long int time;
	time = actual_time();
	print(philo->id, "start sleep !", philo);
	while ((actual_time() - time < philo->arg->t_sleep) && !check_death(philo))
	{
		if (check_death(philo))
			printf("dodo\n");
		usleep(1);
	}
	return (1);
}

int		check_full(t_philo *philo)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&(philo->full.mutex));
	if (philo->full.data == 1)
		ret = 1;
	pthread_mutex_unlock(&(philo->full.mutex));
	return (ret);
}

int		a_table(t_philo *philo)
{
	long int time;

	int test;

	test = 0;
	pthread_mutex_lock(&(philo->last_eat.mutex));
	philo->last_eat.data = actual_time();
    pthread_mutex_unlock(&(philo->last_eat.mutex));
	time = actual_time();
	print(philo->id, "is eating", philo);
	while ((actual_time() - time) < (philo->arg->t_eat))
	{
		//printf("%ld / %d\n",(actual_time() - time), philo->arg->t_eat);
		test++;
	}
	if (!check_death(philo))
	{
		pthread_mutex_lock(&(philo->fork.mutex));
		philo->fork.data = 1;
		pthread_mutex_unlock(&(philo->fork.mutex));
		pthread_mutex_lock(&(philo->next_fork->mutex));
		philo->next_fork->data = 1;
		pthread_mutex_unlock(&(philo->next_fork->mutex));
	}
	return (1);
}