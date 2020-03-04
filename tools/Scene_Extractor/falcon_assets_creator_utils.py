import yaml
import json
import os
from shutil import copyfile

from Scene_Extractor.constants import *
from Scene_Extractor.guid_mapper import *
from Scene_Extractor.file_id_generator import *


def create_dir_structure():
    try:
        os.mkdir(ASSETS_BASE_DIR)
        os.mkdir(ASSETS_BASE_DIR)
        os.mkdir(ASSETS_BASE_DIR)
        os.mkdir(ASSETS_BASE_DIR)
        os.mkdir(ASSETS_BASE_DIR)
    except Exception as e:
        raise Exception(str(e))




