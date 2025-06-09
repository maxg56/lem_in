#!/bin/bash

# Performance evaluator with average calculation
# Either achieves 100% success or calculates performance average

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo -e "${BLUE}🎯 LEM-IN PERFORMANCE EVALUATOR${NC}"
echo -e "${BLUE}================================${NC}"
echo "Calculating performance: 100% success OR average between expected/actual"
echo ""

# Arrays to store results
declare -a TEST_NAMES=("flow-one" "flow-ten" "flow-thousand" "big" "big-superposition")
declare -a EXPECTED_RESULTS=()
declare -a ACTUAL_RESULTS=()
declare -a PERFORMANCE_SCORES=()

test_and_calculate() {
    local option=$1
    local description=$2
    local timeout_val=$3
    
    echo -e "${YELLOW}Testing ${option}${NC} (${description})"
    
    # Change to project root directory
    cd "$PROJECT_ROOT" || return 1
    
    # Generate map
    if ! timeout ${timeout_val} ./scripts/generator_linux ${option} > "temp_${option}.map" 2>/dev/null; then
        echo -e "${RED}  ❌ Generator failed${NC}"
        return 1
    fi
    
    # Get expected and actual results
    local expected=$(grep "#Here is the number of lines required:" "temp_${option}.map" | head -1 | cut -d':' -f2 | tr -d ' ')
    local actual=$(timeout ${timeout_val} ./lem-in < "temp_${option}.map" 2>/dev/null | wc -l)
    
    if [[ -z "$expected" || -z "$actual" || ! "$expected" =~ ^[0-9]+$ || ! "$actual" =~ ^[0-9]+$ ]]; then
        echo -e "${RED}  ❌ Invalid results${NC}"
        rm -f "temp_${option}.map"
        return 1
    fi
    
    # Store results
    EXPECTED_RESULTS+=($expected)
    ACTUAL_RESULTS+=($actual)
    
    # Calculate performance score
    local score
    if [ "$actual" -lt "$expected" ]; then
        # Better than expected = 100% + improvement percentage
        local improvement=$((expected - actual))
        local improvement_percent=$((improvement * 100 / expected))
        score=$((100 + improvement_percent))
        if [ $score -gt 200 ]; then score=200; fi  # Cap at 200%
        echo -e "${GREEN}  ✅ EXCELLENT: ${actual}/${expected} lines (${score}% performance)${NC}"
    elif [ "$actual" -eq "$expected" ]; then
        score=100
        echo -e "${GREEN}  ✅ PERFECT: ${actual}/${expected} lines (100% performance)${NC}"
    else
        # Worse than expected = percentage efficiency (expected/actual * 100)
        score=$((expected * 100 / actual))
        echo -e "${YELLOW}  ⚠️  SUBOPTIMAL: ${actual}/${expected} lines (${score}% performance)${NC}"
    fi
    
    PERFORMANCE_SCORES+=($score)
    rm -f "temp_${option}.map"
    echo ""
    return 0
}

# Test each generator type
echo -e "${BLUE}Running performance evaluation...${NC}"
echo ""

successful_tests=0
total_tests=0

for i in "${!TEST_NAMES[@]}"; do
    case "${TEST_NAMES[$i]}" in
        "flow-one") test_and_calculate "--flow-one" "1 ant with distinctive path" 5 ;;
        "flow-ten") test_and_calculate "--flow-ten" "~10 ants with distinctive path" 5 ;;
        "flow-thousand") test_and_calculate "--flow-thousand" "~100 ants with distinctive path" 5 ;;
        "big") test_and_calculate "--big" "~4000 rooms for time complexity" 5 ;;
        "big-superposition") test_and_calculate "--big-superposition" "big map with overlapping paths" 10 ;;
    esac
    
    if [ $? -eq 0 ]; then
        ((successful_tests++))
    fi
    ((total_tests++))
done

# Calculate overall performance
echo -e "${BLUE}📊 PERFORMANCE ANALYSIS${NC}"
echo -e "${BLUE}========================${NC}"

if [ $successful_tests -eq $total_tests ]; then
    # All tests passed, check if 100% or calculate average
    all_perfect=true
    total_score=0
    
    for score in "${PERFORMANCE_SCORES[@]}"; do
        total_score=$((total_score + score))
        if [ $score -ne 100 ]; then
            all_perfect=false
        fi
    done
    
    if [ "$all_perfect" = true ]; then
        echo -e "${GREEN}🎉 100% SUCCESS! All tests achieved perfect optimization!${NC}"
        echo ""
        echo -e "${GREEN}VALIDATION: ✅ 100% RÉUSSITE${NC}"
    else
        # Calculate average performance
        average_score=$((total_score / successful_tests))
        echo -e "${BLUE}Average Performance: ${average_score}%${NC}"
        echo ""
        echo -e "${BLUE}Detailed Results:${NC}"
        printf "%-20s %-12s %-12s %-15s\n" "Test" "Expected" "Actual" "Performance"
        echo "=================================================================="
        
        for i in "${!TEST_NAMES[@]}"; do
            if [ $i -lt ${#EXPECTED_RESULTS[@]} ]; then
                printf "%-20s %-12s %-12s %-15s%%\n" \
                    "${TEST_NAMES[$i]}" \
                    "${EXPECTED_RESULTS[$i]}" \
                    "${ACTUAL_RESULTS[$i]}" \
                    "${PERFORMANCE_SCORES[$i]}"
            fi
        done
        
        echo ""
        if [ $average_score -ge 95 ]; then
            echo -e "${GREEN}🏆 EXCELLENT PERFORMANCE: ${average_score}% average${NC}"
            echo -e "${GREEN}VALIDATION: ✅ PERFORMANCE EXCELLENTE (${average_score}%)${NC}"
        elif [ $average_score -ge 85 ]; then
            echo -e "${YELLOW}⭐ GOOD PERFORMANCE: ${average_score}% average${NC}"
            echo -e "${YELLOW}VALIDATION: ✅ BONNE PERFORMANCE (${average_score}%)${NC}"
        else
            echo -e "${RED}⚠️  NEEDS IMPROVEMENT: ${average_score}% average${NC}"
            echo -e "${RED}VALIDATION: ❌ NÉCESSITE AMÉLIORATION (${average_score}%)${NC}"
        fi
    fi
else
    echo -e "${RED}❌ Some tests failed. Cannot calculate reliable average.${NC}"
    echo -e "${RED}VALIDATION: ❌ ÉCHECS DE TESTS${NC}"
fi

echo ""
