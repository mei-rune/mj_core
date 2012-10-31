{
  'variables': {
  },
  'targets': [
    {
      'target_name': 'libevent2',
      'type': 'static_library',
      'sources': [
          'libevent2/include/event2/buffer.h',
          'libevent2/include/event2/bufferevent.h',
          'libevent2/include/event2/bufferevent_compat.h',
          'libevent2/include/event2/bufferevent_struct.h',
          'libevent2/include/event2/buffer_compat.h',
          'libevent2/include/event2/dns.h',
          'libevent2/include/event2/dns_compat.h',
          'libevent2/include/event2/dns_struct.h',
          'libevent2/include/event2/event.h',
          'libevent2/include/event2/event_compat.h',
          'libevent2/include/event2/event_struct.h',
          'libevent2/include/event2/http.h',
          'libevent2/include/event2/http_compat.h',
          'libevent2/include/event2/http_struct.h',
          'libevent2/include/event2/keyvalq_struct.h',
          'libevent2/include/event2/listener.h',
          'libevent2/include/event2/rpc.h',
          'libevent2/include/event2/rpc_compat.h',
          'libevent2/include/event2/rpc_struct.h',
          'libevent2/include/event2/tag.h',
          'libevent2/include/event2/tag_compat.h',
          'libevent2/include/event2/thread.h',
          'libevent2/include/event2/util.h',
          'libevent2/event.c',
          'libevent2/buffer.c',
          'libevent2/bufferevent.c',
          'libevent2/bufferevent_sock.c',
          'libevent2/bufferevent_pair.c',
          'libevent2/bufferevent_filter.c',
          'libevent2/bufferevent_ratelim.c',
          'libevent2/listener.c',
          'libevent2/evmap.c',
          'libevent2/log.c',
          'libevent2/evutil.c',
          'libevent2/evutil_rand.c',
          'libevent2/evutil_time.c',
          'libevent2/strlcpy.c',
          'libevent2/signal.c',
          'libevent2/evthread.c',
          'libevent2/event_tagging.c',
          'libevent2/http.c',
          'libevent2/evdns.c',
          'libevent2/evrpc.c',
          'libevent2/bufferevent-internal.h',
          'libevent2/changelist-internal.h',
          'libevent2/defer-internal.h',
          'libevent2/evbuffer-internal.h',
          'libevent2/event-internal.h',
          'libevent2/evmap-internal.h',
          'libevent2/evrpc-internal.h',
          'libevent2/evsignal-internal.h',
          'libevent2/evthread-internal.h',
          'libevent2/ht-internal.h',
          'libevent2/http-internal.h',
          'libevent2/ipv6-internal.h',
          'libevent2/log-internal.h',
          'libevent2/minheap-internal.h',
          'libevent2/mm-internal.h',
          'libevent2/ratelim-internal.h',
          'libevent2/strlcpy-internal.h',
          'libevent2/time-internal.h',
          'libevent2/util-internal.h',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
        'libevent2/include',
          '<(SHARED_INTERMEDIATE_DIR)',
        ]
      },
      'include_dirs': [
        'libevent2/include',
        '<(SHARED_INTERMEDIATE_DIR)',
      ],
      'defines': [ 'EVRPC_URI_PREFIX=\"\"' ],
       'conditions': [
			['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
			  'cflags': [ '--std=c89' ],
			  'defines': [ '_GNU_SOURCE' ],
			  'sources': [
				  'libevent2/kqueue-internal.h',
				  'libevent2/evthread_pthread.c',
				 ],
			}],
			['without_ssl=="false"', {
			  'dependencies': [
				'openssl/openssl.gyp:openssl'
			   ],
			  'defines': [ 'EVENT__HAVE_OPENSSL' ],
			  'direct_dependent_settings': { 
			      'defines': [ 'EVENT__HAVE_OPENSSL' ],
			   },
			  'sources': [
				  'libevent2/include/event2/bufferevent_ssl.h',
				  'libevent2/bufferevent_openssl.c',
			   ],
			}],
			['OS=="win"', {
			  'sources': [
                  'libevent2/iocp-internal.h',
				  'libevent2/win32select.c',
				  'libevent2/evthread_win32.c',
				  'libevent2/buffer_iocp.c',
				  'libevent2/event_iocp.c',
				  'libevent2/bufferevent_async.c',
				  'libevent2/WIN32-Code/event2/event-config.h',
			  ],      
			  'include_dirs': [
				'libevent2/compat',
				'libevent2/WIN32-Code',
			  ],
              'direct_dependent_settings': {
                'include_dirs': [
				'libevent2/compat',
				'libevent2/WIN32-Code',
                  '<(SHARED_INTERMEDIATE_DIR)',
                ]
              },
			}],
       ],
    }, # end libevent2
    {
      'target_name': 'libevent2_core',
      'type': 'static_library',
      'sources': [
          'libevent2/include/event2/buffer.h',
          'libevent2/include/event2/bufferevent.h',
          'libevent2/include/event2/bufferevent_compat.h',
          'libevent2/include/event2/bufferevent_struct.h',
          'libevent2/include/event2/buffer_compat.h',
          'libevent2/include/event2/event.h',
          'libevent2/include/event2/event_compat.h',
          'libevent2/include/event2/event_struct.h',
          'libevent2/include/event2/keyvalq_struct.h',
          'libevent2/include/event2/listener.h',
          'libevent2/include/event2/thread.h',
          'libevent2/include/event2/util.h',
          'libevent2/bufferevent-internal.h',
          'libevent2/changelist-internal.h',
          'libevent2/defer-internal.h',
          'libevent2/evbuffer-internal.h',
          'libevent2/event-internal.h',
          'libevent2/evmap-internal.h',
          'libevent2/evsignal-internal.h',
          'libevent2/evthread-internal.h',
          'libevent2/ht-internal.h',
          'libevent2/ipv6-internal.h',
          'libevent2/log-internal.h',
          'libevent2/minheap-internal.h',
          'libevent2/mm-internal.h',
          'libevent2/ratelim-internal.h',
          'libevent2/strlcpy-internal.h',
          'libevent2/time-internal.h',
          'libevent2/util-internal.h',
          'libevent2/event.c',
          'libevent2/buffer.c',
          'libevent2/bufferevent.c',
          'libevent2/bufferevent_sock.c',
          'libevent2/bufferevent_pair.c',
          'libevent2/listener.c',
          'libevent2/evmap.c',
          'libevent2/log.c',
          'libevent2/evutil.c',
          'libevent2/evutil_rand.c',
          'libevent2/evutil_time.c',
          'libevent2/strlcpy.c',
          'libevent2/signal.c',
          'libevent2/bufferevent_filter.c',
          'libevent2/evthread.c',
          'libevent2/bufferevent_ratelim.c',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
        'libevent2/include',
          '<(SHARED_INTERMEDIATE_DIR)',
        ]
      },
      'include_dirs': [
        'libevent2/include',
        '<(SHARED_INTERMEDIATE_DIR)',
      ],
      'conditions': [
			['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
			  'cflags': [ '--std=c89' ],
			  'defines': [ '_GNU_SOURCE' ]
			}],
			['OS=="win"', {
			  'sources': [
				  'libevent2/win32select.c',
				  'libevent2/evthread_win32.c',
				  'libevent2/buffer_iocp.c',
				  'libevent2/event_iocp.c',
				  'libevent2/bufferevent_async.c',
			  ],      
			  'include_dirs': [
				'libevent2/compat',
				'libevent2/WIN32-Code',
			  ],
              'direct_dependent_settings': {
                'include_dirs': [
				'libevent2/compat',
				'libevent2/WIN32-Code',
                  '<(SHARED_INTERMEDIATE_DIR)',
                ]
              },
			}],
			['without_ssl=="false"', {
			  'dependencies': [
				'openssl/openssl.gyp:openssl'
			   ],
			  'sources': [
				  'libevent2/include/event2/bufferevent_ssl.h',
				  'libevent2/bufferevent_openssl.c',
			   ],
			}],
       ],
    }, # end libevent2_core
    {
      'target_name': 'libevent2_extras',
      'type': 'static_library',
      'sources': [
          'libevent2/include/event2/dns.h',
          'libevent2/include/event2/dns_compat.h',
          'libevent2/include/event2/dns_struct.h',
          'libevent2/include/event2/http.h',
          'libevent2/include/event2/http_compat.h',
          'libevent2/include/event2/http_struct.h',
          'libevent2/include/event2/rpc.h',
          'libevent2/include/event2/rpc_compat.h',
          'libevent2/include/event2/rpc_struct.h',
          'libevent2/include/event2/tag.h',
          'libevent2/include/event2/tag_compat.h',
          'libevent2/evrpc-internal.h',
          'libevent2/http-internal.h',
          'libevent2/event_tagging.c',
          'libevent2/http.c',
          'libevent2/evdns.c',
          'libevent2/evrpc.c',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)',
        ]
      },
      'include_dirs': [
        'libevent2/include',
        '<(SHARED_INTERMEDIATE_DIR)',
      ],
      'defines': [ 'EVRPC_URI_PREFIX=\"\"' ],
       'conditions': [
			['OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
			  'cflags': [ '--std=c89' ],
			  'defines': [ '_GNU_SOURCE' ]
			}],
			['OS=="win"', {   
			  'include_dirs': [
				'libevent2/compat',
				'libevent2/WIN32-Code',
			  ],
			}],
       ],
    }, # end libevent2_extras
  ] # end targets
}
