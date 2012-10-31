{
  'variables': {
    'LIBEVHTP_MAJOR_VERSION': '0',
    'LIBEVHTP_MINOR_VERSION': '4',
    'LIBEVHTP_MICRO_VERSION': '15',
  },
  'targets': [
    {
      'target_name': 'libevhtp',
      'type': 'static_library',
      'dependencies': [
        'oniguruma',
        'libevent2.gyp:libevent2',
        'libevhtp_copy_headers',
      ],
      'sources': [
          'libevhtp/evhtp.c',
          'libevhtp/evhtp.h',
          'libevhtp/htparse/htparse.c',
          'libevhtp/htparse/htparse.h',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)',
        ]
      },
      'include_dirs': [
        'libevhtp/include',
        'libevhtp/htparse',
        'libevhtp/oniguruma',
        '<(SHARED_INTERMEDIATE_DIR)/libevhtp',
      ],
      'defines': [
          'NO_STRNLEN',
          'NO_STRNDUP', ],
      'conditions': [
			['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
			  'cflags': [ '--std=c89' ],
			  'defines': [ '_GNU_SOURCE' ],
			  'sources': [
                  'libevhtp/kqueue-internal.h',
                  'libevhtp/evthread_pthread.c',
                 ],
			}],
			['OS=="win"', {
			  'defines': [ 'NO_SYS_UN', ],
			  'sources': [
			  ],      
			  'include_dirs': [
			  ],
			}],
			['without_ssl=="false"', {
			  'dependencies': [
				'openssl/openssl.gyp:openssl'
			   ],
			  },{
			  'defines': [ 'EVHTP_DISABLE_SSL', ],
			}],
			['without_pthread=="false"', {
			  'sources': [
                  'libevhtp/evthr/evthr.c',
                  'libevhtp/evthr/evthr.h',
			    ],   
			  }, {
			  'defines': [ 'EVHTP_DISABLE_EVTHR' ],
            }],
       ],
    }, # end libevhtp
    
    {
      'target_name': 'ht_test',
      'type': 'executable',
      'sources': [
          'libevhtp/htparse/htparse.h',
          'libevhtp/htparse/htparse.c',
          'libevhtp/htparse/test.c',
      ],
    }, # end ht_test
    {
      'target_name': 'evhtp_test',
      'type': 'executable',
      'dependencies': [
        'libevhtp'
      ],
      'sources': [
          'libevhtp/test.c',
      ],
    }, # end evhtp_test
    {
      'target_name': 'evhtp_test_basic',
      'type': 'executable',
      'dependencies': [
        'libevhtp'
      ],
      'sources': [
          'libevhtp/test_basic.c',
      ],
    }, # end evhtp_test_basic
    {
      'target_name': 'evhtp_test_vhost',
      'type': 'executable',
      'dependencies': [
        'libevhtp'
      ],
      'sources': [
          'libevhtp/test_vhost.c',
      ],
    }, # end evhtp_test_vhost
    {
      'target_name': 'oniguruma',
      'type': 'static_library',
      'sources': [
          'libevhtp/oniguruma/oniggnu.h',
          'libevhtp/oniguruma/onigposix.h',
          'libevhtp/oniguruma/oniguruma.h',
          'libevhtp/oniguruma/regcomp.c',
          'libevhtp/oniguruma/regenc.c',
          'libevhtp/oniguruma/regenc.h',
          'libevhtp/oniguruma/regerror.c',
          'libevhtp/oniguruma/regexec.c',
          'libevhtp/oniguruma/regext.c',
          'libevhtp/oniguruma/reggnu.c',
          'libevhtp/oniguruma/regint.h',
          'libevhtp/oniguruma/regparse.c',
          'libevhtp/oniguruma/regparse.h',
          'libevhtp/oniguruma/regposerr.c',
          'libevhtp/oniguruma/regposix.c',
          'libevhtp/oniguruma/regsyntax.c',
          'libevhtp/oniguruma/regtrav.c',
          'libevhtp/oniguruma/regversion.c',
          'libevhtp/oniguruma/st.c',
          'libevhtp/oniguruma/st.h',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)',
        ]
      },
      'include_dirs': [
        '<(SHARED_INTERMEDIATE_DIR)',
      ],
      'defines': [ 'RUBY_EXPORT', ],
       'conditions': [
			['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
			  'cflags': [ '--std=c89' ],
			  'defines': [ '_GNU_SOURCE' ]
			}],
			['OS=="win"', {
			  'sources': [
				 'libevhtp/oniguruma/win32/config.h',
			  ],      
			  'include_dirs': [
                'libevhtp/oniguruma/win32/',
			  ],
			}],
       ],
    }, # end oniguruma
    {
      'target_name': 'oniguruma_test',
      'type': 'executable',
      'dependencies': [
        'oniguruma'
      ],
      'sources': [
          'libevhtp/oniguruma/testc.c',
          'libevhtp/oniguruma/testu.c',
      ],
    }, # end evhtp_test_vhost
    {
      'target_name': 'libevhtp_copy_headers',
      'type': 'none',
      #'copies': [
      #  {
      #    'destination': '<(SHARED_INTERMEDIATE_DIR)/libevhtp/sys',
      #    'files': [
      #      'libevhtp/compat/sys/tree.h.in',
      #    ]
      #  }
      #],
      
    'actions': [
      {
        'variables': {
          'replacements': [
            '{LIBEVHTP_MAJOR_VERSION}:<(LIBEVHTP_MAJOR_VERSION)',
            '{LIBEVHTP_MINOR_VERSION}:<(LIBEVHTP_MINOR_VERSION)',
            '{LIBEVHTP_MICRO_VERSION}:<(LIBEVHTP_MICRO_VERSION)',
          ]
        },
        'action_name': 'version_header',
        'inputs': [
            'libevhtp/compat/sys/tree.h.in',
        ],
        'outputs': [
          '<(SHARED_INTERMEDIATE_DIR)/libevhtp/sys/tree.h',
        ],
        'action': [
          'python',
          '../tools/lame_sed.py',
          '<@(_inputs)',
          '<@(_outputs)',
          '<@(replacements)',
        ],
      }
    ]
    }, # libevhtp_copy_headers
  ] # end targets
}
