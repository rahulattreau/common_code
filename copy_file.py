import shutil, os

file_name = 'window_filter/window_filter' 
c_files = [file_name + '.c']
h_files = [file_name + '.h']

for f in c_files:
    shutil.copy(f, '../rrhp-odb/Src/control_common_code/')

for f in h_files:
    shutil.copy(f, '../rrhp-odb/Inc/control_common_code/')
