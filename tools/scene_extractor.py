import yaml
import sys
import shutil
import os
import json

from Scene_Extractor.guid_mapper import *
from Scene_Extractor.file_id_generator import *
from Scene_Extractor.constants import *
from Scene_Extractor.scene_extraction_utils import *
from Scene_Extractor.falcon_assets_creator_utils import *


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

        generate_file_id_to_name(data)
        itr = 0
        jump_by_two = False
        while itr < len(data):
            d = data[itr]
            if list(d.keys())[0] == 'GameObject':
                if 'm_Name' not in d['GameObject'].keys():
                    itr += 1
                    continue

                if any(x == d['GameObject']['m_Name'] for x in OBJECTS_TO_EXCLUDE):
                    itr += 1
                    continue
                name = d['GameObject']['m_Name']
                component_count = len(d['GameObject']['m_Component'])
                new_data = data[itr + 1:itr + component_count + 1]
                return_data = read_gameobject(new_data)

                # position, rotation, scale, collider_data, obj_mesh, parent = read_gameobject(new_data)
                return_data['name'] = name
                return_data['parent'] = get_parent_name(data, file_id=return_data['parent_id'])
                all_entities_list.append(return_data)
                itr = itr + component_count


            elif list(d.keys())[0] == 'PrefabInstance':
                # do shit for prefab instances
                prefab = d['PrefabInstance']
                prefab_data = None
                # print(prefab['m_SourcePrefab']['guid'] + "  " + prefab['m_SourcePrefab']['guid'] in UNITY_PREFAB_MAP.keys())
                if prefab['m_SourcePrefab']['guid'] in UNITY_PREFAB_MAP.keys():
                    prefab_file = UNITY_PREFAB_MAP[prefab['m_SourcePrefab']['guid']]
                    prefab_data = read_prefabs(prefab, prefab_file, True)
                    itr += 1
                    if itr < len(data) and list(data[itr].keys())[0] == 'GameObject':
                        itr += 1
                        jump_by_two = True
                elif prefab['m_SourcePrefab']['guid'] in UNITY_MESHES_MAP.keys():
                    prefab_file = UNITY_MESHES_MAP[prefab['m_SourcePrefab']['guid']]
                    prefab_data = read_prefabs(prefab, prefab_file, False)
                    itr += 2  # skipping next entry of GameObject
                    jump_by_two = True
                new_data = []
                while itr < len(data) and (list(data[itr].keys())[0] != 'PrefabInstance' and list(data[itr].keys())[
                    0] != 'GameObject'):
                    new_data.append(data[itr])
                    itr += 1
                if jump_by_two:
                    temp_data = read_gameobject(new_data, True)
                    prefab_data['obj_mesh'] = prefab_file
                    update_mesh_instance_count(prefab_file)
                    prefab_data['collider_data'] = temp_data['collider_data']
                    itr -= 1
                    jump_by_two = False
                prefab_data['parent'] = get_parent_name(data, file_id=prefab_data['parent_id'])
                all_entities_list.append(prefab_data)


            else:
                itr += 1

    return all_entities_list
    # print obj_scale
    # print obj_pos
    # print obj_rot
    # Creating json scene now


if __name__ == "__main__":
    scene_file = sys.argv[1]
    # creating copy of the file
    copyfile(scene_file, "test.yaml")

    guid_mapper(sys.argv[2])
    # SceneTest(sys.argv[1])
    clean_file("test.yaml")
    generate_file_ids("test.yaml")
    scene_data = read_scene_file("test.yaml")

    print("Unity Scene Data:")
    for d in scene_data:
        print(d)
    # print(MESH_INSTANCE_COUNTER)

    if os.path.exists(ASSETS_BASE_DIR):
        shutil.rmtree(ASSETS_BASE_DIR)

    print("Generating Falcon Assets:")
    create_falcon_assets(scene_data)
