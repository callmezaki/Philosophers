NAME = Philo
INCLUDE = Philo.h
SRC =	Philo.c \
		ft_atoi.c \
		
CFLAGS = -Wall -Wextra -Werror

OBJS = $(SRC:.c=.o)
# OBJ_B = $(SRC_B:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@cc  $(CFLAGS) -I $(INCLUDE) -o $@ $^   

# bonus: $(CHECK)

# $(CHECK):$(OBJ_B)
# 	@cc  $(CFLAGS) -I $(INCLUDE_B) -o $(CHECK) $^ 

clean:
	@rm -f $(OBJS)
fclean: clean
	@rm -f $(NAME)
re: fclean all