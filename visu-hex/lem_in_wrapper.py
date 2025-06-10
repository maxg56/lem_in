#!/usr/bin/env python3
"""
LemInCWrapper - Wrapper pour utiliser l'exécutable lem-in en C
"""

import subprocess
import re
from typing import Dict, List, Optional
from Room import Room


class LemInCWrapper:
    """Wrapper pour utiliser l'exécutable lem-in en C avec support des flags"""
    
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
    
    def find_path_with_executable(self, map_file_path: str, flags: List[str] = None) -> Optional[List[List[str]]]:
        """Utilise l'exécutable C pour trouver les chemins avec flags optionnels"""
        try:
            # Construit la commande avec les flags - toujours inclure -p pour avoir les chemins
            command = [self.executable_path, '-p']
            if flags:
                # Ajouter les flags supplémentaires s'ils ne sont pas déjà -p
                for flag in flags:
                    if flag != '-p':
                        command.append(flag)
            command.append(map_file_path)
            
            # Exécute votre programme lem-in
            result = subprocess.run(
                command, 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            if result.returncode != 0:
                print(f"Erreur de l'exécutable: {result.stderr}")
                return None
            
            # Parse la sortie pour extraire les chemins
            output_lines = result.stdout.strip().split('\n')
            return self._parse_paths_from_output(output_lines)
            
        except subprocess.TimeoutExpired:
            print("Timeout lors de l'exécution de lem-in")
            return None
        except Exception as e:
            print(f"Erreur lors de l'exécution: {e}")
            return None
    
    def _parse_paths_from_output(self, output_lines: List[str]) -> Optional[List[List[str]]]:
        """Parse la sortie de l'exécutable pour extraire tous les chemins"""
        try:
            paths = []
            
            for line in output_lines:
                line = line.strip()
                # Recherche les lignes qui contiennent des chemins (format: room -> room -> room)
                if '->' in line and not line.startswith('[') and not line.startswith('L'):
                    # Parse le chemin: "2 -> 3 -> 1" devient ["2", "3", "1"]
                    path_rooms = [room.strip() for room in line.split('->')]
                    if len(path_rooms) > 1:  # Assure qu'il y a au moins 2 salles
                        paths.append(path_rooms)
            
            # Si aucun chemin trouvé avec le format ->, essaie de reconstruire depuis les mouvements
            if not paths:
                return self._reconstruct_paths_from_moves(output_lines)
            
            return paths if paths else None
            
        except Exception as e:
            print(f"Erreur lors du parsing des chemins: {e}")
            return None
    
    def _reconstruct_paths_from_moves(self, output_lines: List[str]) -> Optional[List[List[str]]]:
        """Reconstruit les chemins à partir des mouvements de fourmis"""
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
            
            return [path] if path else None
            
        except Exception as e:
            print(f"Erreur lors de la reconstruction des chemins: {e}")
            return None
    
    def simulate_ants(self, map_file_path: str, flags: List[str] = None) -> Dict:
        """Simule le mouvement des fourmis en utilisant l'exécutable C avec flags optionnels"""
        try:
            # Construit la commande avec les flags
            command = [self.executable_path]
            if flags:
                command.extend(flags)
            command.append(map_file_path)
            
            # Exécute votre programme lem-in
            result = subprocess.run(
                command, 
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
    
    def get_debug_info(self, map_file_path: str) -> Dict:
        """Récupère les informations de debug avec le flag -d"""
        try:
            result = subprocess.run(
                [self.executable_path, '-d', map_file_path], 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            return {
                'success': result.returncode == 0,
                'output': result.stdout,
                'error': result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {'success': False, 'error': str(e)}
    
    def get_statistics(self, map_file_path: str) -> Dict:
        """Récupère les statistiques avec le flag -s"""
        try:
            result = subprocess.run(
                [self.executable_path, '-s', map_file_path], 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            return {
                'success': result.returncode == 0,
                'output': result.stdout,
                'error': result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {'success': False, 'error': str(e)}
    
    def get_benchmark(self, map_file_path: str) -> Dict:
        """Récupère les informations de benchmark avec le flag -b"""
        try:
            result = subprocess.run(
                [self.executable_path, '-b', map_file_path], 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            return {
                'success': result.returncode == 0,
                'output': result.stdout,
                'error': result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {'success': False, 'error': str(e)}
    
    def get_paths_info(self, map_file_path: str) -> Dict:
        """Récupère les informations sur les chemins avec le flag -p"""
        try:
            result = subprocess.run(
                [self.executable_path, '-p', map_file_path], 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            return {
                'success': result.returncode == 0,
                'output': result.stdout,
                'error': result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {'success': False, 'error': str(e)}
    
    def get_verbose_output(self, map_file_path: str) -> Dict:
        """Récupère la sortie verbose avec le flag -v"""
        try:
            result = subprocess.run(
                [self.executable_path, '-v', map_file_path], 
                capture_output=True, 
                text=True, 
                timeout=30
            )
            
            return {
                'success': result.returncode == 0,
                'output': result.stdout,
                'error': result.stderr if result.returncode != 0 else None
            }
        except Exception as e:
            return {'success': False, 'error': str(e)}
    
    def _parse_all_moves(self, output_lines: List[str]) -> List[Dict]:
        """Parse tous les mouvements des fourmis depuis la sortie de l'exécutable C"""
        moves = []
        
        for line in output_lines:
            line = line.strip()
            if not line:
                continue
            
            # Skip lines that don't contain ant moves (paths, room definitions, etc.)
            # Only process lines that contain moves (L{num}-{room})
            if not any(part.startswith('L') and '-' in part for part in line.split()):
                continue
            
            # Skip the path display lines (those with ->)
            if '->' in line:
                continue
            
            # Parse each turn/line of moves
            turn_moves = {}
            parts = line.split()
            
            for part in parts:
                if '-' in part and part.startswith('L'):
                    try:
                        ant_part, room = part.split('-', 1)
                        ant_number = int(ant_part[1:])  # Remove 'L' prefix
                        turn_moves[ant_number] = room
                    except (ValueError, IndexError):
                        continue  # Skip malformed moves
            
            if turn_moves:  # Only add if we found valid moves
                moves.append(turn_moves)
        
        return moves
