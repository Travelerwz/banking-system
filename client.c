#include "utili.h"


//输入时间的年月日时分秒转化成秒数
time_t convert_str_to_tm(char * str_time)
{
	struct tm tt;
	memset(&tt,0,sizeof(tt));
	tt.tm_year=atoi(str_time)-1900;
	tt.tm_mon=atoi(str_time+5)-1;
	tt.tm_mday=atoi(str_time+8);
	tt.tm_hour=atoi(str_time+11);
	tt.tm_min=atoi(str_time+14);
	tt.tm_sec=atoi(str_time+17);
	return mktime(&tt);
}





//登陆显示
void Main_Menu()
{
	system("clear");
	printf("********************************************\n");
	printf("**              1.Sign in                 **\n");
	printf("**           	2.Sign out                **\n");
	printf("********************************************\n");
	printf("**         Please input instruction       **\n");
	printf("********************************************\n");
}


//界面显示
void Manager_Menu()
{
	system("clear");
	printf("*********************************************\n");
	printf("**       1.Add personnel information       **\n");
	printf("**       2.Delete personnel information    **\n");
	printf("**       3.Change personnel information    **\n");
	printf("**       4.Query personnel information     **\n");
	printf("**       5.Save money                      **\n");
	printf("**       6.Get monet                       **\n");
	printf("**       7.Transfer money                  **\n");
	printf("**       8.Finding information by personnel**\n");
	printf("**       9.Finding information by time     **\n");
	printf("*********************************************\n");
	printf("**         Please input instruction        **\n");
	printf("*********************************************\n");
}


//ok,添加用户
void Person_Add(int socketfd)
{   
    struct Msg msg;
    memset(&msg, 0, sizeof(msg));
    strcpy(msg.operate ,"Person_Add");
    printf("operator:%s\n",msg.operate);
    char passwd_temp[20];
    system("clear");
//	printf("please int put money");
    printf("please input name锛歕n");
    scanf("%s",msg.name);
    printf("please input password锛歕n");
    scanf("%s",msg.passwd);
    printf("please input password again锛歕n");
    scanf("%s",passwd_temp);
    if(strcmp(passwd_temp,msg.passwd))
    {
    	printf("passwd not same\n");
    	return ;
    }
	printf("please input money:\n");
	scanf("%d",&msg.money);
	int seconds = time((time_t*)NULL);
	
	printf("msg write:%s,%s,%s,%d\n",msg.name,msg.passwd,msg.operate,msg.money);
    write(socketfd, &msg, sizeof(msg));
    memset(&msg, 0, sizeof(msg));
    read(socketfd, &msg, sizeof(msg));
    printf("%s\n",msg.result);
    if(!strcmp(msg.result,"ok"))
    {
        system("clear");
        printf("add ok\n");
        Menu_Seletc(socketfd);
    }
    else 
    {
        system("clear");
        printf("add error锛乗n");
        Menu_Seletc(socketfd);
    }
        return;

}



//ok，删除用户
void Person_Delete(int socketfd )
{

    struct Msg msg;
    memset(&msg, 0, sizeof(msg));
    strcpy(msg.operate ,"Person_Delete");
    printf("please input delete name锛歕n");
    scanf("%s",msg.name);
	printf("write msg:%s\n",msg.name);
	printf("please input delete name's passwd :\n");
    scanf("%s",msg.passwd);
	printf("write msg:%s,%s\n",msg.name,msg.passwd);
    write(socketfd, &msg, sizeof(msg));
    memset(&msg, 0, sizeof(msg));
    read(socketfd, &msg, sizeof(msg));
	printf("read msg:%s,%s\n",msg.name,msg.passwd);
    printf("%s\n",msg.result);
    if(!strcmp(msg.result,"ok"))
    {
        
        printf("delete ok\n");
        Menu_Seletc(socketfd);
    }
    else 
    {
        system("clear");
        printf("delete error\n");
        Menu_Seletc(socketfd);
    }
    return;

}




//修改用户密码
void Person_Change(int socketfd )
{
	 struct Msg msg;
	 memset(&msg, 0, sizeof(msg));
	 strcpy(msg.operate ,"Person_Change");
	 printf("operate:%s\n",msg.operate);
	 char passwd_temp[20];
	 system("clear");
	 printf("please input change passwd's name:\n");
	 scanf("%s",msg.name);
	
	printf("please intput name passwd:\n");
	scanf("%s",msg.passwd);
	
	printf("please intput new name's passwd:\n");
	scanf("%s",msg.passwd2);

	printf("write msg:%s,%s,%s,%s:\n",msg.name,msg.passwd,msg.passwd2,msg.operate);
	write(socketfd, &msg, sizeof(msg));
	memset(&msg, 0, sizeof(msg));
	read(socketfd, &msg, sizeof(msg));
	printf("%s\n",msg.result);
	if(!strcmp(msg.result,"ok"))
	{
		system("clear");
		printf("change ok\n");
		Menu_Seletc(socketfd);
	}
	else 
	{
		system("clear");
		printf("change error锛侊紒\n");
		Menu_Seletc(socketfd);
	}
	return;

}


//ok查询用户，打印用户名，密码，金额
void Person_Query(int socketfd )
{
	 struct Msg msg;
	 memset(&msg, 0, sizeof(msg));
	 strcpy(msg.operate ,"Person_Query");
	 printf("please input query name:\n");
	 scanf("%s",msg.name);
	 printf("please intput name's  passwd:\n");
	 scanf("%s",msg.passwd);
	 write(socketfd, &msg, sizeof(msg));
	 memset(&msg, 0, sizeof(msg));
	 read(socketfd, &msg, sizeof(msg));
	 printf("%s\n",msg.result);
	 if(!strcmp(msg.result,"ok"))
	 {
	 	system("clear");
		printf("query ok\n");
		printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
		Menu_Seletc(socketfd);
	}
	else 
	{
		system("clear");
		printf("query error\n");
		Menu_Seletc(socketfd);
	}
	return;
}


//存钱
void Save_Money(int socketfd )
{
	
	struct Msg msg;
	memset(&msg, 0, sizeof(msg));
	strcpy(msg.operate ,"save_money");
	printf("please input save_money name:\n");
	scanf("%s",msg.name);
	printf("please intput name's  passwd:\n");
	scanf("%s",msg.passwd);
	printf("please input save_money number:\n");
	scanf("%d",&msg.money);
	printf("write msg:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
	write(socketfd, &msg, sizeof(msg));
	memset(&msg, 0, sizeof(msg));
	read(socketfd, &msg, sizeof(msg));
	printf("%s\n",msg.result);
	if(!strcmp(msg.result,"ok"))
	{
		system("clear");
		printf("save_money OK");
		printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
		Menu_Seletc(socketfd);
	}
	else 
	{
		system("clear");
		printf("save money error锛侊紒锛乗n");
		Menu_Seletc(socketfd);
	}
	return;
}


//取钱
void Get_Money(int socketfd )
{
	
	struct Msg msg;
	memset(&msg, 0, sizeof(msg));
	strcpy(msg.operate ,"get_money");
	printf("please input get_money name:\n");
	scanf("%s",msg.name);
	printf("please intput name's  passwd:\n");
	scanf("%s",msg.passwd);
	printf("please input get_money number:\n");
	scanf("%d",&msg.money);
	write(socketfd, &msg, sizeof(msg));
	memset(&msg, 0, sizeof(msg));
	read(socketfd, &msg, sizeof(msg));
	printf("%s\n",msg.result);
	if(!strcmp(msg.result,"ok"))
	{
		system("clear");
		printf("get_money OK");
		printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
		Menu_Seletc(socketfd);
	}
	else 
	{
		system("clear");
		printf("save money error:\n");
		Menu_Seletc(socketfd);
	}
	return;
}



//转账业务
//jie yong passwd2 zuowei name2
void Trans_Money(int socketfd )
{
	char **str;
	struct Msg msg;
	memset(&msg, 0, sizeof(msg));
	strcpy(msg.operate ,"trans_money");
	printf("please input transe_money hostname:\n");
	scanf("%s",msg.name);
	printf("please input transe_money hostname's passwd:\n");
	scanf("%s",msg.passwd);
	//借用passwd2来充当slavename
	printf("please input transe_money slavename:\n");
	scanf("%s",msg.passwd2);
	printf("please input transe_money number:\n");
	scanf("%d",&msg.money);
	write(socketfd, &msg, sizeof(msg));
	memset(&msg, 0, sizeof(msg));
	read(socketfd, &msg, sizeof(msg));
	
//	read(socketfd, str, sizeof(str));
//	printf("str:%s\n",str[1][1]);
	printf("%s\n",msg.result);
	if(!strcmp(msg.result,"ok"))
	{
		system("clear");
		printf("transe money OK");
		printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
		printf("result:name:%s,money:%d\n",msg.passwd2,msg.tt);
		Menu_Seletc(socketfd);
	}
	else 
	{
		system("clear");
		printf("transe  money error锛侊紒锛乗n");
		Menu_Seletc(socketfd);
	}
	return;

}


//按名字查找用户密码，金额
void Find_name(int socketfd)
{

	 struct Msg msg;
	 memset(&msg, 0, sizeof(msg));
	 strcpy(msg.operate ,"find_name");
	 printf("please input name:/n");
	 scanf("%s",msg.name);
	 write(socketfd, &msg, sizeof(msg));
	 memset(&msg, 0, sizeof(msg));
	 read(socketfd, &msg, sizeof(msg));
	 printf("%s\n",msg.result);
	 if(!strcmp(msg.result,"ok"))
	 {
	 	system("clear");
		printf("find ok\n");
		printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
		Menu_Seletc(socketfd);
	}
	else 
	{
		system("clear");
		printf("query error:\n");
		Menu_Seletc(socketfd);
	}
	return;
}



//按时间来查找用户信息
void Find_Time(int socketfd)
{
//	char **str;
	struct Msg msg;
//	int temp;
	memset(&msg, 0, sizeof(msg));

	strcpy(msg.operate ,"find_time");
	//jie yong passwd ziduan
	printf("input example:2011-12-31,11:43:07\n");
	//借用passwd和passwd2两个字段来表示时间
	printf("please input begin_time:\n");
	scanf("%s",msg.passwd);
	printf("please input end_time:\n");
	scanf("%s",msg.passwd2);
	msg.tt = convert_str_to_tm(msg.passwd);
	msg.money = convert_str_to_tm(msg.passwd2);

	printf("Begin_time:%d,Last_time:%d\n",msg.tt,msg.money);
	int nrow=0;
	write(socketfd, &msg, sizeof(msg));
	read(socketfd, &nrow, sizeof(msg));
	printf("nrow:%d\n",nrow);
	int i;
	for(i=0;i<nrow;i++)
	{
		memset(&msg, 0, sizeof(msg));
		read(socketfd, &msg, sizeof(msg));
//		printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
//		printf("%s\n",msg.result);

		if(!strcmp(msg.result,"ok"))
		{
			system("clear");
//			printf("query ok\n");
			printf("result:name:%s,password:%s,money:%d\n",msg.name,msg.passwd,msg.money);
		//	Menu_Seletc(socketfd);
			
		}
		else 
		{
			system("clear");
			printf("query error:\n");
//	   		Menu_Seletc(socketfd);
		}
		if(i == nrow)
		{
			Menu_Seletc(socketfd);
		}
	}
	
	Menu_Seletc(socketfd);

	return;
}


//调用相应的操作函数
void Menu_Seletc(int socketfd )
{
    int temp=0;
    Manager_Menu();
    scanf("%d",&temp);
    while(getchar() != '\n');
    switch(temp)
        {
            case 1: 
				Person_Add(socketfd);
                break;
            case 2: 
				Person_Delete(socketfd );
                break;
            case 3:     // 
                Person_Change(socketfd );
                break;
            case 4:     // 
                Person_Query(socketfd );
                break;
            case 5:     // 
                Save_Money(socketfd );
                break;
			case 6:     // 
                Get_Money(socketfd );
                break;	
            case 7:     // 
                Trans_Money(socketfd);
                break;
            case 8:     // 
                Find_name(socketfd);
                break;
            case 9:     // 
                Find_Time(socketfd );
                break;
            default: 
                system("clear");
            break;
        
        }


}



//客户端登录
void sign_in(int socketfd)
{
    struct Msg msg;
    memset(&msg, 0, sizeof(msg));
    strcpy(msg.operate ,"sign_in");
    system("clear");
    printf("please input name:\n");
    scanf("%s",msg.name);
    while(getchar() != '\n');
    printf("please input password:\n");
    scanf("%s",msg.passwd);
    while(getchar() != '\n');
    
    write(socketfd, &msg, sizeof(msg));
    memset(&msg, 0, sizeof(msg));
    read(socketfd, &msg, sizeof(msg));

    printf("read msg:%s, %s, %s\n", msg.name, msg.passwd, msg.operate);
    printf("%s\n",msg.result);
    if(!strcmp(msg.result,"ok"))
    {
        system("clear");
        printf("login sucess\n");
        Menu_Seletc(socketfd);
    }
    else 
    {
       	system("clear");
        printf("login error\n");
    }
 	return ;
}


//登录界面
void Log_in(int socketfd)
{
    int temp;
	Main_Menu();
    scanf("%d",&temp);
    while(getchar() != '\n');
    switch(temp)
    {
        case 1:     
            sign_in(socketfd);
            break;
        case 2:     
            system("clear");
            return;
        default: 
            system("clear");
            sleep(10);
        break;
    
    }

}



//主函数
int main()
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( -1== socketfd )
    {
        perror ("socket");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port= htons(PORT);
    inet_aton("127.0.0.1",&(addr.sin_addr));

    int ret = connect(socketfd, (struct sockaddr *)&addr, sizeof(addr));
    if ( -1== ret )
    {
        perror ("connect");
        return -1;
    }

    printf ("connect sucess--------->>>>>\n");

    Log_in(socketfd);

    close(socketfd);

    return 0;
}

