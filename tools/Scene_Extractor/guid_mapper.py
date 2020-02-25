import yaml
import os
import re

unity_material_maps = {}
unity_mesh_map = {}


def guid_mapper(file_path):
    pattern = "^guid: *$"
    for subdir, dirs, files in os.walk(file_path):
        for file in files:
            if ".mat" in file and file.endswith("meta"):
                # print(os.path.join(subdir, file))
                for line in open(os.path.join(subdir, file), 'r'):
                    if "guid" in line:  # TODO: replace with regex
                        guid = line.split(':')[1].strip()
                        material_file = file.split('.')
                        unity_material_maps[guid] = os.path.join(subdir, material_file[0] + "." + material_file[1])

            if "mat" in file and file.endswith("meta"):
                pass
    for key in unity_material_maps.keys():
        print(key + " : " + unity_material_maps[key])
