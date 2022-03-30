#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

#define LENGTH 10000
#define TIME_INTERVAL 3

struct time_and_num_req
{
    int number;
    time_t time;
};

time_t make_time(char *file_str, time_t *time);
int month(char *time_str);
void find_max_num_req(int time_interval, int *max_num_req, time_t *max_num_req_time,
                      time_t *max_num_req_time_end, struct time_and_num_req *short_arr_req, int num_sev_time);
void print_time_window(int time_interval, time_t *max_num_req_time, time_t *max_num_req_time_end);
void print_5xx_req(int num_5xx_req, char (*requests_list)[1600]);
void error();

int main(int argc, char *argv[])
{

    printf("================================\n");
    FILE *file_copy;
    file_copy = fopen(argv[1], "r");
    fseek(file_copy, 0, 2);
    int num_bytes = ftell(file_copy);
    struct time_and_num_req *short_arr_req;
    short_arr_req = malloc(((int)(num_bytes / 20) * sizeof(struct time_and_num_req)));

    FILE *file;
    file = fopen(argv[1], "r");

    if (argc != 2 || file == NULL)
    {
        error();
        return -1;
    }

    char *request_str;
    char *status;
    char requests_list[1000][1600];
    int num_5xx_req = 0;
    time_t time;
    time_t time_befor;
    int num_requests = 1;
    int time_dif = 0;
    int num_sev_time = -1;
    struct time_and_num_req time_and_num_req_present;
    char *file_str;
    file_str = malloc(LENGTH * sizeof(char));

    while ((fgets(file_str, LENGTH, file)) != NULL)
    {
        make_time(file_str, &time);
        time_dif = (int)difftime(time, time_befor);

        if (time_dif == 0)
        {
            num_requests++;
        }
        else
        {
            if (num_sev_time != -1)
            {
                time_and_num_req_present.number = num_requests;
                time_and_num_req_present.time = time_befor;
                short_arr_req[num_sev_time] = time_and_num_req_present;
            }
            num_sev_time++;
            num_requests = 1;
        }
        time_befor = time;

        request_str = strtok(NULL, "\"");
        for (int i = 0; i < 1; i++)
        {
            request_str = strtok(NULL, "\"");
        }

        status = strtok(NULL, " ");
        if (status[0] == '5')
        {
            strcpy(requests_list[num_5xx_req], request_str);
            num_5xx_req++;
        }
    }

    if (num_sev_time != -1)
    {
        time_and_num_req_present.number = num_requests;
        time_and_num_req_present.time = time_befor;
        short_arr_req[num_sev_time] = time_and_num_req_present;
    }
    num_sev_time++;

    printf("Number of  failed requests (Errors):%d\n", num_5xx_req);

    int time_interval = TIME_INTERVAL;
    int max_num_req = 0;
    time_t max_num_req_time;
    time_t max_num_req_time_end;
    find_max_num_req(time_interval, &max_num_req, &max_num_req_time, &max_num_req_time_end, short_arr_req, num_sev_time);
    printf("maximum number of requests: %d (time interval parameter: %d)\n",
           max_num_req, time_interval);

    print_time_window(time_interval, &max_num_req_time, &max_num_req_time_end);

    print_5xx_req(num_5xx_req, requests_list);

    free(file_str);
    free(short_arr_req);
    fclose(file);
    return 0;
}

time_t make_time(char *file_str, time_t *time)
{
    char str_copy[1600] = "";
    char *time_str;
    struct tm time_struct;
    strcpy(str_copy, file_str);
    time_str = strtok(str_copy, "[");
    time_str = strtok(NULL, "/");
    time_struct.tm_mday = atoi(time_str);
    time_str = strtok(NULL, "/");
    time_struct.tm_mon = month(time_str);
    time_str = strtok(NULL, ":");
    time_struct.tm_year = atoi(time_str) - 1900;
    time_str = strtok(NULL, ":");
    time_struct.tm_hour = atoi(time_str);
    time_str = strtok(NULL, ":");
    time_struct.tm_min = atoi(time_str);
    time_str = strtok(NULL, "-");
    time_struct.tm_sec = atoi(time_str);
    *time = mktime(&time_struct);
}

int month(char *time_str)
{
    int num;
    if (strcmp(time_str, "Jan") == 0)
        num = 0;
    else if (strcmp(time_str, "Feb") == 0)
        num = 1;
    else if (strcmp(time_str, "Mar") == 0)
        num = 2;
    else if (strcmp(time_str, "Apr") == 0)
        num = 3;
    else if (strcmp(time_str, "May") == 0)
        num = 4;
    else if (strcmp(time_str, "Jun") == 0)
        num = 5;
    else if (strcmp(time_str, "Jul") == 0)
        num = 6;
    else if (strcmp(time_str, "Aug") == 0)
        num = 7;
    else if (strcmp(time_str, "Sep") == 0)
        num = 8;
    else if (strcmp(time_str, "Oct") == 0)
        num = 9;
    else if (strcmp(time_str, "Nov") == 0)
        num = 10;
    else if (strcmp(time_str, "Dec") == 0)
        num = 11;
    return num;
}

void find_max_num_req(int time_interval, int *max_num_req, time_t *max_num_req_time, time_t *max_num_req_time_end,
                      struct time_and_num_req *short_arr_req, int num_sev_time)
{
    int end = 0;
    time_t time_start;
    time_t time_end;
    int hit_num_req = 0;
    int time_dif = 0;

    for (int i = 0; i < num_sev_time; i++)
    {
        time_start = short_arr_req[i].time;
        time_end = short_arr_req[end].time;
        time_dif = (int)difftime(time_end, time_start);

        while (time_dif < time_interval)
        {
            hit_num_req = hit_num_req + short_arr_req[end].number;
            end++;
            if (end >= num_sev_time)
                break;
            time_end = short_arr_req[end].time;
            time_dif = (int)difftime(time_end, time_start);
        }

        if (*max_num_req < hit_num_req)
        {
            *max_num_req = hit_num_req;
            *max_num_req_time = short_arr_req[i].time;
            *max_num_req_time_end = short_arr_req[end - 1].time;
        }

        hit_num_req = hit_num_req - short_arr_req[i].number;
    }
}

void print_time_window(int time_interval, time_t *max_num_req_time, time_t *max_num_req_time_end)
{
    char *time_window_str;
    time_window_str = malloc(22);
    struct tm *time_struct = localtime(max_num_req_time);
    strftime(time_window_str, 22, "%d/%b/%Y:%H:%M:%S", time_struct);

    printf("Time window: %s -- ", time_window_str);
    free(time_window_str);

    time_window_str = malloc(22);
    time_struct = localtime(max_num_req_time_end);
    strftime(time_window_str, 22, "%d/%b/%Y:%H:%M:%S", time_struct);
    printf("%s\n", time_window_str);
    free(time_window_str);
    printf("================================\n");
}

void print_5xx_req(int num_5xx_req, char (*requests_list)[1600])
{
    for (int i = 0; i < num_5xx_req; i++)
    {
        printf("%s\n", requests_list[i]);
    }
}

void error()
{
    printf("Give a file with logs\n");
}