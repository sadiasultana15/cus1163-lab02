#include "proc_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int is_number(const char *str) {
if (str == NULL || *str == '\0') {
return 0;
}
while (*str) {
if (!isdigit(*str))
 return 0;
str++;
}
return 1;
}

int list_process_directories(void) {


DIR *dir = opendir("/proc");
struct dirent *entry;
int count = 0;

if (dir == NULL) {
perror("opendir");
return -1;
}
printf("PID     Type\n");
printf("___    ____\n");

while ((entry = readdir(dir)) != NULL) {
if (is_number(entry->d_name)) {
printf("%-8s process\n", entry->d_name);
count++;
 
}

}

closedir(dir);
printf("Found %d process directories\n", count);
printf("SUCCESS: Process directories listed!\n");

    return 0; // Replace with proper error handling
}

// read process info from /proc/[pid]
int read_process_info (const char* pid) {
    char filepath[256];
    char buffer[1024];
    ssize_t bytes_read;


//status file
  snprintf(filepath, sizeof(filepath), "/proc/%s/status", pid);
int fd = open(filepath, O_RDONLY);

    if (fd == -1) {
fprintf(stderr, "Failed to read %s\n", filepath);
return -1; 
}

printf("\n--- Process Information for PID %s ---\n", pid);
while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
buffer[bytes_read] = '\0';
printf("%s", buffer);
}
close(fd);

//Cmdline file
snprintf(filepath, sizeof(filepath), "/proc/%s/cmdline", pid);
fd = open(filepath, O_RDONLY);
if (fd == -1) {
fprintf(stderr, "Failed to read %s\n", filepath);
return -1;
}
printf("\n--- Command Line ---\n");
while ((bytes_read = read(fd, buffer, sizeof(buffer) -1)) > 0) {
buffer[bytes_read] = '\0';
printf("%s", buffer);
}
close(fd);

printf("\nSUCCESS: Process information read!\n");
return 0;

}

int show_system_info(void) {
char buffer[256];
    int line_count = 0;


    printf("\n--- CPU Information (first 10 lines) ---\n");
FILE *cpu = fopen("/proc/cpuinfo", "r");
if (cpu) {
while (fgets(buffer, sizeof(buffer), cpu) && line_count < 10) {
printf("%s", buffer);
line_count++;

}
fclose(cpu);
}



line_count = 0;
printf("\n--- Memory Information (first 10 lines) ---\n");
FILE *mem = fopen("/proc/meminfo", "r");
if (mem) {
while (fgets(buffer, sizeof(buffer), mem) && line_count <10) {
printf("%s", buffer);
line_count++;
} 
fclose(mem);
}

printf("SUCCESS: System information displayed!\n");
return 0;

}




void compare_file_methods(void) {
const char *filename = "/proc/version";
char buffer[256];
ssize_t bytes_read;

 printf("\n=== Method 1: Using System Calls ===\n");
int fd = open(filename, O_RDONLY);
if (fd == -1) {
perror("open");
return;

}

while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
buffer[bytes_read] = '\0';
printf("%s", buffer);

}

close(fd);



    printf("\n\n=== Method 2: Using Library Functions ===\n");
FILE *file = fopen(filename, "r");
if (file == NULL) {
perror("fopen");
return;

}

while (fgets(buffer, sizeof(buffer), file) != NULL) {
printf("%s", buffer);
}
fclose(file);

}
//
