import os, sys, subprocess, glob
import shutil
import configparser
from setuptools import setup, Extension
from setuptools.dist import Distribution
import mpi4py

open_kwargs = {'encoding': 'utf-8'}

# read setup.cfg
setup_cfg = 'setup.cfg'
sys.stdout.write('reading from setup.cfg...\n')
config = configparser.ConfigParser()
config.read(setup_cfg)
pnc_config = None
try:
    pnc_config = config.get("options", "pnetcdf_config")
except:
    pass

# TODO: provide more ways to search for pnetcdf-config
if pnc_config is None:
    print("Error: cannot finf pnetcdf-config from setup.cfg. Abort.", file=sys.stderr)
    exit(-1)

def get_str_from_pnc_config(pnc_config, option:str)->str:
    res = subprocess.Popen([pnc_config, option], stdout=subprocess.PIPE).communicate()[0]
    return res.decode().strip()

# get pnc version
pnc_ver = get_str_from_pnc_config(pnc_config, "--version")

def is_pnc_ver_valid(pnc_ver:str)->bool:
    if pnc_ver.split()[0] != "PnetCDF":
        return False
    #TODO: add more checks
    return True

if not is_pnc_ver_valid(pnc_ver):
    print("Error: Invalid PnetCDF Version. Got:", pnc_ver, file=sys.stderr)
    exit(-1)

pnc_libdir = get_str_from_pnc_config(pnc_config, "--libdir")
pnc_includedir = get_str_from_pnc_config(pnc_config, "--includedir")

print("pnc_ver:", pnc_ver)
print("pnc_libdir:", pnc_libdir)
print("pnc_includedir:", pnc_includedir)

pnc_src_root = os.path.join(os.path.join('src','PnetCDF'), '_PnetCDF')
pnc_src_c = pnc_src_root + '.c'
pnc_src_pyx = pnc_src_root + '.pyx'

if os.path.exists(pnc_src_c):
    os.remove(pnc_src_c)

inc_dirs = [pnc_includedir]
lib_dirs = [pnc_libdir]

# Do not require numpy for just querying the package
# Taken from the h5py setup file.
if any('--' + opt in sys.argv for opt in Distribution.display_option_names +
        ['help-commands', 'help']) or sys.argv[1] == 'egg_info':
    pass
else:
    # append numpy include dir.
    import numpy
    inc_dirs.append(numpy.get_include())

inc_dirs.append(mpi4py.get_include())
libs=["pnetcdf"]
runtime_lib_dirs=lib_dirs
print("inc_dirs:", inc_dirs)
print("lib_dirs:", lib_dirs)
print("libs:", libs)
print("runtime_lib_dirs:", runtime_lib_dirs)

DEFINE_MACROS = [("NPY_NO_DEPRECATED_API", "NPY_1_7_API_VERSION")]

ext_modules = [Extension("pncpy",
                             [pnc_src_pyx],
                             define_macros=DEFINE_MACROS,
                             libraries=libs,
                             library_dirs=lib_dirs,
                             include_dirs=inc_dirs + ['include'],
                             runtime_library_dirs=runtime_lib_dirs)]

for e in ext_modules:
    e.cython_directives = {'language_level': "3"}

def extract_version(CYTHON_FNAME):
    version = None
    with open(CYTHON_FNAME) as fi:
        for line in fi:
            if (line.startswith('__version__')):
                _, version = line.split('=')
                version = version.strip()[1:-1]  # Remove quotation characters.
                break
    return version

setup(
    name="pncpy",  # need by GitHub dependency graph
    version=extract_version(pnc_src_pyx),
    ext_modules=ext_modules,
)
