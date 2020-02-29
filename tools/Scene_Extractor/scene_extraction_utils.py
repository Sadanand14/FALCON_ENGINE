import yaml
import sys
from shutil import copyfile
import os
import json
from Scene_Extractor.guid_mapper import *
from Scene_Extractor.file_id_generator import *
from Scene_Extractor.constants import *


def scale_for_falcon(unity_scale):
    scale = {'x': '', 'y': '', 'z': ''}

    for key in scale.keys():
        print(type(unity_scale[key]))
        scale[key] = float(unity_scale[key]) * FALCON_SCALE_DOWN
    return scale


def read_collider_data(falcon_collider_data, collider_type, unity_collider_data):
    if collider_type == UNITY_COLLIDER_LIST[0]:  # sphere:

        falcon_collider_data['radius'] = unity_collider_data[collider_type]['m_Radius']

    elif collider_type == UNITY_COLLIDER_LIST[1]:  # box:

        falcon_collider_data['half_sides'] = {float(unity_collider_data[collider_type]['m_Size']['x']) / 2.0,
                                              float(unity_collider_data[collider_type]['m_Size']['y']) / 2.0,
                                              float(unity_collider_data[collider_type]['m_Size']['z']) / 2.0, }

    elif collider_type == UNITY_COLLIDER_LIST[2]:  # capsule:
        falcon_collider_data['radius'] = unity_collider_data[collider_type]['m_Radius']
        falcon_collider_data['half_height'] = float(unity_collider_data[collider_type]['m_Height']) / 2.0

    return falcon_collider_data


def read_prefab_source(prefab_yaml_file):
    position = {'x': '', 'y': '', 'z': ''}
    rotation = {'x': '', 'y': '', 'z': '', 'w': ''}
    scale = {'x': '', 'y': '', 'z': ''}
    parent = ''
    collider_data = {}
    obj_mesh = ''

    collider_data['rigidbody'] = 0

    with open(prefab_yaml_file, "r") as file_desc:
        data = yaml.load_all(file_desc, Loader=yaml.Loader)
        for d in data:
            current_key = list(d.keys())[0]
            if current_key == 'Transform':
                position = d['Transform']['m_LocalPosition']
                rotation = d['Transform']['m_LocalRotation']
                scale = d['Transform']['m_LocalScale']

            elif current_key == 'MeshFilter':
                mesh_data = d['MeshFilter']['m_Mesh']
                if mesh_data['guid'] in UNITY_MESHES_MAP.keys():
                    obj_mesh = UNITY_MESHES_MAP[mesh_data['guid']]
                else:
                    obj_mesh = "Some default mesh. See collider for details"

            elif current_key == 'Material':
                pass  # we will do material shit here

            elif any(x == current_key for x in UNITY_COLLIDER_LIST):
                print(current_key)
                collider_data['type'] = FALCON_COLLIDERS[current_key]
                collider_data = read_collider_data(collider_data, current_key, d)

            elif current_key == 'Rigidbody':
                collider_data['rigidbody'] = 1

    return position, rotation, scale, collider_data, obj_mesh


def read_gameobject(doc_data,is_prefab_related = False):
    position = {'x': '', 'y': '', 'z': ''}
    rotation = {'x': '', 'y': '', 'z': '', 'w': ''}
    scale = {'x': '', 'y': '', 'z': ''}
    parent = str()
    collider_data = {}
    obj_mesh = ''
    mat = ''
    collider_data['rigidbody'] = 0
    return_data = {}
    for d in doc_data:
        current_key = list(d.keys())[0]
        if current_key == 'Transform':
            position = d['Transform']['m_LocalPosition']
            rotation = d['Transform']['m_LocalRotation']
            scale = d['Transform']['m_LocalScale']
            parent = d['Transform']['m_Father']['fileID']

        elif current_key == 'MeshFilter':
            mesh_data = d['MeshFilter']['m_Mesh']
            if mesh_data['guid'] in UNITY_MESHES_MAP.keys():
                obj_mesh = UNITY_MESHES_MAP[mesh_data['guid']]
            else:
                obj_mesh = "Some default mesh. See collider for details"

        elif current_key == 'MeshRenderer':
            mat = UNITY_MATERIALS_MAP[d['MeshRenderer']['m_Materials'][0]['guid']]

        elif any(x == current_key for x in UNITY_COLLIDER_LIST):
            print(current_key)
            collider_data['type'] = FALCON_COLLIDERS[current_key]
            collider_data = read_collider_data(collider_data, current_key, d)

        elif current_key == 'Rigidbody':
            collider_data['rigidbody'] = 1

    if not is_prefab_related:
        return_data['position'] = position
        return_data['scale'] = scale_for_falcon(scale)
        return_data['rotation'] = rotation
    return_data['parent'] = parent
    return_data['obj_mesh'] = obj_mesh
    return_data['collider_data'] = collider_data
    return_data['mat'] = mat
    return return_data


def read_prefabs(prefab, prefab_file, bool_read_prefab_source=True):
    collider_data = {}
    name = str()
    return_data = {}
    # read the soruce of prefab
    position = {'x': '', 'y': '', 'z': ''}
    rotation = {'x': '', 'y': '', 'z': '', 'w': ''}
    scale = {'x': '', 'y': '', 'z': ''}

    collider_data = {}
    obj_mesh = ''
    mat = ''
    local_modifications = prefab['m_Modification']
    parent = local_modifications['m_TransformParent']['fileID']  # get in more readable format

    local_modifications = prefab['m_Modification']['m_Modifications']
    prefab_yaml = "prefab.yaml"
    if bool_read_prefab_source is True:
        prefab_yaml = "prefab.yaml"
        copyfile(prefab_file, prefab_yaml)
        clean_file(prefab_yaml)

        # By default we make rigidbody static
        collider_data['rigidbody'] = 0
        position, rotation, scale, collider_data, obj_mesh = read_prefab_source(prefab_yaml)

    for d in local_modifications:
        if d['propertyPath'] == 'm_Name':
            name = d['value']

        elif d['propertyPath'] == 'm_LocalPosition.x':
            position['x'] = d['value']
        elif d['propertyPath'] == 'm_LocalPosition.y':
            position['y'] = d['value']
        elif d['propertyPath'] == 'm_LocalPosition.z':
            position['z'] = d['value']
        elif d['propertyPath'] == 'm_LocalRotation.x':
            rotation['x'] = d['value']
        elif d['propertyPath'] == 'm_LocalRotation.y':
            rotation['y'] = d['value']
        elif d['propertyPath'] == 'm_LocalRotation.z':
            rotation['z'] = d['value']
        elif d['propertyPath'] == 'm_LocalRotation.w':
            rotation['w'] = d['value']
        elif d['propertyPath'] == 'm_LocalScale.x':
            scale['x'] = d['value']
        elif d['propertyPath'] == 'm_LocalScale.y':
            scale['y'] = d['value']
        elif d['propertyPath'] == 'm_LocalScale.z':
            scale['z'] = d['value']
        elif d['propertyPath'] == 'm_Materials.Array.data[0]':
            mat = UNITY_MATERIALS_MAP[d['objectReference']['guid']]
            print(mat)

    return_data['name'] = name
    return_data['position'] = position
    return_data['scale'] = scale_for_falcon(scale)
    return_data['rotation'] = rotation
    return_data['parent'] = parent
    return_data['obj_mesh'] = obj_mesh
    return_data['mat'] = mat
    return_data['collider_data'] = collider_data
    if bool_read_prefab_source:
        os.remove(prefab_yaml)
    return return_data

