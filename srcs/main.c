#include "philo.h"
#include <sys/time.h>

//valgrind --tool=helgrind
// fsanitize=thread
void print_time(int time, int philnb, char *str)
{
   printf("%dms |Philo%d| :%s", time, philnb, str);
}

long int	actual_time(void)
{
	struct timeval	tv;
	int				err;

	err = gettimeofday(&tv, NULL);
	if (err != 0)
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int init()
{

    return (0);
}

int    give_fork(t_philo *philo, int nb)
{
    int i;

    if (nb == 1)
    {
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
    }
    else
    {
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
    }
    if (i)
        print(philo->id, "prend une fork!!", philo);
    return (i);
}

void    *routine(void *arg)
{
    t_philo     *philo;
    int         i;

    philo = (t_philo *)arg;

    print(philo->id, "test init", philo);
    philo->last_eat = actual_time();
    while (1)
    {
        i = 0;
        if (philo->next_fork)
        {
	        print(philo->id, "cherche une fork!!", philo);
            while (!give_fork(philo, 1) && !check_death(philo))
            {
                
            }
            while (!give_fork(philo, 2) && !check_death(philo))
			{
                
			}
            if (!check_death(philo))
            {
                //print(philo->id, "veut manger", philo);
                a_table(philo);
                time_sleep(philo);
            }
        }
    }
    return (NULL);
}

int is_dead(t_env *env)
{
    int i;

    i = 0;
    while (i < env->count)
    {
        if (env->philo[i].dead == 1)
            return (1);
        i++;
    }
    return (0);
}

int main(int ac, char **av)
{
    t_env *env;
    int i;

    i = 0;
    if (ac < 5 || ac > 6)
    {
        printf("USAGE: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return (0); // Invalide number of args
    }
    env = malloc(sizeof(t_env));
    env->arg = malloc(sizeof(t_arg));
    ///init_philo()
    env->arg->t_eat = ft_atoi(av[3]);
    env->arg->t_sleep = ft_atoi(av[4]);
    env->arg->t_die = ft_atoi(av[2]);
    env->count = ft_atoi(av[1]);
    env->philo = malloc(sizeof(t_philo) * env->count);
    printf("t_eat %d t_sleep %d t_die count %d count %d\n", env->arg->t_eat, env->arg->t_sleep, env->arg->t_die, env->count);
    
    env->arg->time = actual_time();
    pthread_mutex_lock(&(env->arg->dead.mutex));
    env->arg->dead.data = 0;
    pthread_mutex_unlock(&(env->arg->dead.mutex));
    while (i < env->count)
    {
        env->philo[i].dead = 0;
        pthread_mutex_init(&(env->philo[i].fork.mutex), NULL);
        env->philo[i].fork.data = 1 ;
        env->philo[i].arg = env->arg;
        env->philo[i].last_eat = actual_time();
        env->philo[i].id = i;
        i++;
       // err = pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
    }
    i = -1;
    while (++i < env->count)
    {
        if (i == env->count - 1)
        {
            //printf("le philo %d peut prendre sa fourchette et celle de philo %d\n", i, 0);
            env->philo[i].next_fork = &env->philo[0].fork;
        }
        else if (env->count == 1)
        {
          //  printf("le philo %d est seul et n aura pas deux fourchettes\n", i);
            env->philo[i].next_fork = NULL;
        }
        else
        {
            env->philo[i].next_fork = &env->philo[i + 1].fork;
            //printf("le philo %d peut prendre sa fourchette et celle de philo %d\n", i, i + 1);
        }
        pthread_create(&(env->philo[i].thread), NULL, routine, &(env->philo[i]));
    }

    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
    //test(philo->c_eat, philo->count);
    while (1)
    {
        long int time;

        i = 0;
        while (i < env->count && env->philo[i].thread)
        {
            time = actual_time();
            if (time - env->philo[i].last_eat > env->arg->t_die)
            {
                	print(env->philo[i].id, "MORT DE DECES :\n", &env->philo[i]);
                    printf("dernier repas : %ld\n", env->philo[i].last_eat - env->philo->arg->time);
                exit(EXIT_FAILURE);
            }
            i++;
        }
    }
    free(env->philo);
    free(env);
    return (0);
}

/*
int main(int ac, char **av)
{
    (void)ac;
    get_atoyed(av[1], 3);
}*/