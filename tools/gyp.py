#!/usr/bin/env python
import glob
import os
import shlex
import sys

script_dir = os.path.dirname(__file__)

sys.path.insert(0, os.path.join(script_dir, 'gyp', 'pylib'))
import gyp

# Directory within which we want all generated files (including Makefiles)
# to be written.
output_dir = os.path.join(os.path.abspath(script_dir), '../out')

def run_gyp(args):
  rc = gyp.main(args)
  if rc != 0:
    print 'Error running GYP'
    sys.exit(rc)

if __name__ == '__main__':
  args = sys.argv[1:]

  # GYP bug.
  # On msvs it will crash if it gets an absolute path.
  # On Mac/make it will crash if it doesn't get an absolute path.
  if sys.platform == 'win32':
    args.append(os.path.join(script_dir, '../meijing-core.gyp'))
    common_fn  = os.path.join(script_dir, '../common.gypi')
    options_fn = os.path.join(script_dir, '../options.gypi')
  else:
    args.append(os.path.join(os.path.abspath(script_dir), '../meijing-core.gyp'))
    common_fn  = os.path.join(os.path.abspath(script_dir), '../common.gypi')
    options_fn = os.path.join(os.path.abspath(script_dir), '../options.gypi')

  if os.path.exists(common_fn):
    args.extend(['-I', common_fn])

  if os.path.exists(options_fn):
    args.extend(['-I', options_fn])

  args.append('--depth=' + script_dir)

  # There's a bug with windows which doesn't allow this feature.
  if sys.platform != 'win32':

    # Tell gyp to write the Makefiles into output_dir
    args.extend(['--generator-output', output_dir])

    # Tell make to write its output into the same dir
    args.extend(['-Goutput_dir=' + output_dir])

  args.append('-Dtarget_arch=ia32')
  args.append('-Dcomponent=static_library')
  args.append('-Dlibrary=static_library')
  gyp_args = list(args)
  run_gyp(gyp_args)
  sys.exit(0)
