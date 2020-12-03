
#include"cjs.h"

/***
<string.h>中strtok_r函数
***/
char *strtok_rr(char *s,const char *delim,char **save_ptr)
{
	char *token;
	if(s==NULL)s=*save_ptr;
	s+=strspn(s,delim);
	if(*s=='\0')
		return NULL;
	token=s;
	s=strpbrk(token,delim);
	if(s==NULL)

		*save_ptr=strchr(token,'\0');
	else{
		*s='\0';
		*save_ptr=s+1;
	}
	return token;
}
/***
分号和逗号顺序分割
***/
char *strtok_ec(char *s,  char **save_ptr)
{
	if(s==NULL)s=*save_ptr;
	int add_len=0;
	int value_len=0;
	char *add_pos=NULL;
	char *value_pos=NULL;
	add_pos=strstr(s,";");
	value_pos=strstr(s,",");
	if (add_pos != NULL) add_len=strlen(add_pos);
	if (value_pos != NULL) value_len=strlen(value_pos);
	char *token;

	if (add_len > value_len)
	{
		token=strtok_rr(s,";",save_ptr);
	}

	else
	{
		token=strtok_rr(s,",",save_ptr);
	}
	return token;

}
/***
char指针指向的值加1，再返回char
***/
void value_add(char *s)
{
	char *ptr=NULL;
	int add_ret=0;
	char add_num[20];
	add_ret = strtol(s, &ptr, 16);
	add_ret ++;
	sprintf(add_num,"%x",add_ret);
	strcpy(s,add_num);

}
/***
去除字符串中空字符
***/
char *strip(char *fu_in_strs,char *fu_out_strs)
{
	char *r = fu_out_strs;
	while(*fu_in_strs != '\0')
	{
		if(*fu_in_strs ==' ' || *fu_in_strs == '\t' || *fu_in_strs == '\r' || *fu_in_strs == '\n')
		{
			fu_in_strs++;
		}
		else
		{
			*r++ = *fu_in_strs++;
		}
	}
	*r='\0';
	return fu_out_strs;
}

/***
字符串是不是都是16进制字符判断
***/
int check_hex(char *test_chars)
{
	while(*test_chars != '\0')
	{
		if (!(isxdigit(*test_chars)))
		{
			return 0;
		}
		test_chars++;
	}
	return 1;

}
/***
检测add和value是不是符合要求，add位数大于2，value位数小于3，且2者都要是16进制
***/
bool check_add_value(char *add, char *value)
{
	if ( (strlen(add) >= 2) && (check_hex(add)) && (strlen(value) < 3) && (check_hex(value)) )
	{
		return true;
	}
	else
	{
		return false;
	}

}
/***
连接字符串
***/
char* join3(char *s1, char *s2)
{
    char *result = (char*)malloc(strlen(s1)+strlen(s2)+1);  //+1 for the zero-terminator
    if (result == NULL) return result;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* str_join(int str_nums, ...)
{
    int i = 0;
    int str_total = 0;
    va_list args;
    va_start(args, str_nums);
    for (i=0;i<str_nums;i++){
        str_total += strlen(va_arg(args, char*));
    }
    va_start(args, str_nums);
    char *result = (char*)malloc(str_total+1);  //+1 for the zero-terminator
    for (i=0;i<str_nums;i++){
        if (i==0) strcpy(result, va_arg(args, char*));
        else strcat(result, va_arg(args, char*));
    }
    va_end(args);
    return result;
}

char* str_modifi(char *s1, const char *s2)
{
    int str_total = strlen(s1);
    char* str_temp = NULL;
    str_temp = strrchr(s1, '.');
    if (str_temp){
        int str_left_len = str_total-strlen(str_temp)+1; //包含点.
        char *result = (char*)malloc(str_left_len+strlen(s2)+1);  //+1 for the zero-terminator
        strncpy(result, s1, str_left_len);
        result[str_left_len]='\0';
        strcat(result, s2);
        return result;
    }
    else {
        char *result = (char*)malloc(str_total+strlen(s2)+1);  //+1 for the zero-terminator
        strcpy(result, s1);
        strcat(result, s2);
        return result;
    }
}

char* gnu_basename(char *pathname)
{
	char *base;
	if ((base = strrchr (pathname, '/')) != NULL || (base = strrchr (pathname, '\\')) != NULL)
	{
		return (base + 1);
	}
	else
	{
		return (pathname);
	}
}

/***
处理特殊格式的文件,返回值为匹配到的数量
***/
int ec_file(char  *f_name, EC_CLASS_CHEATS *ff_class_cheats)
{

	//处理文件
	FILE *fp=fopen(f_name,"r");
	if(fp==NULL)
	{
		return -2;
	}
	const char *delfault_OFF="OFF";
	char linebuf[1024];
	char line_strip[1024];
	char ec_name[1024];
	int key_nums=0;
	int ec_nums=0;
	int cheats_nums=-1;
	//int ec_end = 0;
	while(fgets(linebuf,1024,fp)>0)
	{

		strip(linebuf,line_strip);
		char line_first = line_strip[0];
		char line_end = line_strip[strlen(line_strip)-1];
		//大类名称,第一个名称默认OFF，第一个add和value默认0
		if(line_first == '[' && line_end == ']')
		{
			key_nums=0;
			ec_nums=0;
			cheats_nums++;
			if (cheats_nums > MAX_cheats -1)
			{
				cheats_nums--;
				break;
			}
			strncpy(ec_name,&line_strip[1],strlen(line_strip)-1);
			ec_name[strlen(line_strip)-2]='\0';
			ff_class_cheats[cheats_nums].ec_class_name=(char*)malloc(strlen(ec_name)+1);//初始化
			strcpy(ff_class_cheats[cheats_nums].ec_class_name,ec_name);
			ff_class_cheats[cheats_nums].ec_default=0;
			ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_name=(char*)malloc(strlen(delfault_OFF)+1);
			strcpy(ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_name,delfault_OFF);
			ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_add[key_nums]=0;
			ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_value[key_nums]=0;
		}
		//处理大类的内容
		else if((strstr(line_strip,"=")!=NULL) && ((strstr(line_strip,",")!=NULL) ))
		{
			/*获取key_name*/
			char *outer_ptr=NULL;
			char *temp_key_name=NULL;
			key_nums=0;
			ec_nums ++;
			temp_key_name=strtok_rr(line_strip,"=",&outer_ptr);
			ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_name=(char*)malloc(strlen(temp_key_name)+1);
			strcpy(ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_name,temp_key_name);
			/*获取key_add和key_value*/
			char add_char[10];
			char value_char[10];
			char *temp_str=NULL;
			char *outer_ptr2=NULL;

			int next_add = 0;   //下一个值是不是add
			int value_nums = 0; //逗号的第几次分隔
			//首次逗号分隔，为add无需判断,且下个值为value
			temp_str=strtok_rr(outer_ptr,",",&outer_ptr2);
			strcpy(add_char,temp_str);
			outer_ptr=NULL;

			//根据下个字符是分号还是逗号，判断是add还是value，分号(;)后为地址，逗号(,)默认为value
			int add_len=0;
			int value_len=0;
			char *add_pos=NULL;
			char *value_pos=NULL;
			//谁长度长，谁在前,第一个add已经读取，开始读第一个value
			while(strlen(outer_ptr2)>0)
			{
				add_pos=strstr(outer_ptr2,";");
				value_pos=strstr(outer_ptr2,",");
				add_len=0;
				value_len=0;
				if (add_pos != NULL) add_len=strlen(add_pos);
				if (value_pos != NULL) value_len=strlen(value_pos);

				if (add_len > value_len)
				{
					temp_str=strtok_rr(NULL,";",&outer_ptr2);
					next_add = 1;
					value_nums++;
				}
				else
				{
					temp_str=strtok_rr(NULL,",",&outer_ptr2);
					value_nums++;
					if(next_add ==1)
					{
						value_nums = 0;
						next_add = 0;
					}
				}
				/***
				开始判断分隔出的字符
				***/
				//如果是分号分出来的，是add
				if (value_nums==0)
				{
					strcpy(add_char,temp_str);
				}
				//如果是逗号分出来的，是value
				else if(value_nums > 0)
				{
					//第一次逗号分出
					if(value_nums==1)
					{
						strcpy(value_char,temp_str);
					}
					//非第一次逗号分出，连续分出
					else if(value_nums > 1)
					{
						value_add(add_char);
						strcpy(value_char,temp_str);
					}
					//检验add和value符合性
					if(check_add_value(add_char, value_char))
					{
						sscanf(add_char,"%06x",&ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_add[key_nums]);
						sscanf(value_char,"%02x",&ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_value[key_nums]);
						key_nums ++;
						ff_class_cheats[cheats_nums].ec_cheats[ec_nums].key_nums = key_nums;
						if(key_nums ==cheat_MAX_keys) break;
					}
				}
			}
			//没有合格的add和value，中断寻找和中断文件处理
			if( key_nums ==0)
			{
				cheats_nums--;
				break;
			}
		}
		//没有符合格式的文件行，中断文件处理
		else if(strlen(line_strip) > 0)
		{
			cheats_nums--;
			break;
		}
		ff_class_cheats[cheats_nums].ec_max=ec_nums;
	}
	fclose (fp);
	return cheats_nums;

}
