#ifndef PHILO_H
#define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

// struct s_philo
// {
//     enum status;

// };

typedef enum s_status{
    SLEEP,
    EAT,
    THINK,
}           t_status;

typedef struct s_data
{
    int time_eat;
    int time_sleep;
    int time_die;

}   t_data;

typedef struct s_philo
{
    pthread_t id;
    // pthread_mutex_t mutex;
    pthread_t thread_id;
    int r_fork;
    int l_fork;
    int meal_count;

}   t_philo;


#endif