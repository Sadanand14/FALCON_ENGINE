import os

###################################################################################################
# Directories to generate meta data about the assets
###################################################################################################
UNITY_MATERIALS_MAP = {}
UNITY_MESHES_MAP = {}
UNITY_PREFAB_MAP = {}

SCENE_FILE_ID_TO_INDEX = {}
SCENE_INDEX_TO_FILE_ID = {}
FILE_ID_SCENE_NAME = {}

###################################################################################################
# Unity to Falcon mapping constants
###################################################################################################


UNITY_COLLIDER_LIST = ['SphereCollider', 'BoxCollider', 'CapsuleCollider', 'MeshCollider', 'WheelCollider']

FALCON_SCALE_DOWN = 1 #0.01

FALCON_COLLIDERS = {
    'SphereCollider': 0,
    'BoxCollider': 1,
    'CapsuleCollider': 2,
    'MeshCollider': 3,
    'WheelCollider':4,
    'Other': 4
}

FALCON_RG_TYPES = {
    "Static": 0,
    "Dynamic": 1
}

MESH_INSTANCE_COUNTER = {}


OBJECTS_TO_EXCLUDE = ['Directional Light','Main Camera','Terrain','ReflectionProbe','ReflectionProbes']

#Defualt transform values
DEFAULT_POSITION = {'x':0, 'y': 0, 'z': 0}
DEFAULT_ROTATION = {'x': 0, 'y': 0, 'z': 0, 'w': 1}
DEFAULT_SCALE = {'x': 1, 'y': 1, 'z': 1}


###################################################################################################
# Falcon assets directory related constants
###################################################################################################


ASSETS_BASE_DIR = os.path.join(os.getcwd(), "..", "..", "Falcon", "Assets")
MODELS_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Models")
SCENES_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Scenes")
TEXTURES_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Textures")
ENTITIES_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Entity Templates")
SOUNDS_BASE_DIR = os.path.join(ASSETS_BASE_DIR, "Sounds")

# TODO: Get shader paths.
SHADERS = {
    "VShader": "Rendering/Shader/VertexShader.vert",
    "FShader": "Rendering/Shader/FragmentShader.frag",
    "FTransparent" : "Rendering/Shader/Transparent.frag"
}

BASIC_MODELS = {
    "CUBE": os.path.join(os.getcwd(),"Cube.fbx"),
    "SPHERE": os.path.join(os.getcwd(),"Sphere.obj"),
    "CAPSULE": os.path.join(os.getcwd(),"Capsule.obj"),
    "PLANE": os.path.join(os.getcwd(),"Plane.fbx"),
}
