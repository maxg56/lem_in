#!/usr/bin/env python3
"""
Utilitaires pour le visualisateur Lem-in
"""

import os
from typing import List, Dict, Any


def format_room_data(rooms: Dict) -> List[Dict[str, Any]]:
    """Convertit les données des salles en format JSON-serializable"""
    rooms_data = []
    for room in rooms.values():
        rooms_data.append({
            'name': room.name,
            'x': room.x,
            'y': room.y,
            'is_start': room.is_start,
            'is_end': room.is_end
        })
    return rooms_data


def format_connections_data(connections: List[tuple]) -> List[Dict[str, str]]:
    """Convertit les données des connexions en format JSON-serializable"""
    return [{'from': conn[0], 'to': conn[1]} for conn in connections]


def validate_map_file(filename: str) -> bool:
    """Valide qu'un fichier de carte existe et est accessible"""
    if not filename:
        return False
    
    map_path = os.path.join('..', 'maps', filename)
    return os.path.exists(map_path) and os.path.isfile(map_path)


def get_available_maps() -> List[str]:
    """Récupère la liste des cartes disponibles dans le répertoire maps"""
    try:
        maps_dir = os.path.join('..', 'maps')
        if os.path.exists(maps_dir):
            all_files = os.listdir(maps_dir)
            # Filtrer pour ne garder que les fichiers (pas les dossiers)
            map_files = [f for f in all_files 
                        if os.path.isfile(os.path.join(maps_dir, f))]
            return map_files
        else:
            # Cartes par défaut si le répertoire n'existe pas
            return ['map', 'maps.map']
    except Exception:
        return ['map', 'maps.map']


def create_success_response(data: Any) -> Dict[str, Any]:
    """Crée une réponse de succès standardisée"""
    return {'success': True, 'data': data}


def create_error_response(error_message: str) -> Dict[str, Any]:
    """Crée une réponse d'erreur standardisée"""
    return {'success': False, 'error': error_message}


def build_executable_path() -> str:
    """Construit le chemin vers l'exécutable lem-in"""
    return "/home/maxence/Documents/lem_in-/lem-in"


def build_map_path(filename: str) -> str:
    """Construit le chemin complet vers un fichier de carte"""
    return os.path.join('..', 'maps', filename)
