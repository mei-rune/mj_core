{
  'variables': {
  },
  'targets': [
    {
      'target_name': 'libmeijing_core',
      'type': '<(library)' ,
      'dependencies': [
        'deps/yajl.gyp:yajl',
        'deps/libevent2.gyp:libevent2',
        'deps/libevent2.gyp:libevent2_core',
        'deps/libevent2.gyp:libevent2_extras',
        'deps/libevhtp.gyp:libevhtp',
      ],
      'sources': [
          'src/internal.h',
          'src/hashtable.c',
          'src/itostr.c',
          'src/json.c',
          'src/networking.c',
          'src/rpc.c',
          'src/rpc_libevent2.c',
          'src/hooks.c',
          'src/variables.c',
          'src/variables_list.c',
          'src/variables_format.c',
          'src/variables_new.c',
          'src/variables_string.c',
          'src/variables_table.c',
          'src/variables_to.c',
          'src/variables_utils.c',
          'src/variables_private.h',
          'src/rollingfile.c',
          'src/string_buffer.c',
          'src/target.c',
          'src/unittest.c',
          'src/utils.c',
          'include/meijing/rpc.h',
          'include/meijing/hooks.h',
          'include/meijing/cycle.h',
          'include/meijing/array.h',
          'include/meijing/hashtable.h',
          'include/meijing/itostr.h',
          'include/meijing/json.h',
          'include/meijing/last_error.h',
          'include/meijing/link.h',
          'include/meijing/log.h',
          'include/meijing/networking.h',
          'include/meijing/variables.h',
          'include/meijing/os_atomic.h',
          'include/meijing/os_dirent.h',
          'include/meijing/os_dlfcn.h',
          'include/meijing/os_getopt.h',
          'include/meijing/os_sem.h',
          'include/meijing/os_threads.h',
          'include/meijing/os_time.h',
          'include/meijing/platform.h',
          'include/meijing/rollingfile.h',
          'include/meijing/string_buffer.h',
          'include/meijing/target.h',
          'include/meijing/unittest.h',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'include',
        ]
      },
      'include_dirs': [
        'src',
        'include',
        '<(SHARED_INTERMEDIATE_DIR)',
      ],
      'defines': [ ],
      'conditions': [
        ['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
          'cflags': [ '--std=c89' ],
          'defines': [ '_GNU_SOURCE' ]
        }],
        ['OS=="win"', {
          'sources': [
            'src/win32/dirent.c',
            'src/win32/dlfcn.c',
            'src/win32/getopt.c',
            'src/win32/getopt1.c',
            'src/win32/getopt_int.h',
            'src/win32/gettimeofday.c',
            'src/win32/iputilities.c',
            'src/win32/pthread_windows.c',
            'src/win32/sem.c',
            'include/meijing/win32/dirent.h',
            'include/meijing/win32/dlfcn.h',
            'include/meijing/win32/getopt.h',
            'include/meijing/win32/pthread_windows.h',
            'include/meijing/win32/time.h',
          ],
        }],
      ],
    }, # end libmeijing_core
    {
      'target_name': 'meijing_core_test',
      'type': 'executable',
      'dependencies': [
        'libmeijing_core',
      ],
      'sources': [
          'test/array_test.c',
          'test/hashtable_test.c',
          'test/json_test.c',
          'test/object_test.c',
          'test/rpc_test.c',
          'test/main.c',
      ],
      'defines': [ ],
       'conditions': [
        ['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
          'cflags': [ '--std=c89' ],
          'defines': [ '_GNU_SOURCE' ]
        }],
      ],
    }, # end meijing_core_test
  ] # end targets
}
