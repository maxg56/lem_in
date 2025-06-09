#!/usr/bin/env python3
"""
Configuration et état global pour le visualisateur Lem-in
"""

from lem_in_wrapper import LemInCWrapper

# État global
current_wrapper = None
current_map_path = None

def get_current_wrapper():
    """Récupère le wrapper actuel"""
    return current_wrapper

def get_current_map_path():
    """Récupère le chemin de la carte actuelle"""
    return current_map_path

def set_current_wrapper(wrapper: LemInCWrapper):
    """Définit le wrapper actuel"""
    global current_wrapper
    current_wrapper = wrapper

def set_current_map_path(path: str):
    """Définit le chemin de la carte actuelle"""
    global current_map_path
    current_map_path = path

def reset_state():
    """Remet à zéro l'état global"""
    global current_wrapper, current_map_path
    current_wrapper = None
    current_map_path = None

def is_ready():
    """Vérifie si le système est prêt (carte chargée)"""
    return current_wrapper is not None and current_map_path is not None
