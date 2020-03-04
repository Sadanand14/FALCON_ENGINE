import os

###################################################################################################
# Directories to generate meta data about the assets
###################################################################################################
UNITY_MATERIALS_MAP = {}
UNITY_MESHES_MAP = {}
UNITY_PREFAB_MAP = {}

SCENE_FILE_ID_TO_INDEX = {}
SCENE_INDEX_TO_FILE_ID = {}
FILE_ID_SCENE_NAME ={}

###################################################################################################
# Unity to Falcon mapping constants
###################################################################################################


UNITY_COLLIDER_LIST = ['SphereCollider', 'BoxCollider', 'CapsuleCollider','MeshCollider', 'WheelCollider']

FALCON_SCALE_DOWN = 0.01

FALCON_COLLIDERS = {
    'SphereCollider': 0,
    'BoxCollider': 1,
    'CapsuleCollider': 2,
    'MeshCollider': 3,
    'Other': 4
}

FALCON_RG_TYPES = {
    "Static": 0,
    "Dynamic": 1
}

###################################################################################################
# Falcon assets directory related constants
###################################################################################################


ASSETS_BASE_DIR =  os.path.join(os.getcwd(),"Assets")
MODELS_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Models")
SCENES_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Scenes")
TEXTURES_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Textures")
ENTITIES_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Entity Templates")

# TODO: Get shader paths. Currently we just 2.

