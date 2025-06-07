class Room:
    """Web-compatible Room class"""
    def __init__(self, name: str, x: int, y: int, is_start: bool = False, is_end: bool = False):
        self.name = name
        self.x = x
        self.y = y
        self.is_start = is_start
        self.is_end = is_end
        self.connections = set()
