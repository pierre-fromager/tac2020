# :neckbeard: Tac

Thread advanced control

Sync threads in time, set schedule policy and priority, set core affinity, isolate stack.  
Main illustrates usage with a producer/consumer pattern sharing dynamic storage.  

## :bust_in_silhouette: Me

* Freelance involved into IoT projects on embeded systems (atmel, xtensa, arm... ) and others MCUs for data acquisition and modules control where synced threads and low footprint are paramount. 
* RTOS curious, FreeRTOS user.

## :horse: Project

* Build few C libs providing facilities as FreeRTOS did with  [xTaskCreate](https://www.freertos.org/a00125.html) or [xTaskCreatePinnedToCore](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html). 

## :cyclone: Requirements

### Hardware

* At least proc with 2 cores to pin threads to different cores. 

### Os

* Linux but you can try to port it to other platform. 
* Pthread available as dev lib.  

## :hamster: Features

a task :

* is threaded. 
* has name. 
* has a sched policy. 
* has a sched priority. 
* has its own stack with a given size. 
* has cpu core assigned. 
* can be regulated in term of time consuming by frequency without burning cpu time. 

## :coffee: Todos

* provide ellapse for real work payload task. 
* trace potential memory leaks.
* implement shared memory.
* replace condition variables by signals.

## :crying_cat_face: Pitfall

* setting priority to RT (SCHED_RR) on a thread requires escalation privilege. 

## :rocket: Compilation

``` bash
make
```

``` bash
make clean
```
## :innocent: Crosscheck

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

## :bomb: Debug

### Check syntax

Syntax checker with cppcheck  
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

## :star2: Advices

* always check the returned value of methods. 
* printf is greedy on stack. 
* malloc involves free. 
* never trust garbage collector (trace memory leaks). 
