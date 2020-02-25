import yaml
import sys
from shutil import copyfile
import os
import json
from Scene_Extractor.guid_mapper import *



def ReadPrefabs(prefab):
    position = {'x': '', 'y': '', 'z': ''}
    rotation = {'x': '', 'y': '', 'z': '', 'w': ''}
    scale = {'x': '', 'y': '', 'z': ''}
    name = str()
    return_data = {}
    for d in prefab:
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
    return_data['scale'] = scale
    return_data['rotation'] = rotation
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

        c = 0
        for d in data:
            print(d)
            input()
        print(c)
        return
        readTransform = False

        for d in data:
            if readTransform is False and d.keys()[0] == 'GameObject':
                if d['GameObject']['m_Name'] == "Main Camera" or d['GameObject']['m_Name'] == "Directional Light":
                    continue
                name = d['GameObject']['m_Name']
                readTransform = True
            # print(name , d)
            elif readTransform is True and d.keys()[0] == 'Transform':
                position = d['Transform']['m_LocalPosition']
                rotaion = d['Transform']['m_LocalRotation']
                scale = d['Transform']['m_LocalScale']
                readTransform = False
                obj_pos[name] = position
                obj_rot[name] = rotaion
                obj_scale[name] = scale
            elif d.keys()[0] == 'PrefabInstance':
                # do shit for prefab instances
                prefab_data = ReadPrefabs(d['PrefabInstance']['m_Modification']['m_Modifications'])
                name = prefab_data['name']
                obj_pos[name] = prefab_data['position']
                obj_rot[name] = prefab_data['rotation']
                obj_scale[name] = prefab_data['scale']
                print('\n')
                print(name, obj_pos[name], obj_rot[name], obj_scale[name])
                print('\n')
                pass

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


def CleanFile(filepath):
    file_desc = open(filepath, "r")
    result = str()
    for lineNumber, line in enumerate(file_desc.readlines()):
        if lineNumber == 1:
            continue
        if line.startswith('--- !u!'):
            result += '--- ' + line.split(' ')[2] + '\n'
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
    #copyfile(scene_file, "test.yaml")

    guid_mapper(sys.argv[2]);

    #CleanFile("test.yaml")
    data = ReadSceneFile("test.yaml")

# os.remove("test.yaml")\
# input("Hit enter")
