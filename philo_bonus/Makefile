NAME = philo_bonus
INCLUDE = philo_bonus.h
SRC =	philo_bonus.c \
		ft_atoi_bonus.c \
		activities_bonus.c \
		init_bonus.c \
		forks_bonus.c \

		
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