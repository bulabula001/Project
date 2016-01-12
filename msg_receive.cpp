#include <stdio.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <errno.h>  
#include<stdlib.h>
#include<string.h>


#include <stdio.h>                                                                                            
#include<stdlib.h>
#include "cJSON.h"

#define MSGKEY 1024  
  
#include<iostream>
using namespace std;
//MYSQL


#include <mysql/mysql.h>

MYSQL 		*g_conn; // mysql 连接
MYSQL_RES 	*g_res; // mysql 记录集
MYSQL_ROW 	g_row; // 字符串数组，mysql 记录行
 
#define MAX_BUF_SIZE 1024 // 缓冲区最大字节数

const char *g_host_name = "192.168.10.118";
const char *g_user_name = "root";
const char *g_password = "123456";
const char *g_db_name = "test_c";
const unsigned int g_db_port = 3306;

void print_mysql_error(const char *msg) { // 打印最后一次错误
    if (msg)
        printf("%s: %s\n", msg, mysql_error(g_conn));
    else
        puts(mysql_error(g_conn));
}

int executesql(const char * sql) {
    /*query the database according the sql*/
    if (mysql_real_query(g_conn, sql, strlen(sql))) // 如果失败
        return -1; // 表示失败

    return 0; // 成功执行
}


int init_mysql() { // 初始化连接
    // init the database connection
    g_conn = mysql_init(NULL);

    /* connect the database */
    if(!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, g_db_port, NULL, 0)) // 如果失败
        return -1;

    // 是否连接已经可用
    if (executesql("set names utf8")) // 如果失败
        return -1;

    return 0; // 返回成功
}

//START_MYSQL
int init_msg(){

	printf("---------------------------\n");
    puts("       LOAD MYSQL         "); /* prints !!!Hello World!!! */
	printf("---------------------------\n");

    if (init_mysql());
        print_mysql_error(NULL);

    char sql[MAX_BUF_SIZE];
    sprintf(sql, "select * from `d_IP` order by id");

    if (executesql(sql)) // 句末没有分号
       {
		 print_mysql_error(NULL);
		}

	printf("it is here: 2!\n");
    g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

    int iNum_rows = mysql_num_rows(g_res); // 得到记录的行数
    int iNum_fields = mysql_num_fields(g_res); // 得到记录的列数
	
	printf("---------------------------\n");
    printf("共 %d 个记录，每个记录 %d 字段\n", iNum_rows, iNum_fields);
	printf("---------------------------\n");

    //puts("id\tusername\tpassword");
	//printf("---------------------------\n");

    while ((g_row=mysql_fetch_row(g_res))) // 打印结果集
	{
        printf("%s\t%s\t%s\t%s\n", g_row[0], g_row[1], g_row[2], g_row[3]); // 第一，第二字段
		printf("deal the mysql result here!\n");
	}
	printf("---------------------------\n");

    mysql_free_result(g_res); // 释放结果集

    mysql_close(g_conn); // 关闭链接

    //return EXIT_SUCCESS;
	return 0;
}
//END_MYSQL


//struct msgstru  
//{  
//   long msgtype;  
//   char msgtext[2048];  
//  
//};  
  

struct msgstru  
{  
   int  categary; 
   char version[2048];  
  
};  

void msg_format(){

	printf("msg format working here !\n");	
	printf("---------------------------\n");

}

main()  
{  
	int imsgID;  
	struct msgstru slQMsg,slRcvMsg;  
	int ilrc;  
	
	int  cat;
	char str[2048];  
	


	//mysq-start
	printf("---------------------------\n");
    puts("        START         "); /* prints !!!Hello World!!! */
	printf("---------------------------\n");
	init_msg();
	//mysq-end

	msg_format();
	
	imsgID=msgget(MSGKEY,IPC_EXCL );//检查消息队列是否存在  
	if(imsgID < 0){  
	      printf("消息不存在! errno=%d [%s]\n",errno,strerror(errno));  
	      exit(-1);  
	  }  

	while(1){	
		 //ilrc = msgrcv(imsgID,&slRcvMsg,sizeof(struct msgstru),0,0);/*接收消息队列*/  
		 ilrc = msgrcv(imsgID,&slRcvMsg,sizeof(struct msgstru),0,0);/*接收消息队列*/  
	
		  if ( ilrc < 0 ) {  
		       printf("msgsnd()读消息队列失败,errno=%d[%s]\n",errno,strerror(errno));  
		       exit(-1);  
		  
		  }else{
			   printf("消息队列read成功\n");
			  
		  }
		//free(MSG_R);
		printf("TYPE	=	[%d]\n",slRcvMsg.categary);  
		printf("VERSION	=	[%s]\n",slRcvMsg.version);  
		
		//	 msgctl(imsgID,IPC_RMID,0); //删除消息队列  
	
		//mysql
		printf("---------------------------\n");
	    puts("        UPDATE        "); /* prints !!!Hello World!!! */
		printf("---------------------------\n");
		init_msg();
		//end-mysql
	
		msg_format();
	}
	return 0;
}  
