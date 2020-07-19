# Tac

Thread advanced control

Threads facility to sync in time, set schedule policy and priority, set affinity to a core and  isolate stack.  
Main program illustrates this usage with a simple producer/consumer pattern sharing a dynamic storage.  

## Me

* Freelance interested in high and low level langages. 
* Involved into IoT projects on embeded systems (atmel, xtensa, arm... ) and others MCUs for data acquisition and modules control where synchronized threads are not optionals. 
* RTOS curious, FreeRTOS user.

## Project

* I decided to build few C libs providing facilities as FreeRTOS did with  [xTaskCreate](https://www.freertos.org/a00125.html) or [xTaskCreatePinnedToCore](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html). 

## Hardware requirements

* At least proc with 2 cores to pin threads to different cores. 

## Os requirements

* Linux but you can try to port it to other platform. 
* Pthread available as dev lib.  

## Roadmap

* check Features. 
* check Todos. 

## Features

a task :

* is threaded. 
* has name. 
* has a sched policy. 
* has a sched priority. 
* has its own stack with a given size. 
* has cpu core assigned. 
* can be regulated in term of time consuming by frequency without burning cpu time. 

## Todos

* let dstack to use Item as char* or other. 
* provide ellapse for real work payload task. 
* trace potential memory leaks.
* implement shared memory.

## Pitfall

* setting priority to RT (SCHED_RR) on a thread requires escalation privilege. 

## Compilation

``` bash
make
```

``` bash
make clean
```
## Crosscheck

### Cpu affinity 

get thread pids  
``` bash
ps -e -T | grep tac
```

The lower pid is for the main, others pids are for threads.  
So we can check now affinity.  
``` bash
taskset -pc <threadpid>
```
Could return : liste d'affinités actuelle du PID <threadpid> : 0  
Mean the thread is attached to the core 0.  

### System RT priorities
``` bash
chrt -m
```
Provides priotity range by policy.  
Example  
``` bash
SCHED_OTHER min/max priority	: 0/0
SCHED_FIFO min/max priority	: 1/99
SCHED_RR min/max priority	: 1/99
SCHED_BATCH min/max priority	: 0/0
SCHED_IDLE min/max priority	: 0/0
SCHED_DEADLINE min/max priority	: 0/0
```
Lowest is the value highest is the priority.

## Debug

### Check syntax

Syntaxt checker cppcheck  
``` bash
make check
```

### Profiling

``` bash
make profile
```
This use valgrind to generate the callgrind.* files.  
Use QCachegring to check whatever.  

### Gdb

``` bash
make debug
```
Debug sample  
``` bash
run
backtrace
frame N
print somevar
next
kill
quit
```
### Valgrind
To trace memory  
``` bash
make trace
```

## Advices

* always check the returned value of methods. 
* printf is greedy on stack. 
* malloc involves free. 
* never trust garbage collector (trace memory leaks). 
