#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
int main(int argc, char *argv[])/*　传入服务端的ip和端口　*/
{
	struct sockaddr_in addr_Serve;
	int sockfd;
	char buffer[1024]={0};
	
	/*确认有参数*/
	if(argc!=3) 
	{ 
		printf("parameter error"); /*argv[1] 是目的ip argv[2]是目的端口*/
		exit(1); 
	}
	/* 客户程序开始建立 sockfd描述符 */
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))== -1)
	{
		perror("socket");
	}
	
	
	/* 客户程序填充服务端的资料 */ 
	memset(&addr_Serve,0,sizeof(addr_Serve)); 	/*设置结构体初始化为0*/
	addr_Serve.sin_family= AF_INET;	/*协议簇*/
	addr_Serve.sin_port= htons((unsigned short)atoi(argv[2]));		/*目标端口*/ /* atoi将一个字符串转换成一个整数 */
	inet_pton(AF_INET, argv[1], &addr_Serve.sin_addr.s_addr);
	
	
	/* 客户程序发起连接请求 */ 
	if(connect(sockfd,(struct sockaddr *)(&addr_Serve),sizeof(struct sockaddr))==-1) 
	{ 
		perror("connect");
		exit(1); 
	} 

	/* 连接成功了 */ 
	printf("Please input char:\n");
	
	/* 发送数据 */
	scanf("%s",buffer); 
	write(sockfd,buffer,1024); 
	memset(&buffer,0,sizeof(buffer));
	int nbytes;
	
	if((nbytes=read(sockfd,buffer,1024)) == -1)
	{
		perror("read");
		return -1;
	}
	else
	{
		buffer[nbytes-1]='\0';
		printf("Server had received. Server Send back:%s,nbytes=%d\n",buffer,nbytes);
	}
	
	/* 结束通讯 */ 
	close(sockfd); 
	exit(0); 
	return 0;
}