/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/09 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"
#include <dirent.h>
#include <sys/stat.h>

#ifndef DT_REG
#define DT_REG 8
#endif

typedef struct s_test_case {
    char *filename;
    bool should_pass;
    char *description;
} t_test_case;

// Test case definitions
static t_test_case test_cases[] = {
    // Valid cases (should pass)
    {"test_valid.map", true, "Basic valid map"},
    {"test_complex_valid.map", true, "Complex valid map with comments"},
    
    // Error cases (should fail)
    {"test_error_zero_ants.map", false, "Zero ants error"},
    {"test_error_multiple_start.map", false, "Multiple start rooms error"},
    {"test_error_coordinates.map", false, "Invalid coordinates error"},
    {"test_error_duplicate_room.map", false, "Duplicate room name error"},
    {"test_error_insufficient_rooms.map", false, "Insufficient rooms error"},
    {"test_error_link_format.map", false, "Invalid link format error"},
    {"test_error_no_links.map", false, "No links error"},
    {"test_error_room_name.map", false, "Invalid room name error"},
    {"test_error_unknown_room.map", false, "Unknown room in link error"},
    {"test_negative_coords.map", false, "Negative coordinates error"},
    {"test_no_path.map", false, "No path between start and end error"},
    {"test_room_name_L.map", false, "Room name starting with L error"},
    {"test_same_coordinates.map", false, "Same coordinates error"},
    {"test_start_eof.map", false, "Start command at EOF error"},
    {"test_start_followed_by_end.map", false, "Start followed by end error"},
    {"test_start_followed_by_link.map", false, "Start followed by link error"},
    {"test_start_no_room.map", false, "Start with no room error"},
    {"test_start_not_followed.map", false, "Start not followed by room error"},
    {"test_unknown_room_link.map", false, "Unknown room in link error"},
    {"test_debug_hash_room.map", false, "Debug hash room error"},
    {NULL, false, NULL}
};

static bool file_exists(const char *filepath)
{
    struct stat buffer;
    return (stat(filepath, &buffer) == 0);
}

static void test_single_input_file(t_test_case *test_case)
{
    char filepath[300];
    char command[2048];  // Much larger buffer
    char output_file[300];
    int result;
    
    // Build full path to test file
    snprintf(filepath, sizeof(filepath), "tests_error_impout/%s", test_case->filename);
    snprintf(output_file, sizeof(output_file), "test_output_%s.txt", test_case->filename);
    
    // Check if test file exists
    if (!file_exists(filepath)) {
        printf("    " TEST_WARN " Skipping %s (file not found)\n", test_case->filename);
        return;
    }
    
    printf("    Testing: %s\n", test_case->description);
    printf("        File: %s\n", test_case->filename);
    
    // Execute lem-in with the test file
    snprintf(command, sizeof(command), "../lem-in < %s > %s 2>&1", filepath, output_file);
    result = system(command);
    
    bool test_passed = false;
    if (test_case->should_pass) {
        // For valid cases, expect success (exit code 0)
        test_passed = (result == 0);
        print_test_result("Valid input processed successfully", test_passed);
        
        if (test_passed) {
            // Additional validation for successful cases
            FILE *output = fopen(output_file, "r");
            if (output) {
                char line[512];
                bool has_ant_movements = false;
                
                while (fgets(line, sizeof(line), output)) {
                    if (strstr(line, "L1-") || strstr(line, "L2-") || strstr(line, "L3-")) {
                        has_ant_movements = true;
                        break;
                    }
                }
                
                print_test_result("Ant movements generated", has_ant_movements);
                fclose(output);
            }
        } else {
            // Show what error occurred for debugging
            FILE *output = fopen(output_file, "r");
            if (output) {
                printf("        " TEST_INFO " Error output: ");
                char line[256];
                if (fgets(line, sizeof(line), output)) {
                    // Remove newline and print first line of error
                    line[strcspn(line, "\n")] = 0;
                    printf("%s\n", line);
                }
                fclose(output);
            }
        }
    } else {
        // For error cases, expect failure (non-zero exit code)
        test_passed = (result != 0);
        print_test_result("Error case handled correctly", test_passed);
        
        if (test_passed) {
            // Show what error message was generated
            FILE *output = fopen(output_file, "r");
            if (output) {
                printf("        " TEST_INFO " Error message: ");
                char line[256];
                if (fgets(line, sizeof(line), output)) {
                    // Remove newline and print first line of error
                    line[strcspn(line, "\n")] = 0;
                    printf("%s\n", line);
                }
                fclose(output);
            }
        } else {
            printf("        " TEST_WARN " Expected error but got success (exit code: %d)\n", result);
        }
    }
    
    // Clean up output file
    unlink(output_file);
    printf("\n");
}

void test_input_file_validation(void)
{
    print_test_header("Input Files - Validation of test cases");
    
    int total_tests = 0;
    
    printf("    " TEST_INFO " Testing input files from tests_error_impout/\n\n");
    
    // Test each predefined case
    for (int i = 0; test_cases[i].filename != NULL; i++) {
        test_single_input_file(&test_cases[i]);
        total_tests++;
        // Note: We're not tracking individual pass/fail here for simplicity
        // The print_test_result function already shows the status
    }
    
    printf("    " TEST_INFO " Tested %d input files\n", total_tests);
}

void test_input_file_discovery(void)
{
    print_test_header("Input Files - Discovery and basic testing");
    
    DIR *dir;
    struct dirent *entry;
    char dirpath[] = "tests_error_impout";
    
    dir = opendir(dirpath);
    if (!dir) {
        print_test_result("Input directory accessible", false);
        printf("        " TEST_WARN " Could not open directory: %s\n", dirpath);
        return;
    }
    
    print_test_result("Input directory accessible", true);
    
    int file_count = 0;
    int map_files = 0;
    
    printf("    " TEST_INFO " Discovered input files:\n");
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Regular file
            file_count++;
            
            // Check if it's a .map file
            if (strstr(entry->d_name, ".map")) {
                map_files++;
                printf("        - %s\n", entry->d_name);
            }
        }
    }
    
    closedir(dir);
    
    printf("    " TEST_INFO " Found %d total files, %d map files\n", file_count, map_files);
    print_test_result("Map files found", map_files > 0);
    print_test_result("Expected number of test files", map_files >= 15); // We have at least 15 test cases
}

void test_input_categories(void)
{
    print_test_header("Input Files - Categorized testing");
    
    printf("    " TEST_INFO " Testing by categories:\n\n");
    
    // Valid cases
    printf("    📋 VALID INPUT CASES:\n");
    printf("    --------------------------------------------------\n");
    
    t_test_case valid_cases[] = {
        {"test_valid.map", true, "Basic valid configuration"},
        {"test_complex_valid.map", true, "Complex valid with comments"},
        {NULL, false, NULL}
    };
    
    for (int i = 0; valid_cases[i].filename != NULL; i++) {
        test_single_input_file(&valid_cases[i]);
    }
    
    // Error cases - parsing errors
    printf("    📋 PARSING ERROR CASES:\n");
    printf("    --------------------------------------------------\n");
    
    t_test_case parsing_errors[] = {
        {"test_error_zero_ants.map", false, "Zero ants"},
        {"test_error_multiple_start.map", false, "Multiple start rooms"},
        {"test_error_coordinates.map", false, "Invalid coordinates"},
        {"test_error_room_name.map", false, "Invalid room names"},
        {"test_room_name_L.map", false, "Room name starting with L"},
        {"test_start_no_room.map", false, "Start command without room"},
        {NULL, false, NULL}
    };
    
    for (int i = 0; parsing_errors[i].filename != NULL; i++) {
        test_single_input_file(&parsing_errors[i]);
    }
    
    // Error cases - link errors
    printf("    📋 LINK ERROR CASES:\n");
    printf("    --------------------------------------------------\n");
    
    t_test_case link_errors[] = {
        {"test_error_link_format.map", false, "Invalid link format"},
        {"test_error_no_links.map", false, "No links defined"},
        {"test_error_unknown_room.map", false, "Unknown room in link"},
        {"test_unknown_room_link.map", false, "Link to unknown room"},
        {NULL, false, NULL}
    };
    
    for (int i = 0; link_errors[i].filename != NULL; i++) {
        test_single_input_file(&link_errors[i]);
    }
    
    // Error cases - logical errors
    printf("    📋 LOGICAL ERROR CASES:\n");
    printf("    --------------------------------------------------\n");
    
    t_test_case logical_errors[] = {
        {"test_no_path.map", false, "No path between start and end"},
        {"test_error_insufficient_rooms.map", false, "Too few rooms"},
        {"test_error_duplicate_room.map", false, "Duplicate room names"},
        {"test_same_coordinates.map", false, "Same coordinates for rooms"},
        {NULL, false, NULL}
    };
    
    for (int i = 0; logical_errors[i].filename != NULL; i++) {
        test_single_input_file(&logical_errors[i]);
    }
}

void run_input_file_tests(void)
{
    printf("\n📁 INPUT FILE TESTS\n");
    printf("==================================\n");
    printf("Testing input files from tests_error_impout/...\n\n");
    
    test_input_file_discovery();
    test_input_file_validation();
    test_input_categories();
    
    printf("\n✅ Input File Tests Complete\n\n");
}
