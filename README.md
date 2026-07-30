*This project has been created as part of the 42 curriculum by nyramana.*

# Codexion

> While loop is not enough.

## **Description**

**Codexion** is a multithread project that simulate the Dining-Philosopher Problem. It is a classic synchronization and concurrency problem that deals with resource sharing, deadlock, and starvation in systems where multiple processes require limited resources.

The *Dining Philosopher Problem* involves 'n' philosophers sitting around a circular table. But here, we talk about *coder* sitting in a circular hub. Each coder alternate between three state: **Compiling**, **refactoring**, and **debuging**. To compile, A coder need two *dongles*, one on their left and one on their right. However, the number of dongle is equal to the number of coders, and each dongle is shared between two neighboring coders.

### Constraint and conditions

- Every Coder needs two dongle to compile.
- Every Coder need to pick up the dongle on the left and right.
- Coder only compile when they have two dongles.
- After compiling, coder must release their dongles and start debugging and finally refactoring before restarting again.
- Each dongle has cooldown after being used.

### Goal

The goal is to learn how these property can be handled with multithreading, mutexes, and cond. It teaches how to manage deadlock and starvation so that every thread can get their resources evenly without directly communicating to each other.

### Brief Overview

To run the program. You first need to compile the code with the **Makefile** then run the program. If you run the program. It will show some instruction to follow so that the program can run successfully.

- Simple example:

```bash
./codexion 10 1000 100 100 100 2 100 fifo
```

- And the output should be like this:

```bash
0 10 has taken a dongle
0 10 has taken a dongle
0 10 is compiling
0 2 has taken a dongle
0 2 has taken a dongle
0 2 is compiling
1 8 has taken a dongle
1 8 has taken a dongle
1 8 is compiling
1 7 has taken a dongle
1 4 has taken a dongle
1 4 has taken a dongle
1 4 is compiling
100 2 is debugging
100 10 is debugging
101 8 is debugging
101 4 is debugging
...
```

### Execution

To run the program, We have to give it some parameters:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- **number_of_coders**: The number of coder in the table. The number of coder is also the same as the number of dongle.
- **time_to_burnout**: The time for every coder to have before they burn out. This is taken while the coder don't do any actions. 
- **time_to_compile**: The time for every coder to compile and hold the dongle.
- **time_to_debug**: The time for every coder to debug. In this state, the coder doesn't hold dongle anymore.
- **time_to_refactor**: The time for coder to refactor. Like the time_to_debug, the coder doesn't do anything but wait.
- **number_of_compiles_required**: The number of cycle that the coder should do to finish their job.
- **dongle_cooldown**: The time to give to the dongle after they are released so that they can rest.
- **scheduler**: The policy used by dongle to know which coder should take them. FIFO or "First In, First Out" means that the first coder who requested them is the person who need them. EDF or "earliest deadline first" means the coder who is the nearest to the burnout is prioritized.

### Usage

Input:

```bash
./codexion 4 1500 200 200 200 1 100 fifo
```

Output:

```bash
0 2 has taken a dongle
0 2 has taken a dongle
0 2 is compiling
0 1 has taken a dongle
200 2 is debugging
300 1 has taken a dongle
300 1 is compiling
300 3 has taken a dongle
300 3 has taken a dongle
300 3 is compiling
400 2 is refactoring
500 1 is debugging
500 3 is debugging
600 4 has taken a dongle
600 4 has taken a dongle
600 4 is compiling
700 1 is refactoring
700 3 is refactoring
800 4 is debugging
1000 4 is refactoring
```

## **Instructions**

### Makefile

- Compile the program:

```bash
make # or make all
```

- Removing artifact (.o file):

```bash
make clean
```

- Cleaning the project (.o and executable file):

```bash
make fclean
```

- Rebuild the program:

```bash
make re
```

## **Resources**

- **Peer learning**

- [GeekForGeeks Dining-Philosopher](https://www.geeksforgeeks.org/operating-systems/dining-philosophers-problem/)

- [GeekForGeeks Multithread](https://www.geeksforgeeks.org/c/multithreading-in-c/)

- [Introduction Thread C](https://www.youtube.com/watch?v=ldJ8WGZVXZk)

- [Detailed Thread Playlist](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

- [Codexion Visualizer](https://codexion-visualizer.sacha-dev.me/)

### AI usage

- AI was used to guide what was wrong with my approach on how I managed the coders and Teach me how to really use thread.

## Extras

### Blocking cases handled

#### Deadlock

It is possible to have a deadlock if we don't handle it. A popular reason that a deadlock can happens is the *Coffman’s conditions*. All the conditions are:

- **Mutual Exclusion**: If multiple thread one resource. They cannot manipulate the resource at the same time.
- **Hold and Wait**: If a process is having one resource and request another one but the other process hold the resource.
- **No Preemption**: It means that a process cannot forcefully take a resource from another process.
- **Circular wait**: It means that every processor is requesting a resource while holding one so no one can get two resource at the same time.

If every condition is true, Only then the deadlock can happen.

And our project already has some of them like the **Mutual Exclusion**, **Hold and wait**, **No Preemption**. So to handle deadlock, We really need to handle the Circular wait. And to do that, the solution I implemented was simple. If there a multiple coders, we first check if his ID is even, if it is, we need to acquire the right dongle before the right one, and if it's ID is odd, We try to acquire the left dongle.

- What happen if Every coder try to access their left dongle?

A deadlock will happen because everyone hold the left dongle and try to access the right one, or the right dongle is held by the next coder. So every coder wait indefinitely.

By using this method. Adjacent coder with differing parity will compete for the same initial dongle before either of them pick up a second one. And with that, at least one coder is guaranteed to fail picking up their first dongle and won't hold any resource while waiting.

#### Starvation

To handle starvation, We used the scheduler. There are two scheduler which is the FIFO or "First In First Out" and edf or "Earliest Deadline First"

The two scheduler is implemented with the dongle which handle the coder who can take them.

#### Cooldown handling

The cooldown is implemented by using `gettimeofday` with some basic condition and addition and `usleep` to make the dongle sleep.

#### Burnout detection

To detect the burnout, we used another thread to check the burnout time. To do that, he checks every coder last_compile time and check if the current time minus the last compile time is greater than the burnout time.

#### Log serialization

The log is made by using the `print_state` function which lock a mutex, print the state and the unlock the mutex.

### Thread synchronization mechanisms

#### Pthread_mutex_t

Mutex are used to avoid race condition by locking every variable that can be acquired by multiple thread. And also manage dongle acquisition by locking the dongle and releasing only after the dongle is used.

#### Pthread_cond_t

Cond is used to make the coder sleep if they cannot take the dongle and wakes up other coder if they finished their compile time.

#### custom event implementation

To start every coders. `Pthread_cond_t` was not enough because sometimes not every thread wakes up. So I used the basic usleep so that it is guaranteed that every coder wakes up or miss the signal.

#### Example of race condition

Race condition is handle, like we said before, by using mutex. A basic example is the monitoring where the monitor checks the burnout time. Or the print part of the program where the print can overlap we don't use a mutex.

#### thread-safe communication

A thread-safe communication is made by using function who tell the coders if it their turn (We don't forgot the mutex).
