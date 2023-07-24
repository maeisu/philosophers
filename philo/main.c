#include "philo.h"

// t_threaddata *data_init(int argc, char **argv)
// {
//     t_threaddata *data;
//     t_threaddata *tmp;
//     t_threaddata *first;
//     int i;

//     tmp = NULL;
//     i = 0;
//     while (i < argc)
//     {
//         data = malloc(sizeof(t_threaddata));
//         pthread_mutex_init(&data->mutex, NULL);
//         data->right = false;
//         data->left = false;
//         data->die = atoi(argv[2]);
//         data->eat = atoi(argv[3]);
//         data->sleep = atoi(argv[4]);
//         data->status = THINK;
//         if (tmp != NULL)
//         {
//             tmp->next = data;
//             data->prev = tmp;
//         }
//         else
//         {
//             first = data;
//             data->prev = NULL;
//         }
//         tmp = data;
//         i++;
//     }
//     tmp->next = NULL;
//     return (first);
// }

// void *thread(void *t_data)
// {
//     t_threaddata *data;

//     data = (t_threaddata *)t_data;
//     pthread_mutex_lock(&data->mutex);
//     pthread_mutex_unlock(&data->mutex); 
//     return(NULL);
// }

// void    philo(int argc, char **argv)
// {
//     t_threaddata *data;
//     t_threaddata *tmp;
//     int fork;

//     fork = atoi(argv[1]);
//     data = data_init(argc, argv);
//     tmp = data;
//     while (data)
//     {
//         pthread_create(&data->thread_id, NULL, thread, (void *)data);
//         data = data->next;
//     }
//     data = tmp;
//     while (data->next)
//     {
//         pthread_join(data->thread_id, NULL);
//         data = data->next;
//     }
// }

void philo_act(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    printf("%d\n", philo->id);
    if (philo->id % 2 == 1)
        usleep(200);
    pthread_mutex_lock(&philo->data->fork_mutex[philo->r_fork]);
    pthread_mutex_lock(&philo->data->fork_mutex[philo->l_fork]);
    pthread_mutex_lock(&philo->data->eat_mutex);
    printf("timestamp_in_ms %d is eating\n", philo->id);
    pthread_mutex_unlock(&philo->data->eat_mutex);
    usleep(philo->data->time_eat * 1000);
    pthread_mutex_unlock(&philo->data->fork_mutex[philo->r_fork]);
    pthread_mutex_unlock(&philo->data->fork_mutex[philo->l_fork]);
    usleep(philo->data->time_sleep * 1000);
}

int main(int argc, char **argv)
{
    for (int i=0; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");

    if (argc != 5 && argc != 6)
        return (1);

    int fork = atoi(argv[1]);
    pthread_mutex_t *fork_mutex;
    fork_mutex = malloc(sizeof(fork_mutex) * fork);
    if (!fork_mutex)
        return (1);
    t_data data;
    data.time_die = atoi(argv[2]);
    data.time_eat = atoi(argv[3]);
    data.time_sleep = atoi(argv[4]);
    data.max_eat = -1;
    data.fork_mutex = fork_mutex;
    pthread_mutex_init(&data.eat_mutex, NULL);
    if (argv[5])
        data.max_eat = atoi(argv[5]);
    if (data.time_die < (data.time_eat + data.time_sleep))
        return (1);
    t_philo *philo;
    philo = malloc(sizeof(t_philo) * fork);
    if (!philo)
        return (1);
    int i = 0;
    while (i < fork)
    {
        philo[i].id = i; 
        philo[i].r_fork = i;
        philo[i].l_fork = (i + 1) % fork;
        philo[i].meal_count = 0;
        philo[i].data = &data;
        pthread_mutex_init(&fork_mutex[i++], NULL);
    }
    i = 0;
    pthread_t thread_id[10];
    while (i < fork)
    {
        pthread_create(&thread_id[i], NULL, (void *)philo_act, (void *)&philo[i]);
        i++;
    }
    // printf("test\n");
    i = 0;
    while (i < fork)
    {
        pthread_join(thread_id[i++], NULL);
    }
}