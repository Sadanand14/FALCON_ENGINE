import os
import platform
import requests
import sys
import tarfile
import warnings
import zipfile

# Ref. https://stackoverflow.com/questions/38511444/python-download-files-from-google-drive-using-url

class GDriveDownloader:

	URL = "https://docs.google.com/uc?export=download"


	@staticmethod
	def download_file(file_name,file_id, dest_path, unzip = True):
		'''
		file_name = file to be downloaded.
		file_id   = file id from google drive sharable link
		dest_path = where to download
		unzip 	  = should file be unziped?
		'''

		#setting up download directory
		dest_dir = os.path.dirname(dest_path)
		if not os.path.exists(dest_dir):
			os.makedirs(dest_dir)

		#downloading the file if not exists
		full_path = dest_path+file_name
		if not os.path.exists(full_path):
			
			download_session = requests.Session()
			print("Downloading {} to {}".format(file_name,dest_path))

			response = download_session.get(GDriveDownloader.URL, params={'id':file_id},stream = True)

			token = GDriveDownloader.get_confirm_token(response)

			if token:
				params = {'id':file_id, 'confirm':token }
				response = download_session.get(GDriveDownloader.URL, params=params,stream = True)

			GDriveDownloader.save_response_content(response,full_path)
			print("Successfully downloaded the {}".format(file_name))

			if unzip:
				try:
					print('Unzipping...')
					extraction_path = os.path.dirname(dest_path)
					if platform.system() == 'Windows':
						with zipfile.ZipFile(full_path, 'r') as z:
							z.extractall(extraction_path)
					elif platform.system() == 'Linux':
						with tarfile.open(full_path, 'r') as z:
							z.extractall(extraction_path)
					print('Done.Removing archive file')
					os.remove(full_path)
				except zipfile.BadZipfile:
					warnings.warn('Ignoring `unzip` since "{}" does not look like a valid zip file'.format(file_name))

		else:
			print("{} already present. Not downloading".format(file_name))


	@staticmethod
	def get_confirm_token(response):
	    for key, value in response.cookies.items():
	        if key.startswith('download_warning'):
	            return value

	    return None

	@staticmethod
	def save_response_content(response, destination):
	    CHUNK_SIZE = 32768

	    with open(destination, "wb") as f:
	        for chunk in response.iter_content(CHUNK_SIZE):
	            if chunk: # filter out keep-alive new chunks
	                f.write(chunk)


if __name__ == '__main__':
	if platform.system() == 'Windows':
		GDriveDownloader.download_file("vendor.zip","1K6funGJKcaJOuOas6qUDLEqo4fd0VUlw","../GraphicsEngine/GraphicsEngine/")
		GDriveDownloader.download_file("assets.zip","1MibiR-59YDyeMXrKdxREv8tDXY4UDL_f","../GraphicsEngine/GraphicsEngine/")
	elif platform.system() == 'Linux':
		GDriveDownloader.download_file("vendor_linux.tar","1mWyzqTZAGRuzFfU-S2N1N6L927BzhbaN","../GraphicsEngine/GraphicsEngine/")
		GDriveDownloader.download_file("assets.zip","1MibiR-59YDyeMXrKdxREv8tDXY4UDL_f","../GraphicsEngine/GraphicsEngine/")
	else:
		print "Unsupported platform"

