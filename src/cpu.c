#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
    
typedef struct _load_data_s load_data;
struct _load_data_s {
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long total;
};

double percentage(unsigned long current_value, unsigned long total_value)
{
    return 100.0 * ((double) current_value / (double) total_value); 
}

int main (void)
{
    FILE *loadfp;
    load_data *load;
    load_data *curr_load;
    
    load = (load_data *) malloc (sizeof (load_data));
    loadfp = fopen("/proc/stat","r");	
    fscanf(loadfp,"%*s %lu %lu %lu %lu\n", &load->user, &load->nice, &load->system, &load->idle);
    load->total = load->user + load->nice + load->system + load->idle;
    fprintf(stderr, "User: %.2f%, Nice: %.2f%, System: %.2f%, Idle: %.2f%\n", percentage(load->user,load->total), 
	    percentage(load->nice,load->total), percentage(load->system,load->total), 
	    percentage(load->idle,load->total));
    fprintf(stderr, "Used CPU: %.2f%\n", percentage(load->user + load->nice + load->system, load->total));
    
    return 0;
}

