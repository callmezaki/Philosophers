NAME = philo
INCLUDE = philo.h
SRC =	philo.c \
		ft_atoi.c \
		init.c \
		activities.c \
		check_args.c \
		
CFLAGS = -Wall -Wextra -Werror

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@cc  $(CFLAGS) -I $(INCLUDE) -o $@ $^

clean:
	@rm -f $(OBJS)
fclean: clean
	@rm -f $(NAME)
re: fclean all