## **************************************************************************** #
##                                                                              #
##                                                         :::      ::::::::    #
##    Makefile                                           :+:      :+:    :+:    #
##                                                     +:+ +:+         +:+      #
##    By: lchauvet <lchauvet@student.42.fr>          +#+  +:+       +#+         #
##                                                 +#+#+#+#+#+   +#+            #
##    Created: 2024/11/19 15:53:44 by mgendrot          #+#    #+#              #
##    Updated: 2025/01/31 17:09:37 by lchauvet         ###   ########.fr        #
##                                                                              #
## **************************************************************************** #

# Variables

NAME			= lem-in

INCLUDE			= include

TMP				= .tmp/
CC				= cc 
CFLAGS_Dbeug	= -fsanitize=address,undefined -g
CFLAGS			= -Wall -Wextra -Werror -g -O3

RM				= rm -f
MKDIR			= mkdir

LIBFT_DIR		= libft/
LIBFT_CFLAGS	= -L . -l ft
LIBFT			= libft.a
LIBFT_INC_H		= include/libft.h



# **************************************************************************** #
#                                   Colors                                     #
# **************************************************************************** #

DEF_COLOR		= \033[0;39m
GRAY			= \033[0;90m
RED				= \033[0;91m
GREEN			= \033[0;92m
YELLOW			= \033[0;93m
BLUE			= \033[0;94m
MAGENTA			= \033[0;95m
CYAN			= \033[0;96m
WHITE			= \033[0;97m
BOLD			= $'\x1b[1m'
END				= $'\x1b[0m'
TERM_UP			= \033[1A
TERM_CLEAR_LINE	= \033[2K\r

# **************************************************************************** #
#                                   Sources                                    #
# **************************************************************************** #

SRC_DIR		= src/

SRC = $(shell find ./src -iname "*.c")


OBJ_DIR			= $(TMP)obj/
OBJ				= $(patsubst %.c, $(OBJ_DIR)%.o, $(SRC))

DEPS			= $(OBJ:.o=.d)

# **************************************************************************** #
#                             progress_update                                  #
# **************************************************************************** #

TOTAL_FILES     := $(words $(OBJ))
COMPILED_COUNT  = 0

define progress_update
    @if [ ! -f .compiled_count ]; then \
        echo 0 > .compiled_count; \
        printf "\n"; \
    fi; \
    COMPILED_COUNT=$$(cat .compiled_count); \
    COMPILED_COUNT=$$((COMPILED_COUNT + 1)); \
    echo $$COMPILED_COUNT > .compiled_count; \
    printf "$(TERM_UP)$(TERM_CLEAR_LINE)$(CYAN)Compilation progress: \
        $$((COMPILED_COUNT * 100 / $(TOTAL_FILES)))%% $(BLUE) ->$(1) $(DEF_COLOR)\n"; \
    if [ $$COMPILED_COUNT -eq $(TOTAL_FILES) ]; then \
        rm -f .compiled_count; \
    fi
endef
# **************************************************************************** #\
#                                   Rules                                      #
# **************************************************************************** #

all: lib $(NAME) 
	@printf "$(GREEN)All targets compiled successfully!$(DEF_COLOR)\n"

# **************************************************************************** #
#                                   libift                                     #
# **************************************************************************** #

cleanlibs:
	@$(MAKE) clean -C $(LIBFT_DIR)	-s
	
fcleanlibs:
	@$(MAKE) fclean -C $(LIBFT_DIR) -s
	@$(RM) $(LIBFT)
	@$(RM) $(LIBFT_INC_H)

lib : $(LIBFT)

$(LIBFT):
	@printf "$(MAGENTA)Making archive $(TERM_BLUE)\"%s\"$(MAGENTA)...$(DEF_COLOR)\n" $@
	@$(MAKE) -C $(LIBFT_DIR) -s
	@cp -u $(LIBFT_DIR)/$(LIBFT_INC_H) $(INCLUDE)
	@cp $(LIBFT_DIR)/$(LIBFT) ./ 
	@printf "$(TERM_CLEAR_LINE)$(GREEN)Done copying archive $(BLUE)\"%s\"$(DEF_COLOR) !\n$(TERM_RESET)" $@

relibft: fcleanlibs lib

# **************************************************************************** #
#                                   lem-in                                     #
# **************************************************************************** #

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_CFLAGS) $(MINILIBX_CFLAGS)  -o $(NAME)
	@printf "$(GREEN)%s compiled!\n$(DEF_COLOR)" $(NAME)

$(OBJ_DIR)%.o: %.c | $(OBJF)
	@mkdir -p $(dir $@)
	$(call progress_update,$(notdir $@))
	@$(CC) $(CFLAGS) -I $(INCLUDE) -c $< -o $@

-include $(DEPS)

%.d: %.c
	@$(CC) $(CFLAGS) -MM $< -MF $@

$(OBJF):
	@$(MKDIR) -p $(OBJ_DIR)

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		$(RM) -r $(OBJ_DIR); \
		printf "$(YELLOW)Removing %d objects from \"%s\" folder.\n$(DEF_COLOR)" $(words $(OBJ)) $(OBJ_DIR);\
	fi
	
fclean: clean
	@if [ -e $(LIBFT) ]; then \
		printf "$(YELLOW)Removing \"%s\"...\n$(DEF_COLOR)" $(LIBFT);\
		$(RM) $(LIBFT);\
		$(MAKE) fclean -C $(LIBFT_DIR) -s;\
	fi
	@if [ -e $(LIBFT_INC_H) ]; then \
		printf "$(YELLOW)Removing \"%s\"...\n$(DEF_COLOR)" $(INC_DIR)/$(LIBFT_INC_H); \
		$(RM) $(LIBFT_INC_H);\
	fi
	@if [ -e $(NAME) ]; then \
		printf "$(YELLOW)Removing \"%s\"...\n$(DEF_COLOR)" $(NAME);\
		$(RM) -r $(NAME);\
	fi

re: fclean all

fcleanall: fcleanlibs
	@$(RM) -rf $(TMP)
	@printf "$(RED)All files removed!$(DEF_COLOR)\n"

.PHONY: all clean fclean re norminette  cleanlibs fcleanlibs relibft fcleanall