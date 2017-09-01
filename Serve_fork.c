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
#include <signal.h>
/*
服务器端程序包括
Ø  建立套接字（ socket()）
Ø  套接字与端口的绑定(bind())
Ø  设置服务器的侦听连接（listen()）
Ø  接收客户端连接（accept()）
Ø  接收和发送数据（send(),recv()）
Ø  关闭套接字(close())
*/

int main(int argc, char *argv[]) /*　传入服务端的ip和端口　*/
{
	struct sockaddr_in addr_Myself;
	struct sockaddr_in addr_Client; 
	int Listen_sockfd,Accepcted_fd;

	int n;
	
	signal(SIGCHLD, SIG_IGN); //等待全部子进程结束
	
	/*确认有参数*/
	if(argc!=3) 
	{ 
		printf("parameter error"); /*　argv[1] 是目的ip argv[2]是目的端口　*/
		exit(1); 
	}
	/* 客户程序开始建立 Listen_sockfd描述符 */
	if((Listen_sockfd=socket(AF_INET,SOCK_STREAM,0))== -1)
	{
		perror("socket");
	}
	
	/* 客户程序填充服务端的资料 */ 
	memset(&addr_Myself,0,sizeof(addr_Myself)); 	/* 设置结构体初始化为0 */
	addr_Myself.sin_family= AF_INET;	/*协议簇*/
	addr_Myself.sin_port= htons((unsigned short)atoi(argv[2]));		/* 服务端口 */
	addr_Myself.sin_addr.s_addr=htonl(INADDR_ANY);					/* 本机ip */
	
	 n=1;
 	/* 如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间  ctrl+z主动关闭服务端 需要等待30s才能侦听同样的端口*/
 	setsockopt(Listen_sockfd,SOL_SOCKET,SO_REUSEADDR,&n,sizeof(int));
	
	/* 服务端绑定 */
	if(bind(Listen_sockfd,(struct sockaddr *)&addr_Myself,sizeof(struct sockaddr_in)) <0)
	{
		   perror("bind");
		   exit(1);
	}
	/* backlog 存在的意义就是：在这段时间里面除了第一个连接请求是正在进行处理以外，其他的连接请求都在请求队列中等待，而如果超过了队列的最大等待个数时，其他的请求将被忽略或者将不会被处理。 */ 
	if(listen(Listen_sockfd,5)== -1)
	{
		perror("listen");
		exit(1);
	}
	
	while(1) 
	{ 
		/* 服务器阻塞,直到客户程序建立连接 */ 
		socklen_t sin_size=sizeof(struct sockaddr_in); 
		if((Accepcted_fd=accept(Listen_sockfd,(struct sockaddr *)(&addr_Client),&sin_size))==-1) 
		{ 
			perror("accept");
			exit(1); 
		} 
		pid_t Child_pid;
		Child_pid=fork();
		if(Child_pid < 0)
		{
			perror("Creat Chiled process error\n");
			continue;
		}
		else if(Child_pid == 0) /*接收客户端的数据*/
		{
			/*child process*/
			char buffer[1024]={0};
			int nbytes;			
			printf("Server get connection from %s.It's port is %d\n",inet_ntoa(addr_Client.sin_addr),ntohs(addr_Client.sin_port));/* 将网络地址转换成.字符串 */
			//printf("sizeof =%d\n",sizeof(buffer));
			if(( nbytes= read(Accepcted_fd,buffer,sizeof(buffer)))== -1)
			{
				perror("Read error\n");
			}
			else
			{
				buffer[nbytes]='\0';
				printf("Server received string is :%s\n",buffer);
			}
			
			close(Child_pid);
			close(Listen_sockfd);
			exit(0);
		}
		else
		{
			/*Parent process*/
			//close(Listen_sockfd);
		}
		
	}

	return 0;
}