from unityparser import UnityDocument

SCENE_FILE_ID = {}


def generate_file_ids(file):
    file_desc = open(file, "r")
    result = str()
    doc_index = 0
    for lineNumber, line in enumerate(file_desc.readlines()):
        if line.startswith('---'):
            file_id = line.split('&')[1].split('\n')[0]
            SCENE_FILE_ID[file_id] = doc_index
            doc_index += 1

    for key in SCENE_FILE_ID.keys():
        print(key, SCENE_FILE_ID[key])
