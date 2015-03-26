#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>




static MYSQL *conn;
int initdb()
{
	conn = mysql_init(NULL);
	if (conn == NULL) 
	{
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}


	if (mysql_real_connect(conn, "localhost", "root","123", NULL, 0, NULL, 0) == NULL)
	{
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	if(0 >  mysql_query(conn , "use wsisgw\n"))
	{
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	return 0;
}

int insertdb(unsigned int srcip , unsigned int dstip , int type , unsigned short bindport)
{
	int rows = -1;
	char updata[512] = {0};
	char insert[512] = {0};
	char select[512] = {0};

	sprintf(select , "select * from  ws_regav where dstip=%d;\n",dstip);
	sprintf(updata , "update ws_regav set srcip=%d , avtype=%d , bindport=%d where dstip=%d;\n",srcip,type,bindport,dstip);
	sprintf(insert , "insert ws_regav set srcip=%d , avtype=%d , bindport=%d , dstip=%d;\n",srcip,type,bindport,dstip);

	printf("%s%s%s",select , updata , insert);

	if(0 > mysql_query(conn , select))
	{
		printf("updata error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	else
	{
		rows = mysql_affected_rows(conn);
		if(rows > 0)
		{
			printf("selete success line : %d , then do updata \n" , rows);
			if(0 > mysql_query(conn , updata))
			{
				printf("updata error %u: %s\n", mysql_errno(conn), mysql_error(conn));
				exit(1);
			}
			rows = mysql_affected_rows(conn);
			printf("updata success line : %d \n" , rows);
		}
		else
		{
			//if(0 > mysql_query(conn , insert))
			 mysql_query(conn , "use wsisgw\n");
			 mysql_query(conn , insert);
			//{
				printf("insert error %u: %s\n", mysql_errno(conn), mysql_error(conn));
				//exit(1);
			//}
			rows = mysql_affected_rows(conn);
			printf("insert success line : %d \n" , rows);
			printf("insert error %u: %s\n", mysql_errno(conn), mysql_error(conn));

		}
	}


	return 0;	
}

int main()
{
	initdb();

	insertdb(3,4,1,40034);
	//insertdb(1,2,1,40034);

	mysql_close(conn);
}
