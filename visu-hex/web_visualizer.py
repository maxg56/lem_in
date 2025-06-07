#!/usr/bin/env python3
"""
Web-based Lem-in Visualizer
A modern web interface for visualizing lem-in pathfinding algorithms
"""

from flask import Flask, render_template, jsonify, request, send_from_directory
import json
import os
import subprocess
import tempfile
import re
from Room import Room
from typing import Dict, List, Tuple, Optional

app = Flask(__name__)


class LemInCWrapper:
    """Wrapper pour utiliser l'exécutable lem-in en C"""
    
    def __init__(self, executable_path: str = "../lem-in"):
        self.executable_path = executable_path
        self.rooms = {}
        self.connections = []
        self.ant_count = 0
        self.start_room = None
        self.end_room = None
    
    def load_map(self, map_file_path: str) -> bool:
        """Charge une carte et parse les données"""
        try:
            with open(map_file_path, 'r') as f:
                content = f.read()
            
            # Parse le contenu pour extraire les données
            self._parse_map_content(content)
            return True
        except Exception as e:
            print(f"Erreur lors du chargement de la carte: {e}")
            return False
    
    def _parse_map_content(self, content: str):
        """Parse le contenu d'une carte lem-in"""
        lines = [line.strip() for line in content.split('\n') if line.strip()]
        
        if not lines:
            raise ValueError("Fichier vide")
        
        self.ant_count = int(lines[0])
        
        i = 1
        is_start_next = False
        is_end_next = False
        
        while i < len(lines):
            line = lines[i]
            
            if line.startswith('##start'):
                is_start_next = True
            elif line.startswith('##end'):
                is_end_next = True
            elif line.startswith('#'):
                pass
            elif '-' in line and not line.startswith('L'):
                # Connexion
                parts = line.split('-')
                if len(parts) == 2:
                    room1, room2 = parts[0].strip(), parts[1].strip()
                    if room1 and room2 and room1 != room2:
                        self.connections.append((room1, room2))
            else:
                # Salle
                parts = line.split()
                if len(parts) >= 3:
                    name = parts[0]
                    if name.startswith('L') or '-' in name:
                        i += 1
                        continue
                    try:
                        x, y = int(parts[1]), int(parts[2])
                        if name not in self.rooms:
                            room = Room(name, x, y, is_start_next, is_end_next)
                            self.rooms[name] = room
                            
                            if is_start_next:
                                self.start_room = name
                            elif is_end_next:
                                self.end_room = name
                            
                        is_start_next = False
                        is_end_next = False
                    except ValueError:
                        pass
            
            i += 1
    
    def find_path_with_executable(self, map_file_path: str) -> Optional[List[str]]:
        """Utilise l'exécutable C pour trouver un chemin"""
        try:
            # Exécute votre programme lem-in
            result = subprocess.run(
                [self.executable_path, map_file_path], 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            if result.returncode != 0:
                print(f"Erreur de l'exécutable: {result.stderr}")
                return None
            
            # Parse la sortie pour extraire le chemin
            output_lines = result.stdout.strip().split('\n')
            return self._parse_executable_output(output_lines)
            
        except subprocess.TimeoutExpired:
            print("Timeout lors de l'exécution de lem-in")
            return None
        except Exception as e:
            print(f"Erreur lors de l'exécution: {e}")
            return None
    
    def _parse_executable_output(self, output_lines: List[str]) -> Optional[List[str]]:
        """Parse la sortie de l'exécutable pour extraire le chemin"""
        try:
            if not output_lines:
                return None
            
            # Trouve la première ligne commençant par "L" (mouvement d'une fourmi)
            first_move_line = None
            for line in output_lines:
                if line.strip().startswith('L'):
                    first_move_line = line.strip()
                    break
            
            if not first_move_line:
                return None
            
            # Parse les mouvements pour reconstruire le chemin
            moves = re.findall(r'L\d+-(\w+)', first_move_line)
            
            if not moves:
                return None
            
            # Le chemin commence par start_room et inclut tous les mouvements
            path = [self.start_room] if self.start_room else []
            path.extend(moves)
            
            return path
            
        except Exception as e:
            print(f"Erreur lors du parsing de la sortie: {e}")
            return None
    
    def simulate_ants(self, map_file_path: str) -> Dict:
        """Simule le mouvement des fourmis en utilisant l'exécutable C"""
        try:
            # Exécute votre programme lem-in
            result = subprocess.run(
                [self.executable_path, map_file_path], 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            if result.returncode != 0:
                return {'success': False, 'error': f"Erreur de l'exécutable: {result.stderr}"}
            
            # Parse tous les mouvements
            output_lines = result.stdout.strip().split('\n')
            moves = self._parse_all_moves(output_lines)
            
            return {
                'success': True,
                'moves': moves,
                'total_turns': len(moves),
                'raw_output': result.stdout
            }
            
        except subprocess.TimeoutExpired:
            return {'success': False, 'error': "Timeout lors de l'exécution"}
        except Exception as e:
            return {'success': False, 'error': f"Erreur: {str(e)}"}
    
    def _parse_all_moves(self, output_lines: List[str]) -> List[List[Dict]]:
        """Parse tous les mouvements de toutes les fourmis"""
        moves = []
        
        for line in output_lines:
            line = line.strip()
            if line.startswith('L'):
                turn_moves = []
                # Parse chaque mouvement dans la ligne
                ant_moves = re.findall(r'L(\d+)-(\w+)', line)
                for ant_id, room_name in ant_moves:
                    turn_moves.append({
                        'ant_id': int(ant_id),
                        'to': room_name
                    })
                if turn_moves:
                    moves.append(turn_moves)
        
        return moves

# Global state
current_wrapper = None
current_map_path = None

@app.route('/')
def index():
    """Serve the main page"""
    return render_template('index.html')

@app.route('/api/load_map', methods=['POST'])
def load_map():
    """Load a map file"""
    global current_wrapper, current_map_path
    
    try:
        data = request.get_json()
        filename = data.get('filename')
        
        # For demo, load from maps directory
        map_path = os.path.join('..', 'maps', filename)
        
        # Utilise votre wrapper C
        wrapper = LemInCWrapper("/home/maxence/Documents/lem_in-/lem-in")
        
        if not wrapper.load_map(map_path):
            return jsonify({'success': False, 'error': 'Impossible de charger la carte'})
        
        # Convert to JSON-serializable format
        rooms_data = []
        for room in wrapper.rooms.values():
            rooms_data.append({
                'name': room.name,
                'x': room.x,
                'y': room.y,
                'is_start': room.is_start,
                'is_end': room.is_end
            })
        
        connections_data = [{'from': conn[0], 'to': conn[1]} for conn in wrapper.connections]
        
        current_wrapper = wrapper
        current_map_path = map_path
        
        return jsonify({
            'success': True,
            'data': {
                'ant_count': wrapper.ant_count,
                'rooms': rooms_data,
                'connections': connections_data
            }
        })
        
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/find_path', methods=['POST'])
def find_path():
    """Find path using C executable"""
    global current_wrapper, current_map_path
    
    if not current_wrapper or not current_map_path:
        return jsonify({'success': False, 'error': 'No map loaded'})
    
    try:
        # Utilise votre exécutable C pour trouver le chemin
        path = current_wrapper.find_path_with_executable(current_map_path)
        
        if not path:
            return jsonify({'success': False, 'error': 'Aucun chemin trouvé'})
        
        return jsonify({
            'success': True,
            'paths': [path]  # Retourne le chemin trouvé par votre C
        })
        
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/simulate', methods=['POST'])
def simulate():
    """Run simulation using C executable"""
    global current_wrapper, current_map_path
    
    if not current_wrapper or not current_map_path:
        return jsonify({'success': False, 'error': 'No map loaded'})
    
    try:
        # Utilise votre exécutable C pour la simulation complète
        result = current_wrapper.simulate_ants(current_map_path)
        
        if not result['success']:
            return jsonify(result)
        
        return jsonify({
            'success': True,
            'simulation': {
                'moves': result['moves'],
                'total_turns': result['total_turns'],
                'raw_output': result.get('raw_output', '')
            }
        })
        
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/maps')
def list_maps():
    """List available map files"""
    try:
        maps_dir = os.path.join('..', 'maps')
        if os.path.exists(maps_dir):
            list_dir = os.listdir(maps_dir)
            maps = []
            for f in list_dir:
                if os.path.isfile(os.path.join(maps_dir, f)) and f.endswith('.map'):
                    maps.append(f)
            
            maps = [f for f in os.listdir(maps_dir)]
            return jsonify({'success': True, 'maps': maps})
        else:
            return jsonify({'success': True, 'maps': ['map', 'maps.map']})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})


if __name__ == '__main__':
    import sys
    port = 5001 if len(sys.argv) <= 1 else int(sys.argv[1])
    print("🚀 Starting Lem-in Hexagonal Visualizer Web Server...")
    print(f"🌐 Open your browser and go to: http://localhost:{port}")
    print("📊 Available endpoints:")
    print("  - GET  /                 : Main interface")
    print("  - POST /api/load_map     : Load a map file")
    print("  - POST /api/find_paths   : Find paths using algorithm")
    print("  - POST /api/simulate     : Run ant simulation")
    print("  - GET  /api/status       : Get server status")
    print()
    app.run(debug=True, host='0.0.0.0', port=port)
