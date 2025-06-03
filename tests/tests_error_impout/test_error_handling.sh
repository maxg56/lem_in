#!/bin/bash

# Comprehensive test script for lem-in error handling
# Tests all the error cases defined in the error enumeration

echo "🧪 Testing lem-in error handling system..."
echo "=========================================="

cd /home/maxence/Documents/lem_in-

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
total_tests=0
passed_tests=0

# Function to run a test
run_test() {
    local test_name="$1"
    local map_file="$2"
    local expected_error="$3"
    
    echo -n "Testing: $test_name... "
    total_tests=$((total_tests + 1))
    
    # Run the program and capture output
    output=$(./lem-in < "$map_file" 2>&1)
    exit_code=$?
    
    # Check if it should succeed or fail
    if [[ "$expected_error" == "SUCCESS" ]]; then
        if [[ $exit_code -eq 0 ]]; then
            echo -e "${GREEN}PASS${NC}"
            passed_tests=$((passed_tests + 1))
        else
            echo -e "${RED}FAIL${NC} - Expected success but got error: $output"
        fi
    else
        if [[ $exit_code -ne 0 ]] && [[ "$output" == *"$expected_error"* ]]; then
            echo -e "${GREEN}PASS${NC}"
            passed_tests=$((passed_tests + 1))
        else
            echo -e "${RED}FAIL${NC} - Expected '$expected_error' but got: $output"
        fi
    fi
}

# Make sure the project is compiled
echo "Compiling project..."
make > /dev/null 2>&1

echo
echo "Running error validation tests:"
echo "------------------------------"

# Test 1: Valid case
run_test "Valid map" "test_valid.map" "SUCCESS"

# Test 2: Zero ants
run_test "Zero ants" "test_error_zero_ants.map" "Nombre de fourmis ≤ 0"

# Test 3: Non-numeric ants
cat > test_non_numeric_ants.map << EOF
abc
##start
room1 0 0
##end
room2 1 1
room1-room2
EOF
run_test "Non-numeric ants" "test_non_numeric_ants.map" "Nombre de fourmis non numérique"

# Test 4: Room name with L
run_test "Room name with L" "test_room_name_L.map" "##start ou ##end non suivis d'une vraie salle"

# Test 5: Room name with #
cat > test_room_name_hash.map << EOF
3
##start
#room1 0 0
##end
room2 1 1
#room1-room2
EOF
run_test "Room name with #" "test_room_name_hash.map" "Moins de deux salles"

# Test 6: Float coordinates
run_test "Float coordinates" "test_float_coordinates.map" "##start ou ##end non suivis d'une vraie salle"

# Test 7: Duplicate room
run_test "Duplicate room" "test_error_duplicate_room.map" "Doublon de noms de salle"

# Test 8: Invalid link format
run_test "Invalid link format" "test_error_link_format.map" "Format de lien invalide"

# Test 9: Self link
run_test "Self link" "test_error_self_link.map" "Lien d'une salle vers elle-même"

# Test 10: Unknown room in link
run_test "Unknown room in link" "test_unknown_room_link.map" "Lien avec salle non déclarée"

# Test 11: Multiple ##start
run_test "Multiple ##start" "test_error_multiple_start.map" "Plusieurs ##start trouvés"

# Test 12: ##start not followed by room
run_test "##start not followed" "test_start_eof.map" "##start ou ##end non suivis d'une vraie salle"

# Test 13: ##start followed by link
run_test "##start followed by link" "test_start_followed_by_link.map" "##start ou ##end non suivis d'une vraie salle"

# Test 14: No links
run_test "No links" "test_error_no_links.map" "Pas de liens"

# Test 15: No path between start and end
run_test "No path start-end" "test_no_path.map" "Pas de chemin possible entre start et end"

# Test 16: Insufficient rooms
run_test "Insufficient rooms" "test_error_insufficient_rooms.map" "##start ou ##end non suivis d'une vraie salle"

# Clean up temporary files
rm -f test_non_numeric_ants.map test_room_name_hash.map

echo
echo "========================================"
echo "Test Results: $passed_tests/$total_tests passed"
if [[ $passed_tests -eq $total_tests ]]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
else
    echo -e "${RED}❌ Some tests failed.${NC}"
fi
