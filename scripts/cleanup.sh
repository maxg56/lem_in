#!/bin/bash

# Cleanup script for lem-in test files
# Removes temporary files created by test scripts

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Change to project root
cd "$PROJECT_ROOT" || exit 1

echo "🧹 Cleaning up temporary test files..."

# Remove temporary map files
rm -f temp_*.map
rm -f ts
rm -f output.tmp
rm -f result_*.tmp

# Clean debug files
rm -f debug_files/*.map 2>/dev/null

echo "✅ Cleanup complete!"
