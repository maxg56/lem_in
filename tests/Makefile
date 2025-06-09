# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#              #
#    Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Test Variables
TEST_NAME = lem_in_tests

# Directories
TEST_DIR = tests
INCLUDE_DIR = ../include
LIBFT_DIR = ../libft
SRC_DIR = ../src
TMP		= ../.tmp/

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include

# Libraries
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

# Test source files
TEST_SRCS = test_main.c \
            test_utils.c \
            test_graph.c \
            test_parsing.c \
            test_algorithms.c \
            test_integration.c \
            test_edge_management.c \
            test_search_navigation.c \
            test_validation.c \
            test_input_files.c

# Source files to test (your actual implementation)
SRC_FILES = $(shell find ../src -iname "*.c" ! -name "main.c")

# Object files
OBJ_DIR = $(TMP)obj/
TEST_OBJS = $(patsubst %.c, $(OBJ_DIR)%.o, $(TEST_SRCS))
SRC_OBJS = $(patsubst %.c, $(OBJ_DIR)%.o, $(SRC_FILES))

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
DEF_COLOR = \033[0m

# Rules
all: libft $(TEST_NAME)

$(TEST_NAME): $(TEST_OBJS) $(SRC_OBJS)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(SRC_OBJS) $(LIBFT_FLAGS) -o $(TEST_NAME)
	@printf "$(GREEN)Test suite compiled successfully!$(DEF_COLOR)\n"

$(OBJ_DIR)%.o: %.c | $(OBJF)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJF):
	@mkdir -p $(OBJ_DIR)


libft:
	@make -C $(LIBFT_DIR)

# Test execution targets
test: all
	@printf "$(YELLOW)Running all tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME)

test-graph: all
	@printf "$(YELLOW)Running graph tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) graph

test-edge: all
	@printf "$(YELLOW)Running edge management tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) edge

test-search: all
	@printf "$(YELLOW)Running search & navigation tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) search

test-validation: all
	@printf "$(YELLOW)Running validation tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) validation

test-parsing: all
	@printf "$(YELLOW)Running parsing tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) parsing

test-algorithms: all
	@printf "$(YELLOW)Running algorithm tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) algorithms

test-input: all
	@printf "$(YELLOW)Running input file tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) input

test-integration: all
	@printf "$(YELLOW)Running integration tests...$(DEF_COLOR)\n"
	@./$(TEST_NAME) integration

# Debug target for minimal testing
debug: libft
	gcc $(CFLAGS) -I $(INCLUDE_DIR) -I $(LIBFT_DIR)/include \
		$(SRC_FILES) ../debug_test.c -L $(LIBFT_DIR) -lft -o debug_test

# Cleanup
clean:
	@if [ -d $(OBJ_DIR) ]; then \
		rm -rf $(OBJ_DIR); \
		printf "$(YELLOW)Test object files cleaned.$(DEF_COLOR)\n"; \
	fi

fclean: clean
	@rm -f $(TEST_NAME)
	@rm -f test_*.txt
	@if [ -d $(TMP) ]; then \
		rm -rf $(TMP); \
	fi
	@printf "$(YELLOW)Test executable and test files cleaned.$(DEF_COLOR)\n"

fclean-all: fclean
	@make fclean -C $(LIBFT_DIR)

re: fclean all

# Help
help:
	@printf "$(GREEN)Available targets:$(DEF_COLOR)\n"
	@printf "  all               - Build test suite\n"
	@printf "  test              - Run all tests\n"
	@printf "  test-graph        - Run only graph tests\n"
	@printf "  test-edge         - Run only edge management tests\n"
	@printf "  test-search       - Run only search & navigation tests\n"
	@printf "  test-validation   - Run only validation tests\n"
	@printf "  test-parsing      - Run only parsing tests\n"
	@printf "  test-algorithms   - Run only algorithm tests\n"
	@printf "  test-integration  - Run only integration tests\n"
	@printf "  clean             - Remove object files\n"
	@printf "  fclean            - Remove all generated files\n"
	@printf "  fclean-all        - Clean everything including libft\n"
	@printf "  re                - Rebuild everything\n"
	@printf "  help              - Show this help\n"

.PHONY: all test test-graph test-edge test-search test-validation test-parsing test-algorithms test-integration clean fclean fclean-all re help libft debug
