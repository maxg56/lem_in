#!/usr/bin/env python3
"""
Web-based Lem-in Visualizer
A modern web interface for visualizing lem-in pathfinding algorithms
"""

from flask import Flask
from api_routes import api_bp

# Créer l'application Flask
app = Flask(__name__)

# Enregistrer le Blueprint des routes API
app.register_blueprint(api_bp)


if __name__ == '__main__':
    import sys
    import argparse
    
    parser = argparse.ArgumentParser(description='Lem-in Hexagonal Visualizer Web Server')
    parser.add_argument('--port', '-p', type=int, default=5001, 
                       help='Port to run the server on (default: 5001)')
    
    args = parser.parse_args()
    port = args.port
    
    print("🚀 Starting Lem-in Hexagonal Visualizer Web Server...")
    print(f"🌐 Open your browser and go to: http://localhost:{port}")
    print("📊 Available endpoints:")
    print("  - GET  /                 : Main interface")
    print("  - POST /api/load_map     : Load a map file")
    print("  - POST /api/find_path    : Find paths using C executable")
    print("  - POST /api/simulate     : Run ant simulation")
    print("  - GET  /api/maps         : List available maps")
    print("  - POST /api/find_path_with_flags : Find paths with custom flags")
    print("  - POST /api/simulate_with_flags : Run simulation with custom flags")
    print()
    app.run(debug=True, host='0.0.0.0', port=port)
