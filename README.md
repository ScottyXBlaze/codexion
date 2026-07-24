*This project has been created as part of the 42 curriculum by nyramana.*

# Codexion

> While loop is not enough.

## **Description**

**Codexion** is a multithread project that simulate the Dining-Philosopher Problem. It is a classic synchronization and concurrency problem that deals with resource sharing, deadlock, and starvation in systems where multiple processes require limited resources.

The *Dining Philosopher Problem* involves 'n' philosophers sitting around a circular table. But here, we talk about *coder* sitting in a circular hub. Each coder alternate between three state: **Compiling**, **refactoring**, and **debuging**. To compile, A coder need two *dongles*, one on their left and one on their right. However, the number of dongle is equal to the number of coders, and each dongle is shared between two neighboring coders.

### Constraint and conditions:

- Every Coder needs two dongle to compile.
- Every Coder pick up the dongle on the left and right.
- Coder only compile when they have two dongles.
- After compiling, coder must release their dongles and start debugging and finally refactoring before restarting again.
- Each dongle has cooldown after being used.

### Goal

The goal is to learn how these property can be handled with multithreading and mutexes. It teaches how to manage deadlock and starvation so that every thread can get their resources evenly without communicating to each other.

### Brief Overview

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


### Usage:

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

- [GeekForGeeks Dining-Philosopher](https://www.geeksforgeeks.org/operating-systems/dining-philosophers-problem/)

- [GeekForGeeks Multithread](https://www.geeksforgeeks.org/c/multithreading-in-c/)

- [Introduction Thread C](https://www.youtube.com/watch?v=ldJ8WGZVXZk)

- [Detailed Thread Playlist](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

### AI usage

- AI was used to guide what was wrong with my approach and my general structure.

## Extras

### Blocking cases handled

### Thread synchronization mechanisms
