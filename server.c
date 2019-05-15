#include "utili.h"


//verify name and passwd
int Verify(int socketfd ,struct Msg msg)
{
    char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	strcpy(msg.result,"");
	printf("log message：%s, %s, %s\n", msg.name, msg.passwd, msg.operate);
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("open sql fail\n");
		return 0;
	}
	sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);

	if ((ret != SQLITE_OK) || (0==nrow))
	{
		printf ("fail user error：%s\n", errmsg);
		memset(&msg, 0, sizeof(msg));
		strcpy(msg.result,"error");

	}
	else if(!strcmp(resultp[ncolumn+1],msg.passwd))
	{
		printf ("log_in ok：\n");
		printf("dmsg.passwd=%s,mag.passwd=%s",resultp[ncolumn+1],msg.passwd);
		strcpy(msg.time,resultp[ncolumn+2]);
		strcpy(msg.result,"ok");

	}
	else
	{
		strcpy(msg.result,"error");
	}
	printf("send msg：%s, %s, %s,%s,%s\n", msg.result,msg.name, msg.passwd, msg.operate,msg.time);
	write(socketfd, &msg, sizeof(msg));
	
	return 0; 
} 


//Դݨ˽ߝࠢ
void create_sql()
{
    sqlite3 * db;
    int ret = sqlite3_open("Person.db", &db);
    if (ret != SQLITE_OK)
    {
        printf ("open sql fail\n");
    }

    char *errmsg = NULL;
    char *sql1 = "create table if not exists usr(name TEXT,password TEXT,money TEXT,time TEXT,operator TEXT)";
    ret = sqlite3_exec(db, sql1, NULL, NULL, &errmsg);
    if (ret != SQLITE_OK)
    {
        printf ("create table error：%s\n", errmsg);
    }

    struct Msg msg;
    strcpy(msg.name, "admin");
    strcpy(msg.passwd, "hik12345");
	strcpy(msg.operate,"add");
    msg.money = 1000;
	msg.tt = time((time_t*)NULL);
    
	char cond_sql[100];
    sprintf(cond_sql, "insert into usr values('%s','%s','%d','%d','%s')", msg.name,msg.passwd,msg.money,msg.tt,msg.operate);
    ret = sqlite3_exec(db,cond_sql, NULL, NULL, &errmsg);

    sqlite3_close(db);
}



//SocketԵʼۯ
int Socket()
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        perror ("socket");
        return -1;
    }
	
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family  = AF_INET;     
    addr.sin_port    = htons(PORT); 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    int  ret = bind(socketfd,  (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror ("bind");
        return -1;
    }

    ret = listen(socketfd, 5);
    if (ret == -1)
    {
        perror ("listen");
        return -1;
    }

    printf ("waiting--------->>>>>>>\n");
    return socketfd;
}

//acceptگ˽Եʼۯ
int  Accept(int socketfd)
{
    struct sockaddr_in client_addr; 
    int len = sizeof(client_addr);
    int listenfd = accept(socketfd, (struct sockaddr *)&client_addr,  &len);
    if (listenfd == -1)
    {
        perror ("accept");
        return -1;
    }
    return listenfd;
}

//ͭݓɋԱ
int Person_Add(int socketfd ,struct Msg msg)
{
    char **resultp = NULL;
    int nrow, ncolumn;
    char *errmsg = NULL;
    char cond_sql[100];
    char cond_sql2[100];
    strcpy(msg.result,"");
//	msg.money = 0;
    printf("msg：%s, %s, %s,%d\n", msg.name, msg.passwd, msg.operate,msg.money);
    sqlite3 * db;
    int ret = sqlite3_open("Person.db", &db);
    if (ret != SQLITE_OK)
    {
        printf ("open sql fail\n");
        return 0;
    }
    sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
    ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
    if ((ret != SQLITE_OK) )
    {
       printf ("finf user fail：%s\n", errmsg);
       strcpy(msg.result,"error");
    }
    else
    {
		msg.tt = time((time_t*)NULL);
		printf("time:%d\n",msg.tt);
        sprintf(cond_sql2, "insert into usr values('%s','%s','%d','%d','%s')", msg.name, msg.passwd, msg.money, msg.tt,msg.operate);
        ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
        sqlite3_close(db);
        strcpy(msg.result,"ok");
        write(socketfd, &msg, sizeof(msg));
        printf ("add ok\n");
    }

    return 0;

}

//ɾԽԃۧ
int Person_Delete(int socketfd ,struct Msg msg)
{
	    char **resultp = NULL;
		int nrow, ncolumn;
		char *errmsg = NULL;
		char cond_sql[100];
		char cond_sql2[100];
		strcpy(msg.result,"");
		printf("msg message：%s, %s, %s,%d\n", msg.name, msg.passwd, msg.operate,msg.money);
		sqlite3 * db;
		int ret = sqlite3_open("Person.db", &db);
		if (ret != SQLITE_OK)
		{
			printf ("fial open sqlite\n");
			return 0;
		}
		sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
		ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
//		ret = sqlite3_exec(db, cond_sql, NULL, NULL, &errmsg);
		printf("resultp[ncolumn]:%s\n",resultp[ncolumn]);
		printf("resultp[ncolumn]:%s\n",resultp[ncolumn+1]);
		
		if ((ret != SQLITE_OK) || NULL == resultp[ncolumn] || 0 !=strcmp(resultp[ncolumn+1],msg.passwd))
		{
			printf ("fail to delete：%s\n", errmsg);
			strcpy(msg.result,"error");
			sqlite3_close(db);
			write(socketfd, &msg, sizeof(msg));
			printf("delete error\n");
			sqlite3_free_table(resultp);
		}
		else
		{
			
			sprintf(cond_sql2, "delete from usr where name = '%s'", msg.name);
			ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
			sqlite3_close(db);
			strcpy(msg.result,"ok");
			write(socketfd, &msg, sizeof(msg));
			printf ("delete ok\n");
			sqlite3_free_table(resultp);
		}

}

//ўلԃۧĜë
int Person_Change(int socketfd ,struct Msg msg)
{
    char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	char cond_sql2[100];
	strcpy(msg.result,"");
	printf("msg：%s, %s, %s\n", msg.name, msg.passwd2, msg.operate);
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("fail open sql!\n");
		return 0;
	}
	sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
	printf("resultp:%s\n",resultp[ncolumn+2]);
//	ret = sqlite3_exec(db, cond_sql, NULL, NULL, &errmsg);
//	if ((ret != SQLITE_OK) )
	if ((ret != SQLITE_OK) || NULL == resultp[ncolumn] || 0 !=strcmp(resultp[ncolumn+1],msg.passwd))
	{
		printf ("find error：%s\n", errmsg);
		strcpy(msg.result,"error");
		sqlite3_close(db);
		sqlite3_free_table(resultp);
	}
	else
	{
		sprintf(cond_sql2, "update usr set password='%s' where name='%s'", msg.passwd2,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		sprintf(cond_sql2, "update usr set operator='%s' where name='%s'", msg.operate,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		sqlite3_close(db);
		strcpy(msg.result,"ok");
		write(socketfd, &msg, sizeof(msg));
		printf ("change ok\n");
		sqlite3_free_table(resultp);
	}

	return 0;

}

//өѯԃۧĻìĜëìް׮
int Person_Query(int socketfd ,struct Msg msg)
{
    char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	char cond_sql2[100];
	strcpy(msg.result,"");
	printf("msg:%s, %s, %s\n", msg.name, msg.passwd, msg.operate);
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("open sql error\n");
		return 0;
	}
	sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
//	printf("result:%s\n",resultp[nrow]);
	if ((ret != SQLITE_OK) || NULL == resultp[ncolumn] || 0 !=strcmp(resultp[ncolumn+1],msg.passwd))
	{
		printf ("fail error:%s\n", errmsg);
		strcpy(msg.result,"error");
		sqlite3_free_table(resultp);
	}
	else
	{
		strcpy(msg.name,resultp[ncolumn]);
		strcpy(msg.passwd,resultp[ncolumn+1]);
		msg.money = atoi(resultp[ncolumn+2]);
		sqlite3_close(db);
		strcpy(msg.result,"ok");
		write(socketfd, &msg, sizeof(msg));
		printf ("find ok\n");
		sqlite3_free_table(resultp);
	}

	return 0;

}


//զǮ
int Save_Money(int socketfd ,struct Msg msg)
{
	
	char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	char cond_sql2[100];
	strcpy(msg.result,"");
	printf("message:%s, %s, %s,%d\n", msg.name, msg.passwd, msg.operate,msg.money);
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("open sql error\n");
		return 0;
	}
	int temp;
//	temp = msg.money;
	sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
	if ((ret != SQLITE_OK) || NULL == resultp[ncolumn] || 0 !=strcmp(resultp[ncolumn+1],msg.passwd))
	{
		printf ("find user error:%s\n", errmsg);
		strcpy(msg.result,"error");
		sqlite3_free_table(resultp);
	}
	else
	{
		temp= atoi(resultp[ncolumn+2]);
		printf("temp:%d\n",temp);
		msg.money = msg.money+temp;
		printf("msg.money:%d\n",msg.money);
		sprintf(cond_sql2, "update usr set money='%d' where name='%s'" , msg.money,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		sprintf(cond_sql2, "update usr set operator='%s' where name='%s'", msg.operate,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		strcpy(msg.name,resultp[ncolumn]);
		strcpy(msg.passwd,resultp[ncolumn+1]);
		//msg.money = atoi(resultp[ncolumn+2]);
		printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
		sqlite3_close(db);
		strcpy(msg.result,"ok");
		write(socketfd, &msg, sizeof(msg));
		printf ("save money ok\n");
		sqlite3_free_table(resultp);
	}

	return 0;
}


//ȡǮ
int Get_Money(int socketfd ,struct Msg msg)
{
	
	char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	char cond_sql2[100];
	strcpy(msg.result,"");
	printf("message:%s, %s, %s,%d\n", msg.name, msg.passwd, msg.operate,msg.money);
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("open sql error\n");
		return 0;
	}
	int temp;
//	temp = msg.money;
	sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
	if ((ret != SQLITE_OK) || NULL == resultp[ncolumn] || 0 !=strcmp(resultp[ncolumn+1],msg.passwd))
	{
		printf ("find user error：%s\n", errmsg);
		strcpy(msg.result,"error");
		sqlite3_free_table(resultp);
	}
	else
	{
		temp= atoi(resultp[ncolumn+2]);
		printf("temp:%d\n",temp);
		msg.money = temp-msg.money;
		if(msg.money < 0)
		{
			strcpy(msg.result,"error");
			printf("money is error\n");
		}
		printf("msg.money:%d\n",msg.money);
		sprintf(cond_sql2, "update usr set money='%d' where name='%s'" , msg.money,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		sprintf(cond_sql2, "update usr set operator='%s' where name='%s'", msg.operate,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		strcpy(msg.name,resultp[ncolumn]);
		strcpy(msg.passwd,resultp[ncolumn+1]);
	//	msg.money = atoi(resultp[ncolumn+2]);
		sqlite3_close(db);
		strcpy(msg.result,"ok");
		write(socketfd, &msg, sizeof(msg));
		printf ("save money ok\n");
		sqlite3_free_table(resultp);
	}

	return 0;
}
/*
int Get_Money(int socketfd ,struct Msg msg)
{

	char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	char cond_sql2[100];
	strcpy(msg.result,"");
	printf("message：%s, %s, %s,%d\n", msg.name, msg.passwd, msg.operate,msg.money);
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("open sql error\n");
		return 0;
	}
	sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
	if ((ret != SQLITE_OK) || strcmp(msg.name,resultp[ncolumn]) )
	{
		printf ("find user error：%s\n", errmsg);
		strcpy(msg.result,"error");
	}
	else
	{
		sprintf(cond_sql2, "update user set money='%d' where name='%s'" , msg.money,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		sqlite3_close(db);
		strcpy(msg.result,"ok");
		write(socketfd, &msg, sizeof(msg));
		printf ("save money ok\n");
	}

	return 0;
}

*/


//ѻת֋քɋ
int Get_trans_money(char *name,int money,char* operater)
{
	char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	char cond_sql2[100];
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("open sql error\n");
		return 0;
	}
	int temp;
//	temp = msg.money;
	sprintf(cond_sql, "select * from usr where name = '%s'", name);	
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
	if ((ret != SQLITE_OK) || NULL == resultp[ncolumn] )
	{
		printf ("find user error:%s\n", errmsg);
		sqlite3_close(db);
		sqlite3_free_table(resultp);
		return -1;
	}
	
	temp= atoi(resultp[ncolumn+2]);
	printf("temp:%d\n",temp);
	money = money+temp;
	printf("money:%d\n",money);
	sprintf(cond_sql2, "update usr set money='%d' where name='%s'" , money,name);
	ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
	sprintf(cond_sql2, "update usr set operator='%s' where name='%s'", operater,name);
	ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
	sqlite3_free((void*)errmsg);
	sqlite3_close(db);
	sqlite3_free_table(resultp);
	return money;
}

//ת֋ҵϱ
int Trans_Money(int socketfd ,struct Msg msg)
{
	int ret2;
	int temp;
	int temp2;
	char **resultp = NULL;
	int nrow, ncolumn;
	char *errmsg = NULL;
	char cond_sql[100];
	char cond_sql2[100];
	char cond_sql3[100];

	strcpy(msg.result,"");
	//ިԃpasswd2Ԥձsalvename
	printf("message:%s, %s,%s, %s,%d\n", msg.name,msg.passwd2,msg.passwd, msg.operate,msg.money);
	sqlite3 * db;
	int ret = sqlite3_open("Person.db", &db);
	if (ret != SQLITE_OK)
	{
		printf ("open sql error\n");
		return 0;
	}

//	temp = msg.money;
	sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);	
	ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
	if ((ret != SQLITE_OK) || NULL == resultp[ncolumn] || 0 !=strcmp(resultp[ncolumn+1],msg.passwd))
	{
		printf ("find user error:%s\n", errmsg);
		strcpy(msg.result,"error");
		sqlite3_free_table(resultp);
	}
	else
	{
		temp= atoi(resultp[ncolumn+2]);
		printf("temp:%d\n",temp);
		temp2 = msg.money;
		msg.money = temp-temp2;
		if(msg.money < 0)
		{
			strcpy(msg.result,"error");
			printf("money is error\n");
		}

		printf("msg.money:%d\n",msg.money);
		sprintf(cond_sql2, "update usr set money='%d' where name='%s'" , msg.money,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);

		sprintf(cond_sql2, "update usr set operator='%s' where name='%s'", msg.operate,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
//		sprintf(cond_sql2, "update usr set money='%d' where name='%s''" , msg.money,msg.passwd2);
		strcpy(msg.name,resultp[ncolumn]);
		strcpy(msg.passwd,resultp[ncolumn+1]);
//		msg.money = atoi(resultp[ncolumn+2]);
		sqlite3_close(db);
		//ިԃpasswd2Ԥձsalvename
		//ިԃЂmsgޡٹͥքtt
//	    strcpy(operator,"ok");
		msg.tt = Get_trans_money(msg.passwd2,temp2,msg.operate);
		
		strcpy(msg.result,"ok");
		write(socketfd, &msg, sizeof(msg));
		printf ("trans money ok\n");
		sqlite3_free((void*)errmsg);
		sqlite3_free_table(resultp);
	}

	return 0;
}


////дĻؖө֒ԃۧĜëìް׮
int Find_Name(int socketfd ,struct Msg msg)
{
	
	char **resultp = NULL;
    int nrow, ncolumn;
    char *errmsg = NULL;
    char cond_sql[100];
    char cond_sql2[100];
    strcpy(msg.result,"");
    printf("msg:%s, %s\n", msg.name, msg.operate);
    sqlite3 * db;
    int ret = sqlite3_open("Person.db", &db);
    if (ret != SQLITE_OK)
    {
	   printf ("open sql error\n");
	   return 0;
    }
    sprintf(cond_sql, "select * from usr where name = '%s'", msg.name);
    ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
//    printf("result:%s\n",resultp[nrow]);
    if ((ret != SQLITE_OK) )
    {
	   printf ("fail error:%s\n", errmsg);
	   strcpy(msg.result,"error");
	   sqlite3_free_table(resultp);
    }
    else
    {
		sprintf(cond_sql2, "update usr set operator='%s' where name='%s'", msg.operate,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
	   strcpy(msg.name,resultp[ncolumn]);
	   strcpy(msg.passwd,resultp[ncolumn+1]);
	   msg.money = atoi(resultp[ncolumn+2]);
	   sqlite3_close(db);
	   strcpy(msg.result,"ok");
	   write(socketfd, &msg, sizeof(msg));
	   printf ("find time ok\n");
	   sqlite3_free_table(resultp);
    }

   return 0;
}


//дʱݤө֒ԃۧ
int Find_Time(int socketfd ,struct Msg msg)
{
	int i;
	char **resultp = NULL;
    int nrow, ncolumn;
    char *errmsg = NULL;
    char cond_sql[100];
    char cond_sql2[100];
    strcpy(msg.result,"");
    printf("msg:%d, %d, %s\n", msg.tt, msg.money, msg.operate);
    sqlite3 * db;
    int ret = sqlite3_open("Person.db", &db);
    if (ret != SQLITE_OK)
    {
	   printf ("open sql error\n");
	   return 0;
    }
    sprintf(cond_sql, "select * from usr where time > '%d' and time < '%d'",msg.tt,msg.money);
    ret = sqlite3_get_table(db, cond_sql, &resultp, &nrow, &ncolumn, &errmsg);
    printf("result:%s\n",resultp[nrow]);
    if ((ret != SQLITE_OK) )
    {
	   printf ("fail error:%s\n", errmsg);
	   strcpy(msg.result,"error");
	   sqlite3_free_table(resultp);
    }
    else
    {
		sprintf(cond_sql2, "update usr set operator='%s' where name='%s'", msg.operate,msg.name);
		ret = sqlite3_exec(db, cond_sql2, NULL, NULL, &errmsg);
		int index=1;
		write(socketfd, &nrow, sizeof(msg));
		for(i=0;i < nrow;i++ )
		{
			int j=0;
			
		    strcpy(msg.name,resultp[index*ncolumn]);
		    strcpy(msg.passwd,resultp[index*ncolumn+1]);
		    msg.money = atoi(resultp[index*ncolumn+2]);
		    index++;
			printf("index:%d\n",index);
		    
		    strcpy(msg.result,"ok");
	
		    write(socketfd, &msg, sizeof(msg));
		    memset(&msg, 0, sizeof(msg));
		}
			sqlite3_close(db);
	//	   write(socketfd, resultp, sizeof(resultp));
		   printf ("find time ok\n");
/*
		for(i=0;i < nrow;i++ )
		{
			int j=0;
			for(j=0;j<ncolumn;j++)
			{
	   printf("nrow :%d\n",nrow);
		printf("ncolumn: %d\n",ncolumn);
	   strcpy(msg.name,resultp[j]);
	   strcpy(msg.passwd,resultp[ncolumn+1]);
	   msg.money = atoi(resultp[ncolumn+2]);
	   sqlite3_close(db);
	   strcpy(msg.result,"ok");
		


	   
	   write(socketfd, &msg, sizeof(msg));
		}
		}
//	   write(socketfd, resultp, sizeof(resultp));
	   printf ("find time ok\n");


	   int i,j;
	   int index;
	   //өѯԉ٦
	   printf("ҭٱٲ %d ݇¼!\n", nrow);
	   printf("ҭٱٲ %d !\n", ncolumn);
	   // ǰ}ٶؖ׎Ϊؖ׎Ļ field0, field1, row[0][0], row[0][1], row[1][0], row[1][1] ... ...
	   // ˇһά˽ة,һˇ׾ά˽ة,״ֽ݇؅֚0,֚1քֵΪؖ׎Ļ,Ȼ۳Ӆˇؖ׎ֵ;
	   printf( "ؖ׎Ļ|ؖ׎ֵ\n");
	   printf( "%s | %s\n", resultp[0], resultp[1]);
	   index = ncolumn; 
	   for( i = 0; i < nrow ; i )
	   {
	   		printf("֚%d͵݇¼\n", i);
	  		for( j = 0 ; j < ncolumn; j++ )
	   		{
	   			printf( "ؖ׎Ļ:%s  ؖ׎ֵ:%s\n",resultp[j], resultp [index]);
	   			index++; 
	  		}
	   // dbResult քؖ׎ֵˇlѸքìՓ֚0̷ӽս֚ nColumn - 1̷ӽּˇؖ׎ĻԆìՓ֚ nColumn ̷ӽߪʼì۳Ħּˇؖ׎ֵì̼ёһٶ׾άքҭèԫͳքѐҭʾרéԃһٶҢƽքюʽ4ҭʾ
	   }
	   printf("------------------/n" );


*/

	   sqlite3_free_table(resultp);
    }

   return 0;
}



//և½ģࠩìʵЖڜmԱքև½ҩ֤
//name:hik;passwd:hik12345
int sign_in(int socketfd ,struct Msg msg)
{
	//char cond_sql[100];
	char admin_name[] = "hik";
	char admin_passwd[] = "hik12345";
	strcpy(msg.result,"");
	printf("log message：%s, %s, %s\n", msg.name, msg.passwd, msg.operate);
	if(!strcmp(admin_name,msg.name) && !strcmp(admin_passwd,msg.passwd))
	{
		strcpy(msg.result,"ok");
	}
	else
	{
		strcpy(msg.result,"error");
	}
	printf("send msg：%s, %s, %s,%s\n", msg.result,msg.name, msg.passwd, msg.operate);
	write(socketfd, &msg, sizeof(msg));
	return 0;
}


//Ɛ׏ࠍۧ׋ע̍քļ®ìȻ۳޸ѐ׵ܘ˽ֵәطהӦքگ˽

int Operation(char * operate)
{
    printf( "Operation is :%s\n", operate );

    if(!strcmp(operate,"sign_in"))
        return 1;
    if(!strcmp(operate,"Person_Add"))
        return 2;
    if(!strcmp(operate,"Person_Delete"))
        return 3;
    if(!strcmp(operate,"Person_Change"))
        return 4;
    if(!strcmp(operate,"Person_Query"))
        return 5;
    if(!strcmp(operate,"save_money"))
        return 6;
    if(!strcmp(operate,"get_money"))
        return 7;
	if(!strcmp(operate,"trans_money"))
        return 8;
	if(!strcmp(operate,"find_name"))
        return 9;
	if(!strcmp(operate,"find_time"))
        return 10;
return 0;

}

//Ԧmࠍۧ׋ע4քȫȳìʵЖנПԌ
void* thread_client(void* arg)
{
    struct Msg msg;
    int listenfd = (int)arg;
    
    while(1)
    {
        int ret = read(listenfd, &msg, sizeof(msg));
        if (ret == -1)
        {
            perror ("read");
            break;
        }
        if (ret == 0)
        {
            printf ("exit--------->>>>\n");
            break;
        }

        printf("msg：%s, %s, %s\n", msg.name, msg.passwd, msg.operate);

		printf("operate:%d\n",Operation(msg.operate));
        switch (Operation(msg.operate))
        {
            case 1:
                sign_in(listenfd,msg);
                break;
            case 2:
                Person_Add(listenfd,msg);
                break;
			case 3:
				Person_Delete(listenfd,msg);
				break;
			case 4:
				Person_Change(listenfd,msg);
				break;
			case 5:
				Person_Query(listenfd,msg);
				break;
			case 6:
				Save_Money(listenfd,msg);
				break;
			case 7:
				Get_Money(listenfd,msg);
				break;
			case 8:
				Trans_Money(listenfd,msg);
				break;
			case 9:
				Find_Name(listenfd,msg);
				break;
			case 10:
				Find_Time(listenfd,msg);
				break;
            default :
                printf("operate is error!\n");
        }
    }
    close (listenfd);
}
int main()
{
	//ПԌ̸
	pthread_mutex_t mutex; 
    create_sql();
    
    pthread_mutex_init(&mutex, NULL);

    int socketfd = Socket();
      
    while (1)
    {
        int listenfd = Accept(socketfd);
        pthread_t tid;
        pthread_create(&tid , NULL , thread_client , (void *)listenfd);	
        pthread_detach(tid); 
    }
    
    close (socketfd);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
