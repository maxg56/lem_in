#!/bin/bash

# Comprehensive tester for lem-in pathfinding algorithm
# Tests all generator options with multiple iterations

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
TIMEOUT_SECONDS=30
ITERATIONS_PER_TEST=5

# Generator options to test
GENERATOR_OPTIONS=(
    "--flow-one"
    "--flow-ten" 
    "--flow-thousand"
    "--big"
    "--big-superposition"
)

# Statistics
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  LEM-IN COMPREHENSIVE PATHFINDING TEST${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Function to run a single test
run_test() {
    local option=$1
    local iteration=$2
    local temp_map="temp_map_${option}_${iteration}.map"
    
    echo -e "${YELLOW}Testing ${option} (iteration ${iteration}/${ITERATIONS_PER_TEST})...${NC}"
    
    # Generate map
    timeout ${TIMEOUT_SECONDS} ./generator_linux ${option} > "${temp_map}" 2>/dev/null
    if [ $? -ne 0 ]; then
        echo -e "${RED}  ✗ Generator failed or timed out${NC}"
        rm -f "${temp_map}"
        return 1
    fi
    
    # Get ant count from first line
    local ant_count=$(head -1 "${temp_map}")
    if ! [[ "$ant_count" =~ ^[0-9]+$ ]]; then
        echo -e "${RED}  ✗ Invalid map format (ant count: ${ant_count})${NC}"
        rm -f "${temp_map}"
        return 1
    fi
    
    # Run our algorithm
    local start_time=$(date +%s.%N)
    timeout ${TIMEOUT_SECONDS} ./lem-in < "${temp_map}" > output.tmp 2>/dev/null
    local exit_code=$?
    local end_time=$(date +%s.%N)
    
    if [ $exit_code -ne 0 ]; then
        echo -e "${RED}  ✗ Algorithm failed or timed out${NC}"
        rm -f "${temp_map}" output.tmp
        return 1
    fi
    
    # Calculate execution time (fallback if bc not available)
    local duration="N/A"
    if command -v bc >/dev/null 2>&1; then
        duration=$(echo "$end_time - $start_time" | bc -l | xargs printf "%.3f")
    fi
    
    # Count output lines
    local output_lines=$(wc -l < output.tmp)
    
    # Get expected lines from generator comment
    local expected_lines=$(grep "#Here is the number of lines required:" "${temp_map}" | head -1 | cut -d':' -f2 | tr -d ' ')
    
    # Validate output format
    local valid_output=true
    if ! head -1 output.tmp | grep -qE '^L[0-9]+-[a-zA-Z0-9_]+'; then
        if [ "$output_lines" -gt 0 ]; then
            valid_output=false
        fi
    fi
    
    if [ "$valid_output" = true ]; then
        local improvement=""
        if [[ -n "$expected_lines" && "$expected_lines" =~ ^[0-9]+$ ]]; then
            if [ "$output_lines" -le "$expected_lines" ]; then
                local savings=$((expected_lines - output_lines))
                local percentage=$((savings * 100 / expected_lines))
                improvement=" (${percentage}% better than expected ${expected_lines})"
            else
                local excess=$((output_lines - expected_lines))
                improvement=" (${excess} lines over expected ${expected_lines})"
            fi
        fi
        echo -e "${GREEN}  ✓ Success: ${ant_count} ants, ${output_lines} lines, ${duration}s${improvement}${NC}"
        rm -f "${temp_map}" output.tmp
        return 0
    else
        echo -e "${RED}  ✗ Invalid output format${NC}"
        rm -f "${temp_map}" output.tmp
        return 1
    fi
}

# Main testing loop
for option in "${GENERATOR_OPTIONS[@]}"; do
    echo -e "\n${BLUE}Testing ${option}:${NC}"
    
    local option_passed=0
    local option_total=0
    
    for i in $(seq 1 $ITERATIONS_PER_TEST); do
        ((TOTAL_TESTS++))
        ((option_total++))
        
        if run_test "${option}" "${i}"; then
            ((PASSED_TESTS++))
            ((option_passed++))
        else
            ((FAILED_TESTS++))
        fi
    done
    
    # Calculate percentage for this option
    local percentage=0
    if [ $option_total -gt 0 ]; then
        percentage=$((option_passed * 100 / option_total))
    fi
    
    echo -e "${BLUE}  ${option} Summary: ${option_passed}/${option_total} (${percentage}%)${NC}"
done

# Final summary
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  FINAL RESULTS${NC}"
echo -e "${BLUE}========================================${NC}"

local success_percentage=0
if [ $TOTAL_TESTS -gt 0 ]; then
    success_percentage=$((PASSED_TESTS * 100 / TOTAL_TESTS))
fi

echo -e "Total tests: ${TOTAL_TESTS}"
echo -e "Passed: ${GREEN}${PASSED_TESTS}${NC}"
echo -e "Failed: ${RED}${FAILED_TESTS}${NC}"
echo -e "Success rate: ${GREEN}${success_percentage}%${NC}"

if [ $success_percentage -ge 80 ]; then
    echo -e "\n${GREEN}🎉 EXCELLENT PERFORMANCE!${NC}"
elif [ $success_percentage -ge 60 ]; then
    echo -e "\n${YELLOW}⚠️  GOOD PERFORMANCE, some room for improvement${NC}"
else
    echo -e "\n${RED}❌ NEEDS IMPROVEMENT${NC}"
fi

echo ""
