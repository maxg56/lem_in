/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgendrot <mgendrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:00:00 by mgendrot          #+#    #+#             */
/*   Updated: 2025/06/02 00:00:00 by mgendrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_lem_in.h"

static int test_count = 0;
static int pass_count = 0;

void print_test_header(const char *test_name)
{
    printf("\n📋 Testing %s:\n", test_name);
    printf("--------------------------------------------------\n");
}

void print_test_result(const char *test_case, int passed)
{
    test_count++;
    if (passed) {
        pass_count++;
        printf("  %s %s\n", TEST_PASS, test_case);
    } else {
        printf("  %s %s\n", TEST_FAIL, test_case);
    }
}

int create_test_map_file(const char *filename, const char *content)
{
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
        return 0;
    
    int len = strlen(content);
    int written = write(fd, content, len);
    close(fd);
    
    return (written == len);
}

int compare_output_files(const char *expected, const char *actual)
{
    int fd1 = open(expected, O_RDONLY);
    int fd2 = open(actual, O_RDONLY);
    
    if (fd1 < 0 || fd2 < 0) {
        if (fd1 >= 0) close(fd1);
        if (fd2 >= 0) close(fd2);
        return 0;
    }
    
    char buf1[1024], buf2[1024];
    int ret1, ret2;
    int match = 1;
    
    while ((ret1 = read(fd1, buf1, 1024)) > 0 && 
           (ret2 = read(fd2, buf2, 1024)) > 0) {
        if (ret1 != ret2 || memcmp(buf1, buf2, ret1) != 0) {
            match = 0;
            break;
        }
    }
    
    // Check if both files reached EOF
    if (ret1 != ret2) {
        match = 0;
    }
    
    close(fd1);
    close(fd2);
    return match;
}

void cleanup_test_files(void)
{
    // Remove test files
    unlink("test_map_simple.txt");
    unlink("test_map_complex.txt");
    unlink("test_map_invalid.txt");
    unlink("test_output.txt");
    unlink("expected_output.txt");
}

void print_final_results(void)
{
    printf("\n==================================================\n");
    printf("🏁 FINAL TEST RESULTS\n");
    printf("==================================================\n");
    
    if (pass_count == test_count) {
        printf(TEST_PASS " ALL TESTS PASSED! (%d/%d)\n", pass_count, test_count);
        printf("🎉 Congratulations! Your lem-in implementation is working perfectly!\n");
    } else {
        printf(TEST_WARN " Tests passed: %d/%d (%.1f%%)\n", 
               pass_count, test_count, 
               (float)pass_count / test_count * 100);
        printf("🔧 %d test(s) need attention.\n", test_count - pass_count);
    }
    
    printf("==================================================\n");
}
