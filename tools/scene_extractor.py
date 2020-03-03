import yaml
import sys
from shutil import copyfile
import os
import json
from Scene_Extractor.guid_mapper import *
from Scene_Extractor.file_id_generator import *
from Scene_Extractor.constants import *
from Scene_Extractor.scene_extraction_utils import *

def read_scene_file(filepath):
    obj_pos = {}
    obj_rot = {}
    obj_scale = {}
    name = str()
    position = {}
    rotation = {}
    scale = {}

    all_entities_list = []
    # reads the yaml file into the
    with open(filepath, "r") as file_desc:
        data = yaml.load_all(file_desc, Loader=yaml.Loader)
        data = list(data)
        itr = 0
        while itr < len(data):
            d = data[itr]
            if list(d.keys())[0] == 'GameObject':
                if d['GameObject']['m_Name'] == "Main Camera" or d['GameObject']['m_Name'] == "Directional Light":
                    itr += 1
                    continue
                name = d['GameObject']['m_Name']
                component_count = len(d['GameObject']['m_Component'])
                new_data = data[itr + 1:itr + component_count + 1]
                return_data = read_gameobject(new_data)

                # position, rotation, scale, collider_data, obj_mesh, parent = read_gameobject(new_data)
                return_data['name'] = name
                all_entities_list.append(return_data)
                itr = itr + component_count


            elif list(d.keys())[0] == 'PrefabInstance':
                # do shit for prefab instances
                prefab = d['PrefabInstance']
                prefab_data = None
                print(prefab['m_SourcePrefab']['guid'] + "  "+ prefab['m_SourcePrefab']['guid'] in UNITY_PREFAB_MAP.keys())
                if prefab['m_SourcePrefab']['guid'] in UNITY_PREFAB_MAP.keys():
                    prefab_file = UNITY_PREFAB_MAP[prefab['m_SourcePrefab']['guid']]
                    prefab_data = read_prefabs(prefab, prefab_file, True)
                    itr += 1
                elif prefab['m_SourcePrefab']['guid'] in UNITY_MESHES_MAP.keys():
                    prefab_file = UNITY_MESHES_MAP[prefab['m_SourcePrefab']['guid']]
                    prefab_data = read_prefabs(prefab, prefab_file, False)
                    itr += 2  # skipping next entry of GameObject
                    new_data = []
                    while list(data[itr].keys())[0] != 'PrefabInstance' and list(data[itr].keys())[0] != 'GameObject':
                        new_data.append(data[itr])
                        itr += 1
                    temp_data = read_gameobject(new_data,True)
                    prefab_data['obj_mesh'] = prefab_file
                    update_mesh_instance_count(prefab_file)
                    prefab_data['collider_data'] = temp_data['collider_data']

                    itr -= 1
                all_entities_list.append(prefab_data)
            else:
                itr += 1

    return all_entities_list
    # print obj_scale
    # print obj_pos
    # print obj_rot
    # Creating json scene now


'''scene_dict = {'entities': []}

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
'''





if __name__ == "__main__":
    scene_file = sys.argv[1]
    # creating copy of the file
    copyfile(scene_file, "test.yaml")

    guid_mapper(sys.argv[2])
    # SceneTest(sys.argv[1])
    clean_file("test.yaml")
    generate_file_ids("test.yaml")
    data = read_scene_file("test.yaml")

    for d in data:
        print(d)
    print(MESH_INSTANCE_COUNTER)
        # input()
# os.remove("test.yaml")\
# input("Hit enter")
