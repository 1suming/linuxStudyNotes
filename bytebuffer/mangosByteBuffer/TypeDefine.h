/*TypeDefine.h 
*/

#ifndef  _TYPEDEFINE_H
#define  _TYPEDEFINE_H

#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>


#define SERVER_PORT		20000
#define SERVER_IP		"127.0.0.1"
#define BUFFER_SIZE		255

typedef struct sockaddr SA;




#ifdef _STLP_HASH_MAP
	#define HashMap ::std::hash_map
#else
	#define HashMap ::stdext::hash_map
#endif

#endif