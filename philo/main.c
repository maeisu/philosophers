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

void philo_act()
{
    printf("test\n");
}

int main(int argc, char **argv)
{
    for (int i=0; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");

    if (argc != 5 && argc != 6)
        return (1);

    int fork = atoi(argv[1]);
    int to_die = atoi(argv[2]);
    int to_eat = atoi(argv[3]);
    int to_sleep = atoi(argv[4]);
    int max_eat = -1;
    if (argv[5])
        max_eat = atoi(argv[5]);
    if (to_die < (to_eat + to_sleep))
        return (1);
    pthread_mutex_t *fork_mutex;
    fork_mutex = malloc(sizeof(fork_mutex) * fork);
    if (!fork_mutex)
        return (1);
    t_philo *philo;
    philo = malloc(sizeof(t_philo) * fork);
    if (!philo)
        return (1);
    int i = 0;
    while (i < fork)
    {
        pthread_mutex_init(&fork_mutex[i++], NULL);
        philo[i].r_fork = i;
        philo[i].l_fork = (i + 1) % fork;
        philo[i].meal_count = 0;
    }
    i = 0;
    while (i < fork)
    {
        pthread_create(&philo[i++].thread_id, NULL, (void *)philo_act, NULL);
    }
    i = 0;
    while (i < fork)
    {
        pthread_join(philo[i++].thread_id, NULL);
    }
}