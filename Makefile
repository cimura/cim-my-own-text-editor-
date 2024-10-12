NAME		:=	my_vim
LIST_SRCS	:=	main.c setup.c

DIR_SRCS	:=	src
DIR_OBJS	:=	objs
DIR_INCS	:=	inc

SRCS		:=	$(addprefix $(DIR_SRCS)/,$(LIST_SRCS))
OBJS		:=	$(addprefix $(DIR_OBJS)/,$(LIST_SRCS:.c=.o))

INCS		:=	$(addprefix $(DIR_INCS)/,$(LIST_SRCS:.c=.h))

CC			:=	cc
CFLAGS		:=	-Wall -Wextra -Werror
DEBUG_FLAGS	:=	-g -fsanitize=address

ERASE		:=	\033[2K\r
PINK		:=	\033[35m
BLUE		:=	\033[34m
GREEN		:=	\033[32m
END			:=	\033[0m

all: $(NAME)

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c
	@$(CC) $(CFLAGS) -I $(DIR_INCS) -c $< -o $@
	@printf "$(ERASE)$(BLUE) > Compilation :$(END) $<"

$(DIR_OBJS):
	@mkdir -p $(DIR_OBJS)

$(NAME): $(DIR_OBJS) $(OBJS)
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) -I $(DIR_INCS) -o $(NAME) $(OBJS)
	@printf "$(ERASE)$(GREEN)$@ made\n$(END)"

clean:
	@rm -rf $(DIR_OBJS)
	@printf "$(PINK)remove obj dir\n$(END)"

fclean: clean
	@rm -f $(NAME)
	@printf "$(PINK)remove my_vim\n$(END)"

re: fclean all

.PHONY: all clean fclean re