NAME = philo
INCLUDE = philo.h
SRC =	philo.c \
		ft_atoi.c \
		init.c \
		forks.c \
		activities.c \
		
CFLAGS = -Wall -Wextra -Werror

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@cc  $(CFLAGS) -I $(INCLUDE) -o $@ $^   

push:fclean
	@rm -rf outfile
	@git add .
	@read -p "Message:" message; \
	git commit -m "$$message"; \
	git push

clean:
	@rm -f $(OBJS)
fclean: clean
	@rm -f $(NAME)
re: fclean all