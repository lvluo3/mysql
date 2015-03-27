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

	if(0 !=  mysql_query(conn , "use wsisgw\n"))
	{
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	return 0;
}

int regist_av2db(unsigned int srcip , unsigned int dstip , unsigned int type , unsigned short bindport)
{
	MYSQL_RES * res = NULL;
	MYSQL_ROW row;
	//int num_fields = 0;
	int rows_changed = -1;
	int rows_select = -1;

	char updata[512] = {0};
	char insert[512] = {0};
	char select[512] = {0};

	sprintf(select , "select * from  ws_regav where dstip=%d and avtype = %d;\n",dstip , type);
	sprintf(updata , "update ws_regav set srcip=%d , bindport=%d where dstip=%d and avtype = %d;\n",srcip , bindport , dstip , type);
	sprintf(insert , "insert ws_regav set srcip=%d , avtype=%d , bindport=%d , dstip=%d\n",srcip,type,bindport,dstip);

	printf("%s%s%s",select , updata , insert);

	if(0 != mysql_query(conn , select))
	{
		printf("select error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}


	//res = mysql_use_result(conn);
	res = mysql_store_result(conn);
	//num_fields = mysql_num_fields(res);
	//row = mysql_fetch_row(res);//get one row from res , that produce by select.
	rows_select = mysql_num_rows(res);
	if(rows_select  == 1  )
	{
		printf(" > 0 select row : %d , then do updata \n" , rows_select);
		mysql_free_result(res);
		if(0 > mysql_query(conn , updata))
		{
			printf("updata error %u: %s\n", mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
		rows_changed = mysql_affected_rows(conn);// It returns the number of rows changed
		printf("updata success rows_changed : %d \n" , rows_changed);
	}
	else if(rows_select <= 0)
	{
		printf(" < 0 select row : %d , then do insert \n" , rows_select);
		mysql_free_result(res);
		if(0 != mysql_query(conn , insert))
		{
			printf("insert error %u: %s\n", mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
		rows_changed = mysql_affected_rows(conn);// It returns the number of rows changed
		printf("insert success rows_changed : %d \n" , rows_changed);

	}
	else
	{
		mysql_free_result(res);
		printf(" > 0 select row : %d , repeating data !!!!!!!!!!!!!!11 \n" , rows_select);
		
	}

	return 0;	
}

int main()
{
	initdb();

	regist_av2db(3,4,1,40034);
	//insertdb(1,2,1,40034);

	mysql_close(conn);
}
