/**
  *	Simple TCP client to fetch a web page
  */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


static char *google_news_url = "https://news.google.com/rss/topics/CAAqJggKIiBDQkFTRWdvSUwyMHZNRGx1YlY4U0FtVnVHZ0pWVXlnQVAB?hl=en-US&gl=US&ceid=US:en";
static char *bbc_news_url = "http://feeds.bbci.co.uk/news/world/rss.xml";

int main(int argc,
	 char *argv[])
{
  int socket_desc;
  struct sockaddr_in server;
  char *message , server_reply[6000];
  
  // Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1)
  {
    printf("Couldnt create socke\n");
  }
	
  //ip address of www.msn.com (get by doing a ping www.msn.com at terminal)

//  hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 

  struct hostent *host_entry; 
  host_entry = gethostbyname("www.google.com"); 
  
  server.sin_addr.s_addr = inet_addr("www.google.com");
  server.sin_family = AF_INET;
  server.sin_port = htons( 80 );
  
  //Connect to remote server
  if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
  {
    puts("connect error");
    return 1;
  }
  
  puts("Connected\n");
  
  //Send some data
  message = "GET /?st=1 HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
  if( send(socket_desc , message , strlen(message) , 0) < 0)
  {
    puts("Send failed");
    return 1;
  }
  puts("Data Send\n");
  
  //Receive a reply from the server
  if( recv(socket_desc, server_reply , 6000 , 0) < 0)
  {
    puts("recv failed");
  }
  puts("Reply received\n");
  puts(server_reply);
  
  return 0;
}
