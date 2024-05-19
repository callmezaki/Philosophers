# Philosophers

## Description

Philosophers is a project that involves solving the classic Dining Philosophers problem using both processes and threads. This project is part of the 42 curriculum and aims to enhance understanding of concurrency, synchronization, and deadlocks.

## Features

- Multiple philosopher processes (main part)
- Synchronization using semaphores
- Handling of race conditions and deadlocks
- Bonus part with philosopher threads

## Getting Started

### Prerequisites

- GCC or any C compiler
- Make

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/callmezaki/Philosophers.git
   cd Philosophers
   ```

2. Compile the project:

   ```bash
   make
   ```

### Usage

Run the program with the following syntax:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

Example:

```bash
./philo 5 800 200 200
```

## Project Structure

- `philo/`: Contains the source files for the main part of the project using processes.
- `philo_bonus/`: Contains the source files for the bonus part using threads.
- `Makefile`: Defines the build process for the project.
- `README.md`: Project documentation.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [42 School](https://42.fr/en/homepage/) for providing the project framework.
- The 42 community for their support and collaboration.
