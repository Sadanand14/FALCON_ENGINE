import yaml
import os


UNITY_MATERIALS_MAP = {}
UNITY_MESHES_MAP = {}
UNITY_PREFAB_MAP = {}
model_extensions = ['FBX', 'OBJ']


def guid_mapper(file_path):

    for subdir, dirs, files in os.walk(file_path):
        for file in files:
            if ".mat" in file and file.endswith("meta"):
                # print(os.path.join(subdir, file))
                for line in open(os.path.join(subdir, file), 'r'):
                    if "guid" in line:  # TODO: replace with regex
                        guid = line.split(':')[1].strip()
                        material_file = file.split('.')
                        UNITY_MATERIALS_MAP[guid] = os.path.join(subdir, material_file[0] + "." + material_file[1])

            if any(x in file.upper() for x in model_extensions) and file.endswith("meta"):
                for line in open(os.path.join(subdir, file), 'r'):
                    if "guid" in line:  # TODO: replace with regex
                        guid = line.split(':')[1].strip()
                        mesh_file = file.split('.')
                        UNITY_MESHES_MAP[guid] = os.path.join(subdir, mesh_file[0] + "." + mesh_file[1])

            if ".prefab" in file and file.endswith("meta"):
                # print(os.path.join(subdir, file))
                for line in open(os.path.join(subdir, file), 'r'):
                    if "guid" in line:  # TODO: replace with regex
                        guid = line.split(':')[1].strip()
                        prefab_file = file.split('.')
                        UNITY_PREFAB_MAP[guid] = os.path.join(subdir, prefab_file[0] + "." + prefab_file[1])

    '''for key in UNITY_MATERIALS_MAP.keys():
        print(key + " : " + UNITY_MATERIALS_MAP[key])

    for key in UNITY_MESHES_MAP.keys():
        print(key + " : " + UNITY_MESHES_MAP[key])
    for key in UNITY_PREFAB_MAP.keys():
        print(key + " : " + UNITY_PREFAB_MAP[key])'''