from Scene_Extractor.constants import UNITY_PREFAB_MAP,UNITY_MESHES_MAP,UNITY_MATERIALS_MAP
from Scene_Extractor.constants import SCENE_FILE_ID_TO_INDEX,SCENE_INDEX_TO_FILE_ID,FILE_ID_SCENE_NAME
import json

def generate_file_ids(file):
    file_desc = open(file, "r")
    result = str()
    doc_index = 0
    for lineNumber, line in enumerate(file_desc.readlines()):
        if line.startswith('---'):
            file_id = line.split('&')[1].split(' ')[0].split('\n')[0]
            SCENE_FILE_ID_TO_INDEX[file_id] = doc_index
            SCENE_INDEX_TO_FILE_ID[doc_index] = file_id
            doc_index += 1

    #for key in SCENE_FILE_ID_TO_INDEX.keys():
     #   print(key + "  " + str(SCENE_FILE_ID_TO_INDEX[key]))

    #test = 'file_id_to_index.json'
    #with open(test, 'w') as fp:
        #json.dump(SCENE_FILE_ID_TO_INDEX, fp)


def generate_file_id_to_name(data):

    for index in range(0,len(data)):
        d= data[index]
        if list(d.keys())[0] == 'GameObject':
            if 'm_Name' in list(d['GameObject'].keys()):
                FILE_ID_SCENE_NAME[SCENE_INDEX_TO_FILE_ID[index]] = d['GameObject']['m_Name']
        elif list(d.keys())[0] == 'PrefabInstance':
            local_modifications = d['PrefabInstance']['m_Modification']['m_Modifications']
            is_name_found = False
            for mod in local_modifications:
                if mod['propertyPath'] == 'm_Name':
                    FILE_ID_SCENE_NAME[SCENE_INDEX_TO_FILE_ID[index]] = mod['value']
                    is_name_found = True
            if not is_name_found:
                guid = d['PrefabInstance']['m_SourcePrefab']['guid']
                if guid in UNITY_MESHES_MAP.keys():
                    name = UNITY_MESHES_MAP[guid].split('\\')[-1].split('.')[0]
                    FILE_ID_SCENE_NAME[SCENE_INDEX_TO_FILE_ID[index]] = name
                    is_name_found = True

            if not is_name_found:
                print("Failed to find name for " + str(index))


    #for key in FILE_ID_SCENE_NAME.keys():
        #print(key + "  " + FILE_ID_SCENE_NAME[key])
    #test = 'file_id_to_name.json'
    #with open(test, 'w') as fp:
     #   json.dump(FILE_ID_SCENE_NAME, fp)