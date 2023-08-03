#include "philo.h"

long long get_time()
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL) == 0)
        return (tv.tv_usec / 1000 + tv.tv_sec * 1000);
    else
        return (-1);
}

void philo_print(char *msg, t_philo *philo, long long time)
{
    pthread_mutex_lock(&philo->data->print_message);
    if (philo->data->repeat)
    {
        if (time == -1)
            printf("%lld %d %s\n", get_time() - philo->data->start_time, philo->id, msg);
        else
            printf("%lld %d %s\n", time - philo->data->start_time, philo->id, msg);
    }
    pthread_mutex_unlock(&philo->data->print_message);
}

void philo_act(void *arg)
{
    t_philo *philo;

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
        while (philo->data->repeat && philo->data->max_eat != philo->meal_count)
        {
            pthread_mutex_lock(&philo->data->fork_mutex[philo->r_fork]);
            philo_print("has taken a fork", philo, -1);
            pthread_mutex_lock(&philo->data->fork_mutex[philo->l_fork]);
            philo_print("has taken a fork", philo, -1);
            pthread_mutex_lock(&philo->meal);
            philo->last_meal_time = get_time();
            philo->meal_count += 1;
            pthread_mutex_unlock(&philo->meal);
            philo_print("is eating", philo, philo->last_meal_time);
            usleep(philo->data->time_eat * 999);
            pthread_mutex_unlock(&philo->data->fork_mutex[philo->r_fork]);
            pthread_mutex_unlock(&philo->data->fork_mutex[philo->l_fork]);
            philo_print("is sleeping", philo, -1);
            usleep(philo->data->time_sleep * 999);
            philo_print("is thinking", philo, -1);
    }
    }
}

void data_init(t_data *data, char **argv, pthread_mutex_t *fork_mutex)
{
    data->philo_count = atoi(argv[1]);
    data->time_die = atoi(argv[2]);
    data->time_eat = atoi(argv[3]);
    data->time_sleep = atoi(argv[4]);
    data->start_time = get_time();
    data->max_eat = -1;
    if (argv[5])
        data->max_eat = atoi(argv[5]);
    data->fork_mutex = fork_mutex;
    data->repeat = true;
    pthread_mutex_init(&data->check, NULL);
    pthread_mutex_init(&data->print_message, NULL);
    pthread_mutex_init(&data->eat_mutex, NULL);
}

void philo_init(t_data *data, t_philo *philo)
{
    int i;

    i = 0;
    while (i < data->philo_count)
    {
        philo[i].id = i;
        philo[i].r_fork = i;
        philo[i].l_fork = (i + 1) % data->philo_count;
        philo[i].meal_count = 0;
        philo[i].data = data;
        philo[i].last_meal_time = get_time();
        pthread_mutex_init(&philo[i].meal, NULL);
        pthread_mutex_init(&data->fork_mutex[i++], NULL);
    }
}

t_philo *init(char **argv, t_data *data)
{
    int fork;
    pthread_mutex_t *fork_mutex;
    t_philo *philo;
    
    fork = atoi(argv[1]);
    fork_mutex = malloc(sizeof(pthread_mutex_t) * fork);
    if (!fork_mutex)
        return(NULL);
    data_init(data, argv, fork_mutex);
    philo = malloc(sizeof(t_philo) * fork);
    if (!philo)
        return (NULL);
    philo_init(data, philo);
    return (philo);
}

int main(int argc, char **argv)
{
    t_data data;
    t_philo *philo;
    int count;

    if (argc != 5 && argc != 6)
        return (1);
    philo = init(argv, &data);
    if (data.time_die < (data.time_eat + data.time_sleep))
        return (1);
    int i;
    i = 0;
    pthread_t thread_id[10];
    while (i < data.philo_count)
    {
        pthread_create(&thread_id[i], NULL, (void *)philo_act, (void *)&philo[i]);
        i++;
    }
    while (data.repeat && count != data.max_eat)
    {
        i = 0;
        pthread_mutex_lock(&philo[0].meal);
        count = philo[0].meal_count;
        pthread_mutex_unlock(&philo[0].meal);
        while (i < data.philo_count)
        {
            pthread_mutex_lock(&philo[i].meal);
            if (philo[i].meal_count < data.max_eat || data.max_eat == -1)
            {
                if (data.time_die < get_time() -  philo[i].last_meal_time)
                {
                    philo_print("died", philo, -1);
                    data.repeat = false;
                }
            }
            if (philo[i].meal_count < count)
                count = philo[i].meal_count;
            pthread_mutex_unlock(&philo[i].meal);
            i++;
        }
    }
    i = 0;
    while (i < data.philo_count)
    {
        pthread_join(thread_id[i++], NULL);
    }
}