#!/bin/bash

# Lem-in Hexagonal Visualizer Setup Script

echo "🎯 Setting up Lem-in Hexagonal Visualizer..."

# Check if we're in the right directory
if [ ! -f "requirements.txt" ]; then
    echo "❌ Error: Please run this script from the visu-hex directory"
    exit 1
fi

# Create virtual environment if it doesn't exist
if [ ! -d "venv" ]; then
    echo "📦 Creating virtual environment..."
    python3 -m venv venv
fi

# Activate virtual environment
echo "🔌 Activating virtual environment..."
source venv/bin/activate

# Install dependencies
echo "📥 Installing dependencies..."
pip install -r requirements.txt
pip install flask

# Make scripts executable
chmod +x *.py

echo ""
echo "✅ Setup complete!"
echo ""
echo "🚀 Usage:"
echo "  1. Pygame Visualizer (Desktop):"
echo "     python3 hex_visualizer.py ../maps/map"
echo ""
echo "  2. Interactive Visualizer (Enhanced Desktop):"
echo "     python3 interactive_visualizer.py ../maps/map"
echo ""
echo "  3. Web Visualizer (Browser-based):"
echo "     python3 web_visualizer.py"
echo "     Then open http://localhost:5000"
echo ""
echo "🎮 Controls:"
echo "  • Mouse wheel: Zoom"
echo "  • Mouse drag: Pan"
echo "  • Space: Start simulation"
echo "  • R: Reset view"
echo "  • ESC/Q: Quit"
echo ""
echo "🗺️ Available maps in ../maps/:"
ls -la ../maps/ | grep -E '\.(map|txt)$' | awk '{print "  • " $9}'
echo ""
echo "Happy visualizing! 🎨"
