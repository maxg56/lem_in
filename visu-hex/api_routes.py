#!/usr/bin/env python3
"""
Routes API pour le visualisateur Lem-in
"""

from flask import Blueprint, render_template, jsonify, request
from lem_in_wrapper import LemInCWrapper
import config
import utils 

# Créer un Blueprint pour les routes API
api_bp = Blueprint('api', __name__)

@api_bp.route('/')
def index():
    """Serve the main page"""
    return render_template('index.html')

@api_bp.route('/api/load_map', methods=['POST'])
def load_map():
    """Load a map file"""
    try:
        data = request.get_json()
        filename = data.get('filename')
        
        if not utils.validate_map_file(filename):
            return jsonify(utils.create_error_response('Fichier de carte invalide'))
        
        # Utilise votre wrapper C
        wrapper = LemInCWrapper(utils.build_executable_path())
        map_path = utils.build_map_path(filename)
        
        if not wrapper.load_map(map_path):
            return jsonify(utils.create_error_response('Impossible de charger la carte'))
        
        # Convert to JSON-serializable format
        rooms_data = utils.format_room_data(wrapper.rooms)
        connections_data = utils.format_connections_data(wrapper.connections)
        
        config.set_current_wrapper(wrapper)
        config.set_current_map_path(map_path)
        
        return jsonify(utils.create_success_response({
            'ant_count': wrapper.ant_count,
            'rooms': rooms_data,
            'connections': connections_data
        }))
        
    except Exception as e:
        return jsonify(utils.create_error_response(str(e)))

@api_bp.route('/api/find_path', methods=['POST'])
def find_path():
    """Find path using C executable"""
    if not config.is_ready():
        return jsonify(utils.create_error_response('No map loaded'))
    
    try:
        # Utilise votre exécutable C pour trouver les chemins
        paths = config.get_current_wrapper().find_path_with_executable(config.get_current_map_path())
        
        if not paths:
            return jsonify(utils.create_error_response('Aucun chemin trouvé'))
        
        return jsonify(utils.create_success_response({'paths': paths}))
        
    except Exception as e:
        return jsonify(utils.create_error_response(str(e)))

@api_bp.route('/api/simulate', methods=['POST'])
def simulate():
    """Run simulation using C executable"""
    if not config.is_ready():
        return jsonify(utils.create_error_response('No map loaded'))
    
    try:
        # Utilise votre exécutable C pour la simulation complète
        result = config.get_current_wrapper().simulate_ants(config.get_current_map_path())
        
        if not result['success']:
            return jsonify(result)
        
        return jsonify(utils.create_success_response({
            'simulation': {
                'moves': result['moves'],
                'total_turns': result['total_turns'],
                'raw_output': result.get('raw_output', '')
            }
        }))
        
    except Exception as e:
        return jsonify(utils.create_error_response(str(e)))

@api_bp.route('/api/maps')
def list_maps():
    """List available map files"""
    try:
        maps = utils.get_available_maps()
        if not maps:
            return jsonify(utils.create_error_response('No maps available'))
        return jsonify(utils.create_success_response({'maps': maps}))
    except Exception as e:
        return jsonify(utils.create_error_response(str(e)))

# Routes avec flags personnalisés (optionnelles mais utiles)
@api_bp.route('/api/find_path_with_flags', methods=['POST'])
def find_path_with_flags():
    """Find path using C executable with custom flags"""
    if not config.is_ready():
        return jsonify(utils.create_error_response('No map loaded'))
    
    try:
        data = request.get_json()
        flags = data.get('flags', [])
        
        paths = config.get_current_wrapper().find_path_with_executable(config.get_current_map_path(), flags)
        
        if not paths:
            return jsonify(utils.create_error_response('Aucun chemin trouvé'))
        
        return jsonify(utils.create_success_response({
            'paths': paths,
            'flags_used': flags
        }))
        
    except Exception as e:
        return jsonify(utils.create_error_response(str(e)))

@api_bp.route('/api/simulate_with_flags', methods=['POST'])
def simulate_with_flags():
    """Run simulation using C executable with custom flags"""
    if not config.is_ready():
        return jsonify(utils.create_error_response('No map loaded'))
    
    try:
        data = request.get_json()
        flags = data.get('flags', [])
        
        result = config.get_current_wrapper().simulate_ants(config.get_current_map_path(), flags)
        
        if not result['success']:
            return jsonify(result)
        
        return jsonify(utils.create_success_response({
            'simulation': {
                'moves': result['moves'],
                'total_turns': result['total_turns'],
                'raw_output': result.get('raw_output', ''),
                'flags_used': flags
            }
        }))
        
    except Exception as e:
        return jsonify(utils.create_error_response(str(e)))
