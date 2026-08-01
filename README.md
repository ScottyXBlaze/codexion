*This project has been created as part of the 42 curriculum by nyramana.*

# Codexion

> While loop is not enough.

## **Description**

**Codexion** is a multithread project that simulate the Dining-Philosopher Problem. It is a classic synchronization and concurrency problem that deals with resource sharing, deadlock, and starvation in systems where multiple processes require limited resources.

The *Dining Philosopher Problem* involves 'n' philosophers sitting around a circular table. But here, we talk about *coder* sitting in a circular hub. Each coder alternates between three states: **Compiling**, **refactoring**, and **debugging**. To compile, A coder need two *dongles*, one on their left and one on their right. However, the number of dongle is equal to the number of coders, and each dongle is shared between two neighboring coders.

### Constraints and conditions

- Every coder needs two dongles to compile.
- Every coder needs to pick up the dongle on the left and right.
- Coder only compile when they have two dongles.
- After compiling, coder must release their dongles and start debugging and finally refactoring before restarting again.
- Each dongle has a cooldown after being used.

### Goal

The goal is to learn how these properties can be handled with multithreading, mutexes, and condition variables. It teaches how to manage deadlock and starvation so that every thread can get their resources evenly without directly communicating to each other.

### Brief Overview

To run the program, you first need to compile the code with the **Makefile** then run the program. If you run the program, it will show some instructions to follow so that the program can run successfully.

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

To run the program, we have to give it some parameters:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- **number_of_coders**: The number of coders in the table. The number of coders is also the same as the number of dongles.
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

- Removing artifacts (.o files):

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

- [GeeksforGeeks Dining-Philosopher](https://www.geeksforgeeks.org/operating-systems/dining-philosophers-problem/)

- [GeeksforGeeks Multithread](https://www.geeksforgeeks.org/c/multithreading-in-c/)

- [Introduction Thread C](https://www.youtube.com/watch?v=ldJ8WGZVXZk)

- [Detailed Thread Playlist](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

- [Codexion Visualizer](https://codexion-visualizer.sacha-dev.me/)

### AI usage

- AI was used to guide what was wrong with my approach on how I managed the coders and teach me how to really use threads.

## Extras

### Blocking cases handled

#### Deadlock

It is possible to have a deadlock if we don't handle it. A popular reason that a deadlock can happen is the *Coffman's conditions*. All the conditions are:

- **Mutual Exclusion**: If multiple threads share one resource. They cannot manipulate the resource at the same time.
- **Hold and Wait**: If a process is having one resource and requests another one but the other process holds the resource.
- **No Preemption**: It means that a process cannot forcefully take a resource from another process.
- **Circular Wait**: It means that every process is requesting a resource while holding one so no one can get two resources at the same time.

If every condition is true, only then the deadlock can happen.

And our project already has some of them like the **Mutual Exclusion**, **Hold and Wait**, **No Preemption**. So to handle deadlock, we really need to handle the Circular Wait. And to do that, the solution I implemented was simple. If there are multiple coders, we first check if his ID is even, if it is, we need to acquire the left dongle before the right one, and if its ID is odd, we try to acquire the right dongle first.

- What happens if every coder tries to access their left dongle?

A deadlock will happen because everyone holds the left dongle and tries to access the right one, or the right dongle is held by the next coder. So every coder waits indefinitely.

By using this method, adjacent coders with differing parity will compete for the same initial dongle before either of them picks up a second one. And with that, at least one coder is guaranteed to fail picking up their first dongle and won't hold any resource while waiting.

#### Starvation

To handle starvation, we used the scheduler. There are two schedulers which are the FIFO or "First In First Out" and EDF or "Earliest Deadline First".

The two schedulers are implemented with the dongle which handles the coders who can take them.

#### Cooldown handling

The cooldown is implemented by using `gettimeofday` with some basic condition and addition and `usleep` to make the dongle sleep.

#### Burnout detection

To detect the burnout, we used another thread to check the burnout time. To do that, it checks every coder's last_compile time and checks if the current time minus the last compile time is greater than the burnout time.

#### Log serialization

The log is made by using the `print_state` function which locks a mutex, prints the state and then unlocks the mutex.

### Thread synchronization mechanisms

#### Pthread_mutex_t

Mutexes are used to avoid race conditions by locking every variable that can be acquired by multiple threads. They also manage dongle acquisition by locking the dongle and releasing it only after the dongle is used.

#### Pthread_cond_t

Cond is used to make the coder sleep if they cannot take the dongle and wakes up other coder if they finished their compile time.

#### custom event implementation

To start every coder, `Pthread_cond_t` was not enough because sometimes not every thread wakes up. So I used the basic usleep so that it is guaranteed that every coder wakes up or misses the signal.

#### Example of race condition

Race conditions are handled, like we said before, by using mutexes. A basic example is the monitoring where the monitor checks the burnout time. Or the print part of the program where the prints can overlap if we don't use a mutex.

#### thread-safe communication

A thread-safe communication is made by using functions that tell the coders if it's their turn (we don't forget the mutex).
