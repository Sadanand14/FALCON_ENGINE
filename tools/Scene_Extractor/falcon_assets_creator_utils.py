import yaml
import json
import os
from shutil import copyfile

from Scene_Extractor.constants import *
from Scene_Extractor.guid_mapper import *
from Scene_Extractor.file_id_generator import *

FILES_TO_COPY_SRC_DEST = {}
NAME_TO_TEMPLATE_MAP = {}
MATERIAL_INSTANCE_COUNT = {}
MESH_MATERIAL_MAP = {}


def create_dir_structure():
    try:
        if not os.path.exists(ASSETS_BASE_DIR):
            os.mkdir(ASSETS_BASE_DIR)
        else:
            print(ASSETS_BASE_DIR + " already present")

        if not os.path.exists(MODELS_BASE_DIR):
            os.mkdir(MODELS_BASE_DIR)
        else:
            print(MODELS_BASE_DIR + " already present")

        if not os.path.exists(ENTITIES_BASE_DIR):
            os.mkdir(ENTITIES_BASE_DIR)
        else:
            print(ENTITIES_BASE_DIR + " already present")

        if not os.path.exists(TEXTURES_BASE_DIR):
            os.mkdir(TEXTURES_BASE_DIR)
        else:
            print(TEXTURES_BASE_DIR + " already present")

        if not os.path.exists(SCENES_BASE_DIR):
            os.mkdir(SCENES_BASE_DIR)
        else:
            print(SCENES_BASE_DIR + " already present")

        if not os.path.exists(SOUNDS_BASE_DIR):
            os.mkdir(SOUNDS_BASE_DIR)
        else:
            print(SOUNDS_BASE_DIR + " already present")
    except Exception as e:
        print(e.__traceback__)
        raise Exception(str(e))


def copy_assets_from_src():
    for key in FILES_TO_COPY_SRC_DEST.keys():
        copyfile(key, FILES_TO_COPY_SRC_DEST[key])


def add_to_mesh_list(material_json, mesh):
    if mesh in MESH_MATERIAL_MAP.keys():
        MESH_MATERIAL_MAP[mesh].add(material_json)
    else:
        MESH_MATERIAL_MAP[mesh] = {material_json}


def update_instance_counter(temp):
    if temp in MATERIAL_INSTANCE_COUNT.keys():
        MATERIAL_INSTANCE_COUNT[temp] += 1
    else:
        MATERIAL_INSTANCE_COUNT[temp] = 1


def create_material_json(name, mesh, mat, isTransparent):
    material_dir = os.path.join(MODELS_BASE_DIR, mesh)
    material_json = os.path.join(material_dir, mesh + "_material.json")
    materials_textures_dir = os.path.join(TEXTURES_BASE_DIR, mesh)

    if not os.path.exists(material_dir):
        os.mkdir(material_dir)
    if not os.path.exists(materials_textures_dir):
        os.mkdir(materials_textures_dir)

    falcon_mat = {}
    for key in mat.keys():
        if key == "Normal_Map":
            falcon_mat["normal"] = os.path.join(materials_textures_dir, os.path.split(mat["Normal_Map"])[1])
            FILES_TO_COPY_SRC_DEST[mat["Normal_Map"]] = falcon_mat["normal"]
        elif key == "Albedo_Map":
            falcon_mat["albedo"] = os.path.join(materials_textures_dir, os.path.split(mat["Albedo_Map"])[1])
            FILES_TO_COPY_SRC_DEST[mat["Albedo_Map"]] = falcon_mat["albedo"]
        elif key == "Metallic":
            falcon_mat["metallic"] = os.path.join(materials_textures_dir, os.path.split(mat["Metallic"])[1])
            FILES_TO_COPY_SRC_DEST[mat["Metallic"]] = falcon_mat["metallic"]
        elif key == "AO_Map":
            falcon_mat["ao"] = os.path.join(materials_textures_dir, os.path.split(mat["AO_Map"])[1])
            FILES_TO_COPY_SRC_DEST[mat["AO_Map"]] = falcon_mat["ao"]
    if isTransparent:
        falcon_mat['Fshader'] = SHADERS['FTransparent']
    else:
        falcon_mat['Fshader'] = SHADERS['FShader']
    falcon_mat['Vshader'] = SHADERS['VShader']

    if not os.path.exists(material_json):
        with open(material_json, 'w') as fp:
            json.dump(falcon_mat, fp)
    else:
        all_files = os.listdir(material_dir)
        all_files = [i for i in all_files if i.endswith("json")]
        create_new = False
        for mat_file in all_files:
            with open(os.path.join(material_dir, mat_file)) as fp:
                curr_mat_data = json.load(fp)

            if len(curr_mat_data.keys()) != len(falcon_mat.keys()):
                create_new = False
            else:
                for key in curr_mat_data.keys():
                    if curr_mat_data[key] != falcon_mat[key]:
                        create_new = True
                        break
        if create_new:
            material_json = os.path.join(material_dir, mesh + "_material" + str(len(all_files)) + str(".json"))
            with open(material_json, 'w') as fp:
                json.dump(falcon_mat, fp)

    # generate metadata for further things
    add_to_mesh_list(material_json, mesh)
    temp = os.path.split(material_json)[1]
    update_instance_counter(temp)


def create_model_json(mesh_location_map):
    for mesh in MESH_MATERIAL_MAP.keys():
        counter = 0
        for mat in MESH_MATERIAL_MAP[mesh]:
            model_file = os.path.join(MODELS_BASE_DIR, mesh + str(counter if counter > 0 else '') + ".json")
            falcon_model = {}
            falcon_model["path"] = os.path.join(MODELS_BASE_DIR, mesh, os.path.split(mesh_location_map[mesh])[1])
            FILES_TO_COPY_SRC_DEST[mesh_location_map[mesh]] = falcon_model["path"]

            falcon_model['material'] = mat
            falcon_model['instances'] = MATERIAL_INSTANCE_COUNT[os.path.split(mat)[1]]

            with open(mat) as fp:
                curr_mat_data = json.load(fp)

            falcon_model['transparent'] = True if curr_mat_data['Fshader'] == SHADERS['FTransparent'] else False

            with open(model_file, 'w') as fp:
                json.dump(falcon_model, fp)

            counter += 1


def create_entity_json(entity_data):
    entity_json = os.path.join(ENTITIES_BASE_DIR, entity_data['name'] + "_Entity.json")
    entity_template = {}
    render_component = {}
    physics_component = {}

    if 'obj_mesh' in entity_data.keys():
        entity_template['renderComponent'] = {"mesh": FILES_TO_COPY_SRC_DEST[entity_data['obj_mesh']]}
    if 'collider_data' in entity_data.keys():
        entity_template['physicsComponent'] = {}
        for key in entity_data['collider_data']:
            entity_template['physicsComponent'][key] = entity_data['collider_data'][key]

    with open(entity_json, 'w') as fp:
        json.dump(entity_template, fp)
    NAME_TO_TEMPLATE_MAP[entity_data['name']] = entity_json


def create_scene_file(unity_data, name_to_index):
    scene_file = os.path.join(SCENES_BASE_DIR, "scene.json")
    scene_dict = {'entities': []}
    for entity in unity_data:
        tmp_entity = {}
        tmp_entity['name'] = entity['name']
        tmp_entity['scale'] = entity['scale']
        tmp_entity['pos'] = entity['position']
        tmp_entity['rot'] = entity['rotation']
        tmp_entity['childCount'] = 0
        tmp_entity['template'] = NAME_TO_TEMPLATE_MAP[entity['name']]
        tmp_entity['parent'] = name_to_index[entity['parent']] if entity['parent'] != '' else -1

        scene_dict['entities'].append(tmp_entity)
    with open(scene_file, 'w') as fp:
        json.dump(scene_dict, fp)


def create_falcon_assets(unity_data):
    create_dir_structure()
    mesh_location_map = {}
    name_to_list_index = {}
    # update default meshes in the obj_mesh
    for d in unity_data:
        if " See collider for details" in d["obj_mesh"]:
            if d['collider_data']['type'] == 0:
                d["obj_mesh"] = BASIC_MODELS["SPHERE"]
            elif d['collider_data']['type'] == 1:
                d["obj_mesh"] = BASIC_MODELS["CUBE"]
            elif d['collider_data']['type'] == 2:
                d["obj_mesh"] = BASIC_MODELS["CAPSULE"]
            elif d['collider_data']['type'] == 6:
                d["obj_mesh"] = BASIC_MODELS["PLANE"]

    # create materials & other metadata
    print("Creating material jsons")
    print("Creating models jsons")
    for d in unity_data:
        mesh = os.path.split(d["obj_mesh"])[1].split('.')[0]
        mesh_location_map[mesh] = d['obj_mesh']
        create_material_json(d['name'], mesh, d['mat'], d['transparency'])

    create_model_json(mesh_location_map)

    # create entity jsons
    index = 0
    for d in unity_data:
        create_entity_json(d)
        name_to_list_index[d['name']] = index
        index += 1

    # scene json
    create_scene_file(unity_data, name_to_list_index)
    copy_assets_from_src()
