import shutil, os

file_name = 'lookup_table/lookup_table' 
c_files = [file_name + '.c']
h_files = [file_name + '.h']

for f in c_files:
    shutil.copy(f, '../rrhp-odb/Src')

for f in h_files:
    shutil.copy(f, '../rrhp-odb/Inc')