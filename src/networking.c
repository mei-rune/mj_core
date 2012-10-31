
#include "meijing/networking.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAS_INET_NTOP

const char * os_inet_ntop(int af, const void *src, char *dst, size_t size);
int os_inet_pton(int af,  const char *src,  void *dst);

#else

#define os_inet_ntop inet_ntop
#define os_inet_pton inet_pton

#endif


#define sys_call(func)   (SOCKET_ERROR != (func))?true:false




DLL_VARIABLE boolean mj_socket_set_nonblocking(SOCKET sock)
{
#ifdef _WIN32
    u_long nonblock = 1;
    return sys_call(SOCKET_ERROR != ioctlsocket(sock,
                              FIONBIO,
                              &nonblock));
#else
	int flag = fcntl(fd,F_GETFL);
	if(-1 == flag)
		return false;

    return sys_call(-1 == fcntl(fd,F_SETFL, flag | O_NONBLOCK)));
#endif 
}

DLL_VARIABLE boolean  mj_initialize_networks()
{
#ifdef _WIN32
    DWORD dwBytes = 0;

    WSADATA wsaData;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
        return false;

    if (LOBYTE(wsaData.wVersion) != 2 ||
            HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        return false;
    }
#endif

    return true;
}

DLL_VARIABLE void mj_shutdown_networks()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

DLL_VARIABLE boolean mj_string_to_sockaddr(const char* url
                     , struct sockaddr* addr)
{
	char host[50];
	const char* ptr = url;
	const char* end = NULL;

	if('[' == *ptr)
	{
		// 处理 ipv6的地址，格式如 [xxxx:xxxx::xxx]:port
		addr->sa_family = AF_INET6;

		if(NULL == (end = strchr(++ptr, ']')))
			return false;

		if( ':' != *(end + 1))
			return false;


		strncpy(host, ptr, end-ptr);
		host[end-ptr] =0;
		if(1 != os_inet_pton(AF_INET6, host, &(((struct sockaddr_in6*)addr)->sin6_addr)))
			return false;

		++ end;

		((struct sockaddr_in6*)addr)->sin6_port = htons(atoi(end));
		return true;
	}
	else
	{
		// 处理 ipv4的地址，格式如  xxx.xxx.xxx.xxx:port
		addr->sa_family = AF_INET;

		if(NULL == (end = strchr(ptr, ':')))
			return false;

		strncpy(host, ptr, end-ptr);
		host[end-ptr] =0;
		if(1 != os_inet_pton(AF_INET, host, &(((struct sockaddr_in*)addr)->sin_addr)))
			return false;

		++ end;

		((struct sockaddr_in*)addr)->sin_port = htons(atoi(end));
		return true;
	}
}
//
//DLL_VARIABLE boolean mj_sockaddr_to_string(struct sockaddr* addr
//                     , const char* schema
//                     , size_t schema_len
//                     , mj_buffer_t* url)
//{
//
//	size_t len = 0;
//	const char* ptr = 0;
//
//    if(NULL == schema)
//        mj_buffer_assignLen(url, "tcp", 3);
//    else if(-1 == schema_len)
//        mj_buffer_assignLen(url, schema, strlen(schema));
//    else
//        mj_buffer_assignLen(url, schema, schema_len);
//
//
//	if(addr->sa_family == AF_INET6)
//		mj_buffer_appendLen(url, "6", 1);
//
//	mj_buffer_appendLen(url, "://", 3);
//	len = string_length(url);
//
//	mj_buffer_appendN(url, 0, IP_ADDRESS_LEN);
//
//	if(AF_INET6 == addr->sa_family)
//		ptr = os_inet_ntop(addr->sa_family,  &(((struct sockaddr_in6*)addr)->sin6_addr), string_data(url) + len, IP_ADDRESS_LEN);
//	else
//		ptr = os_inet_ntop(addr->sa_family,  &(((struct sockaddr_in*)addr)->sin_addr), string_data(url) + len, IP_ADDRESS_LEN);
//	if(0 == ptr)
//	{
//		string_truncate(url, 0);
//		return false;
//	}
//	string_truncate(url, len + strlen(ptr));
//
//	mj_buffer_append_sprintf(url, ":%d", 
//			ntohs((AF_INET6 == addr->sa_family)?((struct sockaddr_in6*)addr)->sin6_port
//			:((struct sockaddr_in*)addr)->sin_port));
//	
//	return true;
//}


DLL_VARIABLE socket_type mj_socket_listen_at(const char* url){
	
    SOCKADDR_STORAGE addr;
    socket_type fd;
	
	fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == fd)
		return fd;

    if(!mj_socket_set_nonblocking(fd)) {
		closesocket(fd);
		return INVALID_SOCKET;
	}

    if(!mj_string_to_sockaddr(url,(struct sockaddr*) &addr)){
		closesocket(fd);
		return INVALID_SOCKET;
	}

    if(SOCKET_ERROR == bind(fd,(struct sockaddr*)&addr, sizeof(addr))) {
		closesocket(fd);
		return INVALID_SOCKET;
	}

    if(SOCKET_ERROR == listen(fd,SOMAXCONN)){
		closesocket(fd);
		return INVALID_SOCKET;
	}
    return fd;
}

#ifdef __cplusplus
}
#endif
