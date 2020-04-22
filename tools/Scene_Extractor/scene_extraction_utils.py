import yaml
import sys
from shutil import copyfile
import os
import json
from Scene_Extractor.guid_mapper import *
from Scene_Extractor.file_id_generator import *
from Scene_Extractor.constants import *


def clean_file(filepath):
    file_desc = open(filepath, "r")
    result = str()
    for lineNumber, line in enumerate(file_desc.readlines()):
        if lineNumber == 1:
            continue
        if line.startswith('--- !u!'):
            result += '\n--- ' + line.split(' ')[2] + '\n'
        else:
            result += line
    file_desc.close()

    # write cleaned file
    file_desc = open(filepath, 'w')
    file_desc.write(result)
    file_desc.close()


def update_mesh_instance_count(obj_mesh):
    tmp = obj_mesh.split('\\')[-1]
    if tmp in MESH_INSTANCE_COUNTER.keys():
        MESH_INSTANCE_COUNTER[tmp] += 1
    else:
        MESH_INSTANCE_COUNTER[tmp] = 1


def get_parent_name(data, file_id):
    if file_id == '' or file_id == 0:
        return ''

    else:
        obj_index = SCENE_FILE_ID_TO_INDEX[str(file_id)]
        index = -1
        while index != obj_index:
            index += 1
        d = data[index]['Transform']
        obj_id = ''
        if 'm_GameObject' in list(d.keys()):
            obj_id = d['m_GameObject']['fileID']
        elif 'm_PrefabInstance' in list(d.keys()):
            obj_id = d['m_PrefabInstance']['fileID']

        return FILE_ID_SCENE_NAME[str(obj_id)]


def scale_for_falcon(unity_scale):
    scale = {'x': 0, 'y': 0, 'z': 0}
    for key in scale.keys():
        scale[key] = round(float(unity_scale[key]) * FALCON_SCALE_DOWN, 3)
    return scale


def read_collider_data(falcon_collider_data, collider_type, unity_collider_data):
    # print("In read collider data")
    if collider_type == UNITY_COLLIDER_LIST[0]:  # sphere:

        falcon_collider_data['radius'] = unity_collider_data[collider_type]['m_Radius']

    elif collider_type == UNITY_COLLIDER_LIST[1]:  # box:

        falcon_collider_data['half_sides'] = [float(unity_collider_data[collider_type]['m_Size']['x']) / 2.0,
                                              float(unity_collider_data[collider_type]['m_Size']['y']) / 2.0,
                                              float(unity_collider_data[collider_type]['m_Size']['z']) / 2.0]

    elif collider_type == UNITY_COLLIDER_LIST[2]:  # capsule:
        falcon_collider_data['radius'] = unity_collider_data[collider_type]['m_Radius']
        falcon_collider_data['half_height'] = float(unity_collider_data[collider_type]['m_Height']) / 2.0

    return falcon_collider_data


def read_prefab_source(prefab_yaml_file):
    # print("In read prefab source")
    position = DEFAULT_POSITION
    rotation = DEFAULT_ROTATION
    scale = DEFAULT_SCALE
    collider_data = {}
    obj_mesh = ''
    mat = ''
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
                    update_mesh_instance_count(obj_mesh)
                else:
                    obj_mesh = "Some default mesh. See collider for details"

            elif current_key == 'Material':
                pass  # we will do material shit here

            elif any(x == current_key for x in UNITY_COLLIDER_LIST):

                collider_data['type'] = FALCON_COLLIDERS[current_key]
                collider_data = read_collider_data(collider_data, current_key, d)

            elif current_key == 'Rigidbody':
                collider_data['rigidbody'] = 1

            elif current_key == 'MeshRenderer':
                if d['MeshRenderer']['m_Materials'][0]['guid'] in UNITY_MATERIALS_MAP.keys():
                    mat = UNITY_MATERIALS_MAP[d['MeshRenderer']['m_Materials'][0]['guid']]
                else:
                    mat = ''

    return position, rotation, scale, collider_data, obj_mesh, mat


def read_gameobject(doc_data, is_prefab_related=False):
    # print("In read gameobjects data")
    position = DEFAULT_POSITION
    rotation = DEFAULT_ROTATION
    scale = DEFAULT_SCALE
    parent = str()
    collider_data = {}
    obj_mesh = ''
    mat = ''
    collider_data['rigidbody'] = 0
    return_data = {}
    for d in doc_data:
        current_key = list(d.keys())[0]
        if current_key == 'Transform' and not is_prefab_related:
            position = d['Transform']['m_LocalPosition']
            rotation = d['Transform']['m_LocalRotation']
            scale = d['Transform']['m_LocalScale']
            parent = d['Transform']['m_Father']['fileID']

        elif current_key == 'MeshFilter':
            mesh_data = d['MeshFilter']['m_Mesh']
            if mesh_data['guid'] in UNITY_MESHES_MAP.keys():
                obj_mesh = UNITY_MESHES_MAP[mesh_data['guid']]
                update_mesh_instance_count(obj_mesh)
            else:
                obj_mesh = "Some default mesh. See collider for details"

        elif current_key == 'MeshRenderer':

            if 'guid' in d['MeshRenderer']['m_Materials'][0].keys():
                mat = UNITY_MATERIALS_MAP[d['MeshRenderer']['m_Materials'][0]['guid']]


        elif any(x == current_key for x in UNITY_COLLIDER_LIST):
            collider_data['type'] = FALCON_COLLIDERS[current_key]
            collider_data = read_collider_data(collider_data, current_key, d)

        elif current_key == 'Rigidbody':
            collider_data['rigidbody'] = 1

    if not is_prefab_related:
        return_data['position'] = position
        return_data['scale'] = scale_for_falcon(scale)
        return_data['rotation'] = rotation
    return_data['parent_id'] = parent
    return_data['obj_mesh'] = obj_mesh
    return_data['collider_data'] = collider_data
    return_data['mat'], return_data['transparency'] = read_material_data(mat)
    return return_data


def read_prefabs(prefab, prefab_file, bool_read_prefab_source=True):
    # print("In read prefab data")
    collider_data = {}
    name = str()
    return_data = {}
    print(prefab_file)
    # read the soruce of prefab
    position = DEFAULT_POSITION
    rotation = DEFAULT_ROTATION
    scale = DEFAULT_SCALE

    collider_data = {}
    obj_mesh = ''
    mat = ''
    local_modifications = prefab['m_Modification']
    parent = local_modifications['m_TransformParent']['fileID']  # get in more readable format

    local_modifications = prefab['m_Modification']['m_Modifications']
    prefab_yaml = "prefab.yaml"
    if bool_read_prefab_source is True:
        # print("Reading prefab file")
        prefab_yaml = "prefab.yaml"
        copyfile(prefab_file, prefab_yaml)
        clean_file(prefab_yaml)

        # By default we make rigidbody static
        collider_data['rigidbody'] = 0
        position, rotation, scale, collider_data, obj_mesh, mat = read_prefab_source(prefab_yaml)
    is_name_found = False
    for d in local_modifications:
        if d['propertyPath'] == 'm_Name':
            name = d['value']
            is_name_found = True

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
            temp_mat = UNITY_MATERIALS_MAP[d['objectReference']['guid']]
            if temp_mat != '':
                mat = temp_mat
            # print(mat)

    if not is_name_found:
        guid = prefab['m_SourcePrefab']['guid']
        if guid in UNITY_MESHES_MAP.keys():
            name = UNITY_MESHES_MAP[guid].split('\\')[-1].split('.')[0]
            is_name_found = True

    return_data['name'] = name
    return_data['position'] = position
    # print (prefab_file,scale)
    return_data['scale'] = scale_for_falcon(scale)
    return_data['rotation'] = rotation
    return_data['parent_id'] = parent
    return_data['obj_mesh'] = obj_mesh
    return_data['mat'], return_data['transparency'] = read_material_data(mat)
    return_data['collider_data'] = collider_data
    if bool_read_prefab_source:
        os.remove(prefab_yaml)
    return return_data


def read_material_data(material_file):
    # print("In read material data")
    materials = {}
    transparency = False
    # print(material_file)
    if material_file != '':
        material_temp = "mat.yaml"
        copyfile(material_file, material_temp)
        clean_file(material_temp)
        with open(material_temp, "r") as file_desc:
            data = yaml.load_all(file_desc, Loader=yaml.Loader)
            # data = data['Material']['m_SavedProperties']['m_TexEnv']
            for d in data:
                if list(d.keys())[0] == "Material":
                    temp = d['Material']['m_SavedProperties']['m_TexEnvs']
                    for tex in temp:
                        if list(tex.keys())[0] == '_BumpMap':
                            if 'guid' in tex['_BumpMap']['m_Texture'].keys():
                                materials['Normal_Map'] = UNITY_MATERIALS_MAP[tex['_BumpMap']['m_Texture']['guid']]
                        if list(tex.keys())[0] == '_MainTex':
                            if 'guid' in tex['_MainTex']['m_Texture'].keys():
                                materials['Albedo_Map'] = UNITY_MATERIALS_MAP[tex['_MainTex']['m_Texture']['guid']]
                        if list(tex.keys())[0] == '_OcclusionMap':
                            if 'guid' in tex['_OcclusionMap']['m_Texture'].keys():
                                materials['AO_Map'] = UNITY_MATERIALS_MAP[tex['_OcclusionMap']['m_Texture']['guid']]
                        if list(tex.keys())[0] == '_MetallicGlossMap':
                            if 'guid' in tex['_MetallicGlossMap']['m_Texture'].keys():
                                materials['Metallic'] = UNITY_MATERIALS_MAP[
                                    tex['_MetallicGlossMap']['m_Texture']['guid']]

                    temp = d['Material']['stringTagMap']
                    # print(temp.keys())
                    if temp is not None and 'RenderType' in temp.keys():
                        # print(temp['RenderType'])
                        transparency = (temp['RenderType'] == 'Transparent')
        os.remove(material_temp)
    return materials, transparency
