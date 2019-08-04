## Setting up Vendors and Assets with scrpt:

### Prerequisites:
1. Install python 3.* and its path to system paths.
   #### Resource links:
   1. [python download link(select latest 3.* from all releases)](https://www.python.org/downloads/)
   2. [Setting up python and pip path for windows(Tutorial)](https://www.youtube.com/watch?v=Y2q_b4ugPWk)
   
2. We are using `requests` module in python. To install it run following commands in terminal,
   1. ```shell python -m pip install --upgrade pip ```
   2. ```shell pip install requests```

### Setting up drive files:
Whenever you update vendor folders on GDrive, take care of following things,

1. Vendor is .zip archive of Windows and tarball for linux.
2. Share vendor archive on drive and make sure you make it available to everyone with read access outside RIT email as well.

3. Now copy file id from share link and update the relevent variables in the python script. i.e. ASSET_FILE_ID for asset id and VENDOR_WIN_ID 
   and VENDOR_LINUX_ID for respective os.
   
### Running the script

Open tools folder in terminal/CMD and, 

if you have path to the python setup in system paths use, 

```shell
python setup_vendor.py
```
if not then,

```shell
<path to python.exe> setup_vendor.py
```
