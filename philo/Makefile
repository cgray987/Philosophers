NAME		:= philo
CFLAGS		:= -Wall -Wextra -Werror -g -pthread
CC 			:= cc

SRC_PATH	:= src/
SRC			:= get_and_set.c init.c input.c philo.c routines.c sim_check.c sync.c utils.c virtual_dinner.c wrappers.c
SRCS		:= $(addprefix $(SRC_PATH), $(SRC))

OBJ_PATH	:= obj/
OBJ			:= ${SRC:.c=.o}
OBJS		:= $(addprefix $(OBJ_PATH), $(OBJ))

BOLD		:=	\033[1m
GREEN		:=	\033[32m
RED			:=	\033[31m
BCYAN		:=	\033[96m
NC			:=	\033[0m

all: $(NAME)

$(OBJ_PATH)%.o :$(SRC_PATH)%.c

	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): | $(OBJ_PATH)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(NAME): $(OBJS)
	@echo "$(BOLD)$(BCYAN)[ 🖩 Compiling $(NAME)... 🖩 ]$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(BOLD)$(GREEN)[ ❕$(NAME) ready❕ ]$(NC)"

clean:
	@rm -Rf $(OBJ_PATH)
	@echo "$(BOLD)$(RED)[   Obj files deleted   ]$(NC)"

fclean: clean
	@rm -rf $(NAME) $(NAME_B)
	@echo "$(BOLD)$(RED)[   Program deleted   ]$(NC)"

# Thread sanitizer rule must use gcc
san: CFLAGS += -fsanitize=thread -O1
san: CC = gcc
san: fclean $(NAME)
	@echo "$(BOLD)$(GREEN)[   USING THREAD SANITIZER   ]$(NC)"

re: clean all

party:
					@printf "\033c"
					@echo "\n\033[35m♪┏(・o･)┛♪"
					@sleep .5
					@printf "\033c"
					@echo "\033[1;33m♪┗(・o･)┓♪"
					@sleep .5
					@printf "\033c"
					@echo "\n\033[36m♪┏(・o･)┛♪"
					@sleep .5
					@printf "\033c"
					@echo "\033[34m♪┗(・o･)┓♪\n"

.PHONY: all, clean, fclean, re
