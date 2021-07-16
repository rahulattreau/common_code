import shutil, os

folder_name = 'input_conditioning'
file_name = folder_name 
c_files = [folder_name + '/' + file_name + '.c']
h_files = [folder_name + '/control_common_code/' + file_name + '.h']

for f in c_files:
    shutil.copy(f, '../rrhp-odb/Src/control_common_code/')

for f in h_files:
    shutil.copy(f, '../rrhp-odb/Inc/control_common_code/')
