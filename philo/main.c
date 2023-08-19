#include "philo.h"

bool ft_isdigit(char n)
{
    if ('0' <= n && n <= '9')
        return (true);
    return (false);
}

int philo_atoi(char *num)
{
    int i;
    long long res;
    long long number;

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
    return ((int)res);
}

long long get_time()
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL) == 0)
        return (tv.tv_usec / 1000 + tv.tv_sec * 1000);
    else
        return (-1);
}

void ft_usleep(int wait_time)
{
    long long now_time;

    now_time = get_time();
    while(1)
    {
        if (get_time() - now_time >= wait_time)
            break;
        usleep(1000);
    }
}

bool get_repeat(t_data *data)
{
    bool res;

    pthread_mutex_lock(&data->eat_mutex);
    res = data->repeat;
    pthread_mutex_unlock(&data->eat_mutex);
    return (res);
}

int get_meal_count(t_philo philo)
{
    int res;

    pthread_mutex_lock(&philo.meal);
    res = philo.meal_count;
    pthread_mutex_unlock(&philo.meal);
    return (res);
}

void philo_print(char *msg, t_philo *philo, long long time)
{
    if (get_repeat(philo->data))
    {
        pthread_mutex_lock(&philo->data->print_message);
        if (time == -1)
            printf("%lld %d %s\n", get_time() - philo->data->start_time, philo->id, msg);
        else
            printf("%lld %d %s\n", time - philo->data->start_time, philo->id, msg);
        pthread_mutex_unlock(&philo->data->print_message);
    }
}

void loop_act(t_philo *philo)
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
    ft_usleep(philo->data->time_eat);
    pthread_mutex_unlock(&philo->data->fork_mutex[philo->r_fork]);
    pthread_mutex_unlock(&philo->data->fork_mutex[philo->l_fork]);
    philo_print("is sleeping", philo, -1);
    ft_usleep(philo->data->time_eat);
    philo_print("is thinking", philo, -1);
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
        while (get_repeat(philo->data) && philo->data->max_eat != philo->meal_count)
            loop_act(philo);
    }
}

void data_init(t_data *data, char **argv, pthread_mutex_t *fork_mutex)
{
    data->philo_count = philo_atoi(argv[1]);
    data->time_die = philo_atoi(argv[2]);
    data->time_eat = philo_atoi(argv[3]);
    data->time_sleep = philo_atoi(argv[4]);
    data->start_time = get_time();
    data->max_eat = -1;
    if (argv[5])
        data->max_eat = philo_atoi(argv[5]);
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

t_philo *init(char **argv)
{
    int fork;
    pthread_mutex_t *fork_mutex;
    t_philo *philo;
    t_data *data1;
    
    fork = philo_atoi(argv[1]);
    fork_mutex = malloc(sizeof(pthread_mutex_t) * fork);
    if (!fork_mutex)
        return(NULL);
    data1 = malloc(sizeof(t_data));
    if (!data1)
        return (NULL);
    data_init(data1, argv, fork_mutex);
    philo = malloc(sizeof(t_philo) * fork);
    if (!philo)
        return (NULL);
    philo_init(data1, philo);
    return (philo);
}

void philo_exit(t_philo *philo)
{
    int i;

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
    free(philo->data);
    free(philo);
}

void monitor(int count, t_philo *philo)
{
    int i;
    
    i = 0;
    while (i < philo->data->philo_count)
    {
        pthread_mutex_lock(&philo[i].meal);
        if (philo[i].meal_count < philo->data->max_eat || philo->data->max_eat == -1)
        {
            if (philo->data->time_die < get_time() -  philo[i].last_meal_time)
            {
                philo_print("died", philo, -1);
                pthread_mutex_lock(&philo[i].data->eat_mutex);
                philo[i].data->repeat = false;
                pthread_mutex_unlock(&philo[i].data->eat_mutex);
                pthread_mutex_unlock(&philo[i].meal);
                break;
            }
        }
        if (philo[i].meal_count < count)
            count = philo[i].meal_count;
        pthread_mutex_unlock(&philo[i].meal);
        i++;
    }
}

bool input_error(int argc, char **argv)
{
    int i;

    if (argc != 5 && argc != 6)
    {
        printf("Input error\n");
        return (true);
    }
    i = 1;
    while (i < argc)
    {
        if (philo_atoi(argv[i]) == -1)
            return (true);
    }
    return (false);
}

int main(int argc, char **argv)
{
    t_data *data;
    t_philo *philo;
    int count;
    int i;

    if (argc != 5 && argc != 6)
        return (1);
    philo = init(argv);
    data = philo->data;
    i = 0;
    while (i < data->philo_count)
    {
        pthread_create(&philo[i].thread_id, NULL, (void *)philo_act, (void *)&philo[i]);
        i++;
    }
    while (get_repeat(philo->data) && count != data->max_eat)
    {
        pthread_mutex_lock(&philo[0].meal);
        count = philo[0].meal_count;
        pthread_mutex_unlock(&philo[0].meal);
        monitor(count, philo);
    }
    philo_exit(philo);
}