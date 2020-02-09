#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>

#define PORT 80    /* the port client will be connecting to */

#define MAXDATASIZE 2048 /* max number of bytes we can get at once */

int main(int argc, char *argv[])
{
  int sockfd, numbytes;  
  char buf[MAXDATASIZE];
  struct hostent *he;
  struct sockaddr_in their_addr; /* connector's address information */
  
  if(argc != 2)
  {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }

  printf("About the gethostbyname\n");
  
  if ((he=gethostbyname(argv[1])) == NULL)
  {
    herror("gethostbyname");
    exit(1);
  }

  printf("Have gothostbyname\n");
  
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket");
    exit(1);
  }

  printf("Have got socket\n");
  
  their_addr.sin_family = AF_INET;      /* host byte order */
  their_addr.sin_port = htons(PORT);    /* short, network byte order */
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

  printf("About to connect\n");
  
  if (connect(sockfd, (struct sockaddr *)&their_addr,			\
	      sizeof(struct sockaddr)) == -1)
  {
    perror("connect");
    exit(1);
  }

  printf("About to enter loop\n");
  
  while (1) 
  {

    // Construct HTTP get request here
    char *s = "GET / HTTP/1.1\r\nHost: www.qnx.com\r\n";



//    if (send(sockfd, "Hello, world!\n", 14, 0) == -1)
    if (send(sockfd, s, strlen(s)+1, 0) == -1)
    {
      perror("send");
      exit (1);
    }
       
    printf("After the send function\n");
    
    if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) 
    {
      perror("recv");
      exit(1);
    }	
    
    buf[numbytes] = '\0';
    printf("After recv. Numebr of bytes recved = %d\n", numbytes);

    if (numbytes > 0)
    {
      printf("Received in pid=%d\nText=:\n%s \n",getpid(), buf);
    }
    else
    {
      printf("recved zero bytes...exiting\n");
      exit(0);
    }
    
    sleep(1);
  }
  
  close(sockfd);

  return 0;
}
