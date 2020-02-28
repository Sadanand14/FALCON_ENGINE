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


def parse_components(scene_data, comp_list):
    print(comp_list)
    print("\n")


def ReadPrefabs(prefab):
    position = {'x': '', 'y': '', 'z': ''}
    rotation = {'x': '', 'y': '', 'z': '', 'w': ''}
    scale = {'x': '', 'y': '', 'z': ''}
    obj_mesh = ''
    collider_data = {}
    name = str()
    return_data = {}
    # read the soruce of prefab  ['m_Modification']['m_Modifications']
    prefab_file = UNITY_PREFAB_MAP[prefab['m_SourcePrefab']['guid']]
    local_modifications = prefab['m_Modification']['m_Modifications']
    prefab_yaml = "prefab.yaml"
    copyfile(prefab_file, prefab_yaml)
    clean_file(prefab_yaml)

    # By default we make rigidbody static
    collider_data['rigidbody'] = 0
    with open(prefab_yaml, "r") as file_desc:
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

    return_data['name'] = name
    return_data['position'] = position
    return_data['scale'] = scale_for_falcon(scale)
    return_data['rotation'] = rotation

    os.remove(prefab_yaml)
    return return_data


def ReadSceneFile(filepath):
    obj_pos = {}
    obj_rot = {}
    obj_scale = {}
    name = str()
    position = {}
    rotation = {}
    scale = {}
    # reads the yaml file into the
    with open(filepath, "r") as file_desc:
        data = yaml.load_all(file_desc, Loader=yaml.Loader)

        readTransform = False
        c = 0

        '''for d in data:
            if readTransform is False and list(d.keys())[0] == 'GameObject':
                if d['GameObject']['m_Name'] == "Main Camera" or d['GameObject']['m_Name'] == "Directional Light":
                    continue
                else:
                    parse_components(data,d['GameObject']['m_Component'])

        return
        '''
        for d in data:
            if readTransform is False and list(d.keys())[0] == 'GameObject':
                if d['GameObject']['m_Name'] == "Main Camera" or d['GameObject']['m_Name'] == "Directional Light":
                    continue
                name = d['GameObject']['m_Name']
                readTransform = True
            # print(name , d)
            elif readTransform is True and list(d.keys())[0] == 'Transform':
                position = d['Transform']['m_LocalPosition']
                rotaion = d['Transform']['m_LocalRotation']
                scale = d['Transform']['m_LocalScale']
                readTransform = False
                obj_pos[name] = position
                obj_rot[name] = rotaion
                obj_scale[name] = scale_for_falcon(scale)
            elif list(d.keys())[0] == 'PrefabInstance':
                # do shit for prefab instances
                prefab_data = ReadPrefabs(d['PrefabInstance'])
                name = prefab_data['name']
                obj_pos[name] = prefab_data['position']
                obj_rot[name] = prefab_data['rotation']
                obj_scale[name] = prefab_data['scale']
                print('\n')
                print(name, obj_pos[name], obj_rot[name], obj_scale[name])
                print('\n')

    # print obj_scale
    # print obj_pos
    # print obj_rot
    # Creating json scene now
    scene_dict = {'entities': []}

    for key in obj_pos.keys():
        position = obj_pos[key]
        rotation = obj_rot[key]
        scale = obj_scale[key]
        tmp_dict = {"name": key,
                    "pos": [position['x'], position['y'], position['z']],
                    "rot": [rotation['x'], rotation['y'], rotation['z'], rotation['w']],
                    "scale": [scale['x'], scale['y'], scale['z']],
                    "template": '',
                    "childCount": 0
                    }
        scene_dict['entities'].append(tmp_dict)

    print(str(scene_dict))

    with open('result.json', 'w') as fp:
        json.dump(scene_dict, fp)

    return data


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


if __name__ == "__main__":
    scene_file = sys.argv[1]
    # creating copy of the file
    copyfile(scene_file, "test.yaml")

    guid_mapper(sys.argv[2])
    # SceneTest(sys.argv[1])
    clean_file("test.yaml")
    generate_file_ids("test.yaml")
    data = ReadSceneFile("test.yaml")

# os.remove("test.yaml")\
# input("Hit enter")
