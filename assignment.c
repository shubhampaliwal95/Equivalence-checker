#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int bf=0;
int prevbf=0;
char buffer[10000];
char temp[1000];
int lno;

struct structure
{
	char name[50];

	char itemdatatype[30][20];
	int nitems;

	struct arrasparam *arrinstr[20];
	int arinstr;
};

struct arrays
{
	char name[20];
	int dim;
	int dimsz[7];
	char datyp[30];
	char defname[30];
};


struct arrasparam
{
	int pos;
	int d;
	int dsz[10];
};

struct functions
{
	char name[20];
	char returntype[20];
	char argtype[20][20];
	int nargs;
	char defname[30];
	struct arrasparam *arf[10];
	int af;
};

struct variable
{
	char name[30];
	char datatype[30];
	char defname[30];
	int lineno;
};


struct typo
{
	char assignment[30];
	char dtype[30];
};

struct structure *strct[30];
int stct;

struct functions *fun[30];
int fn=0;

struct typo *tdef[30];
int td=0;

struct variable *var[30];
int v=0;

struct arrays *arr[30];
int ar=0;


void analyze(char *tok[],int nt)
{

	if(strcmp(tok[0],"typedef")==0)
	{

		if(strcmp(tok[1],"struct")==0)
		{
			tdef[td]=(struct typo *)malloc(sizeof(struct typo));
    		strcpy(tdef[td]->assignment,tok[3]);
    		strcpy(tdef[td]->dtype,tok[1]);
    		strcat(tdef[td]->dtype," ");
    		strcat(tdef[td]->dtype,tok[2]);
    		td++;
		}
		else
		{
			tdef[td]=(struct typo *)malloc(sizeof(struct typo));
    		strcpy(tdef[td]->assignment,tok[2]);
    		strcpy(tdef[td]->dtype,tok[1]);
    		td++;
    	}
	}
	else//any fn. or arrays or normal var. start with any basic data type or struct type
	{

		if(strchr(tok[1],'(')!=NULL)//handling fns not having struct as return type
		{
			//for(int y=0;y<nt;y++)
				//printf("%s\n",tok[y]);
			fun[fn]=(struct functions*)malloc(sizeof(struct functions));
			strcpy(fun[fn]->defname,tok[0]);//name used for return type
			fun[fn]->af=0;

			if((strcmp(tok[0],"int")==0)||(strcmp(tok[0],"half")==0)||(strcmp(tok[0],"fixed")==0)||(strcmp(tok[0],"float")==0)||(strcmp(tok[0],"bool")==0))
				strcpy(fun[fn]->returntype,tok[0]);//return type of fn.
			else
			{
				int q;
    			for(q=0;q<td;q++)
    			{
    				if(strcmp(tok[0],tdef[q]->assignment)==0)
    				{
    					strcpy(fun[fn]->returntype,tdef[q]->dtype);
    					break;
    				}
    			}
			}

			fun[fn]->nargs=0;//initializing nargs in fn. to 0

			char comb[100]="";
			for(int cv=1;cv<nt;cv++)
			{
				strcat(comb,tok[cv]);
				strcat(comb," ");
			}

			//printf("%s\n",comb);
			char* t[2];////removing '('and getting name
			int p=0;
			t[p]=strtok(comb,"(");

			while(t[p]!=NULL)
			{
				//printf("%s\n",t[p]);
				p++;
    			t[p]=strtok(NULL,"(");
    		}

    		strcpy(fun[fn]->name,t[0]);//name of fn.

    		t[1]=strtok(t[1],")");//removing ')'

    		char* t2[20];
    		int z=0;
    		t2[z]=strtok(t[1],",");

			while(t2[z]!=NULL)
			{
				//printf("%s\n",t2[z]);
				z++;
    			t2[z]=strtok(NULL,",");
    		}



    		char *t3[20];
    		int h=0;
    		for(int cc=0;cc<z;cc++)
    		{
    			h=0;
    			for(int re=0;re<20;re++)
    				t3[re]=NULL;

    			t3[h]=strtok(t2[cc]," ");
				while(t3[h]!=NULL)
				{
					//printf("%s\n",t3[h]);
					h++;
    				t3[h]=strtok(NULL," ");
    			}


    			if(strcmp(t3[0],"struct")==0)
				{
					strcpy(fun[fn]->argtype[fun[fn]->nargs],t3[0]);
					strcat(fun[fn]->argtype[fun[fn]->nargs]," ");
					strcat(fun[fn]->argtype[fun[fn]->nargs],t3[1]);
					fun[fn]->nargs++;

					if(strchr(t3[2],'[')!=NULL)
					{
						fun[fn]->arf[fun[fn]->af]=(struct arrasparam*)malloc(sizeof(struct arrasparam));
						int gh=0;
						int brac=0;
						while(gh<strlen(t3[2]))
						{
							if(t3[2][gh]=='[')
								brac++;
							gh++;
						}

						fun[fn]->arf[fun[fn]->af]->d=brac;
						fun[fn]->arf[fun[fn]->af]->pos=cc+1;
						fun[fn]->af++;
					}
				}
				else if((strcmp(t3[0],"int")==0)||(strcmp(t3[0],"half")==0)||(strcmp(t3[0],"fixed")==0)||(strcmp(t3[0],"float")==0)||(strcmp(t3[0],"bool")==0))
				{
					strcpy(fun[fn]->argtype[fun[fn]->nargs],t3[0]);
					fun[fn]->nargs++;

					if(strchr(t3[1],'[')!=NULL)
					{
						fun[fn]->arf[fun[fn]->af]=(struct arrasparam*)malloc(sizeof(struct arrasparam));
						int gh=0;
						int brac=0;
						while(gh<strlen(t3[1]))
						{
							if(t3[1][gh]=='[')
								brac++;
							gh++;
						}

						fun[fn]->arf[fun[fn]->af]->d=brac;
						fun[fn]->arf[fun[fn]->af]->pos=cc+1;
						fun[fn]->af++;
					}
				}
				else
				{
					int q;
    				for(q=0;q<td;q++)
    				{
    					if(strcmp(t3[0],tdef[q]->assignment)==0)
    					{
    						strcpy(fun[fn]->argtype[fun[fn]->nargs],tdef[q]->dtype);
    						break;
    					}
    				}
					fun[fn]->nargs++;

					if(strchr(t3[1],'[')!=NULL)
					{
						fun[fn]->arf[fun[fn]->af]=(struct arrasparam*)malloc(sizeof(struct arrasparam));
						int gh=0;
						int brac=0;
						while(gh<strlen(t3[1]))
						{
							if(t3[1][gh]=='[')
								brac++;
							gh++;
						}

						fun[fn]->arf[fun[fn]->af]->d=brac;
						fun[fn]->arf[fun[fn]->af]->pos=cc+1;
						fun[fn]->af++;
					}

				}

    		}

			fn++;
		}
		else if(strcmp(tok[0],"struct")==0)//struct related declaration stuff(fns.,arrays, and other variables)
  		{
  			if(strchr(tok[2],'(')!=NULL)//fns. of return type struct
  			{
  				fun[fn]=(struct functions*)malloc(sizeof(struct functions));
  				strcpy(fun[fn]->returntype,tok[0]);
  				strcat(fun[fn]->returntype," ");
  				strcat(fun[fn]->returntype,tok[1]);
  				strcpy(fun[fn]->defname,fun[fn]->returntype);
  				fun[fn]->nargs=0;//initializing nargs in fn. to 0

				char comb[100]="";
				for(int cv=2;cv<nt;cv++)
				{
					strcat(comb,tok[cv]);
					strcat(comb," ");
				}

				//printf("%s\n",comb);
				char* t[5];////removing '('and getting name
				int p=0;
				t[p]=strtok(comb,"(");

				while(t[p]!=NULL)
				{
					//printf("%s\n",t[p]);
					p++;
    				t[p]=strtok(NULL,"(");
    			}

    			strcpy(fun[fn]->name,t[0]);//name of fn.

    			t[1]=strtok(t[1],")");//removing ')'

    			char* t2[20];
    			int z=0;
    			t2[z]=strtok(t[1],",");

				while(t2[z]!=NULL)
				{
					//printf("%s\n",t2[z]);
					z++;
    				t2[z]=strtok(NULL,",");
    			}



    			char *t3[20];
    			int h=0;
    			for(int cc=0;cc<z;cc++)
    			{
    				h=0;
    				for(int re=0;re<20;re++)
    					t3[re]=NULL;

	    			t3[h]=strtok(t2[cc]," ");
					while(t3[h]!=NULL)
					{
						//printf("%s\n",t3[h]);
						h++;
    					t3[h]=strtok(NULL," ");
    				}


    				if(strcmp(t3[0],"struct")==0)
					{
						strcpy(fun[fn]->argtype[fun[fn]->nargs],t3[0]);
						strcat(fun[fn]->argtype[fun[fn]->nargs]," ");
						strcat(fun[fn]->argtype[fun[fn]->nargs],t3[1]);
						fun[fn]->nargs++;


						if(strchr(t3[2],'[')!=NULL)
					{
						fun[fn]->arf[fun[fn]->af]=(struct arrasparam*)malloc(sizeof(struct arrasparam));
						int gh=0;
						int brac=0;
						while(gh<strlen(t3[2]))
						{
							if(t3[2][gh]=='[')
								brac++;
							gh++;
						}

						fun[fn]->arf[fun[fn]->af]->d=brac;
						fun[fn]->arf[fun[fn]->af]->pos=cc+1;
						fun[fn]->af++;
					}
					}
					else if((strcmp(t3[0],"int")==0)||(strcmp(t3[0],"half")==0)||(strcmp(t3[0],"fixed")==0)||(strcmp(t3[0],"float")==0)||(strcmp(t3[0],"bool")==0))
					{
						strcpy(fun[fn]->argtype[fun[fn]->nargs],t3[0]);
						fun[fn]->nargs++;

						if(strchr(t3[1],'[')!=NULL)
					{
						fun[fn]->arf[fun[fn]->af]=(struct arrasparam*)malloc(sizeof(struct arrasparam));
						int gh=0;
						int brac=0;
						while(gh<strlen(t3[1]))
						{
							if(t3[1][gh]=='[')
								brac++;
							gh++;
						}

						fun[fn]->arf[fun[fn]->af]->d=brac;
						fun[fn]->arf[fun[fn]->af]->pos=cc+1;
						fun[fn]->af++;
					}
					}
					else
					{
						int q;
    					for(q=0;q<td;q++)
    					{
    						if(strcmp(t3[0],tdef[q]->assignment)==0)
    						{
    							strcpy(fun[fn]->argtype[fun[fn]->nargs],tdef[q]->dtype);
    							break;
    						}
    					}
						fun[fn]->nargs++;

						if(strchr(t3[1],'[')!=NULL)
					{
						fun[fn]->arf[fun[fn]->af]=(struct arrasparam*)malloc(sizeof(struct arrasparam));
						int gh=0;
						int brac=0;
						while(gh<strlen(t3[1]))
						{
							if(t3[1][gh]=='[')
								brac++;
							gh++;
						}

						fun[fn]->arf[fun[fn]->af]->d=brac;
						fun[fn]->arf[fun[fn]->af]->pos=cc+1;
						fun[fn]->af++;
					}
					}

    			}
    			fn++;


  			}
  			else//handling simultaneous and individual declaration of (normal var. and array) of struct type
  			{
    			char* t[20];
				int p=0;
				t[p]=strtok(tok[2],",");
				while(t[p]!=NULL)
				{
					p++;
    				t[p]=strtok(NULL,",");
    			}
    			for(int i=0;i<p;i++)
    			{
    				//printf("%s\n",t[i]);

    				if(strchr(t[i],'[')!=NULL)//array handling of struct
    				{
    					//tokenize to find dim,dimsz and name

    					//printf("%s\n",t[i]);
    					arr[ar]=(struct arrays *)malloc(sizeof(struct arrays));
    					strcpy(arr[ar]->datyp,tok[0]);
    					strcat(arr[ar]->datyp," ");
    					strcat(arr[ar]->datyp,tok[1]);

    					strcpy(arr[ar]->defname,tok[0]);
    					strcat(arr[ar]->defname," ");
    					strcat(arr[ar]->defname,tok[1]);

    					char* jj[20];
						int kk=0;
						jj[kk]=strtok(t[i],"[");
						while(jj[kk]!=NULL)
						{
							kk++;
    						jj[kk]=strtok(NULL,"[");
    					}
    					strcpy(arr[ar]->name,jj[0]);
    					//printf("%d\n", kk);
    					arr[ar]->dim=kk-1;


    					int u=0;//
    					char num[10];//dim size in string
    					int numb;//dim size store
    					for(int hh=1;hh<kk;hh++)
    					{
    						//printf("%s\n", jj[hh]);
    						u=0;
    						while(jj[hh][u]!=']')
    						{
    							num[u]=jj[hh][u];
    							u++;
    						}
    						num[u]='\0';
    						//printf("%d\n", u);
    						numb=atoi(num);
    						arr[ar]->dimsz[(hh-1)]=numb;
    					}

    					ar++;
    				}
    				else
    				{
    					var[v]=(struct variable *)malloc(sizeof(struct variable));
    					strcpy(var[v]->datatype,tok[0]);
    					strcat(var[v]->datatype," ");
   						strcat(var[v]->datatype,tok[1]);
   						strcpy(var[v]->defname,tok[0]);
    					strcat(var[v]->defname," ");
   						strcat(var[v]->defname,tok[1]);
   						strcpy(var[v]->name,t[i]);
   						v++;
    				}
    			}
    		}

    	}

		else//handling simultaneous and individual declaration of normal var. and array with basic data type
		{

			//tokenizing based on ,
			char* t[20];
			int p=0;
			t[p]=strtok(tok[1],",");
			while(t[p]!=NULL)
			{
				p++;
    			t[p]=strtok(NULL,",");
    		}
    		// iterate through tokens
    		for(int i=0;i<p;i++)
    		{
    			//printf("%s\n",t[i]);
    			if(strchr(t[i],'[')!=NULL)//check for array
    			{
    				if((strcmp(tok[0],"int")==0)||(strcmp(tok[0],"half")==0)||(strcmp(tok[0],"fixed")==0)||(strcmp(tok[0],"float")==0)||(strcmp(tok[0],"bool")==0))
    				{
    					//tokenize to find dim,dimsz and name
    					//printf("%s\n",t[i]);
    					arr[ar]=(struct arrays *)malloc(sizeof(struct arrays));
    					strcpy(arr[ar]->datyp,tok[0]);
    					strcpy(arr[ar]->defname,tok[0]);
    				}
    				else
    				{
    					//printf("%s\n",tok[0]);
    					//printf("%s\n", t[i]);
    					int q;
    					for(q=0;q<td;q++)
    					{
    						if(strcmp(tok[0],tdef[q]->assignment)==0)
    						{
    							arr[ar]=(struct arrays *)malloc(sizeof(struct arrays));
    							strcpy(arr[ar]->datyp,tdef[q]->dtype);
    							strcpy(arr[ar]->defname,tok[0]);
    							break;
    						}
    					}
    				}
    				char* jj[20];
					int kk=0;
					jj[kk]=strtok(t[i],"[");
					while(jj[kk]!=NULL)
					{
						kk++;
    					jj[kk]=strtok(NULL,"[");
    				}
    				strcpy(arr[ar]->name,jj[0]);
    				//printf("%d\n", kk);
    				arr[ar]->dim=kk-1;


    				int u=0;//
    				char num[10];//dim size in string
    				int numb;//dim size store
    				for(int hh=1;hh<kk;hh++)
    				{
    					//printf("%s\n", jj[hh]);
    					u=0;
    					while(jj[hh][u]!=']')
    					{
    						num[u]=jj[hh][u];
    						u++;
    					}
    					num[u]='\0';
    					//printf("%d\n", u);
    					numb=atoi(num);
    					arr[ar]->dimsz[(hh-1)]=numb;
    				}
    				ar++;
    			}

    			else
    			{
    				if((strcmp(tok[0],"int")==0)||(strcmp(tok[0],"half")==0)||(strcmp(tok[0],"fixed")==0)||(strcmp(tok[0],"float")==0)||(strcmp(tok[0],"bool")==0))
    				{
    					var[v]=(struct variable *)malloc(sizeof(struct variable));
    					strcpy(var[v]->name,t[i]);
    					strcpy(var[v]->datatype,tok[0]);
    					strcpy(var[v]->defname,tok[0]);
    					var[v]->lineno=lno;
    					v++;
    				}
    				else
    				{
    					int q;
    					for(q=0;q<td;q++)
    					{
    						if(strcmp(tok[0],tdef[q]->assignment)==0)
    						{
    							var[v]=(struct variable *)malloc(sizeof(struct variable));
    							strcpy(var[v]->name,t[i]);
    							strcpy(var[v]->datatype,tdef[q]->dtype);
    							strcpy(var[v]->defname,tok[0]);
    							var[v]->lineno=lno;
    							v++;
    							break;
    						}
    					}
    				}
    			}
    		}

		}

	}
}


void struct_handler()
{
	//printf("%s\n",temp);

	int h2=0;
	char temp2[1000];
	for(int i=0;i<strlen(temp);i++)
	{
		if(temp[i]!='\n'&&temp[i]!='\t')//this process is to remove \n and \t because these cause issues later in tokenizing
		{

			temp2[h2++]=temp[i];
		}
		else if(temp[i]=='\n')
		{
			lno++;
		}
	}
	temp2[h2]='\0';
	//printf("%s\n",temp2);

	strct[stct]=(struct structure*)malloc(sizeof(struct structure));
	strct[stct]->nitems=0;
	strct[stct]->arinstr=0;
	char* t[3];
	int p=0;
	t[p]=strtok(temp2,"{");
	while(t[p]!=NULL)
	{
		//printf("%s\n",t[p]);
		p++;
    	t[p]=strtok(NULL,"{");
    }
	strcpy(strct[stct]->name,t[0]);
	//printf("%s\n",strct[stct]->name);
	char *temp3=strtok(t[1],"}");
	//printf("%s\n",temp3);
	char* t2[20];
    int z=0;
   	t2[z]=strtok(t[1],";");
	while(t2[z]!=NULL)
	{
		//printf("%s\n",t2[z]);
		z++;
		t2[z]=strtok(NULL,";");
   	}

   	char *t3[20];
    int h=0;
    for(int cc=0;cc<z;cc++)
   	{
    	h=0;
    	for(int re=0;re<20;re++)
 			t3[re]=NULL;

 	   t3[h]=strtok(t2[cc]," ");
		while(t3[h]!=NULL)
		{
			//printf("%s\n",t3[h]);
			h++;
	    	t3[h]=strtok(NULL," ");
	   	}


 	   if(strcmp(t3[0],"struct")==0)
		{
			strcpy(strct[stct]->itemdatatype[strct[stct]->nitems],t3[0]);
			strcat(strct[stct]->itemdatatype[strct[stct]->nitems]," ");
			strcat(strct[stct]->itemdatatype[strct[stct]->nitems],t3[1]);
			strct[stct]->nitems++;
			if(strchr(t3[2],'[')!=NULL)
			{
				strct[stct]->arrinstr[strct[stct]->arinstr]=(struct arrasparam*)malloc(sizeof(struct arrasparam));

				char* jj[20];
				int kk=0;
				jj[kk]=strtok(t3[2],"[");
				while(jj[kk]!=NULL)
				{
					kk++;
    				jj[kk]=strtok(NULL,"[");
    			}
    			//printf("%d\n", kk);
    			strct[stct]->arrinstr[strct[stct]->arinstr]->d=kk-1;


    			int u=0;//
    			char num[10];//dim size in string
    			int numb;//dim size store
    			for(int hh=1;hh<kk;hh++)
    			{
    				//printf("%s\n", jj[hh]);
    				u=0;
    				while(jj[hh][u]!=']')
    				{
    					num[u]=jj[hh][u];
    					u++;
    				}
    				num[u]='\0';
    				//printf("%d\n", u);
    				numb=atoi(num);
    				strct[stct]->arrinstr[strct[stct]->arinstr]->dsz[(hh-1)]=numb;
    			}
    			strct[stct]->arrinstr[strct[stct]->arinstr]->pos=cc+1;
    			strct[stct]->arinstr++;
			}
		}
		else if((strcmp(t3[0],"int")==0)||(strcmp(t3[0],"half")==0)||(strcmp(t3[0],"fixed")==0)||(strcmp(t3[0],"float")==0)||(strcmp(t3[0],"bool")==0))
		{
			strcpy(strct[stct]->itemdatatype[strct[stct]->nitems],t3[0]);
			strct[stct]->nitems++;

			if(strchr(t3[1],'[')!=NULL)
			{
				strct[stct]->arrinstr[strct[stct]->arinstr]=(struct arrasparam*)malloc(sizeof(struct arrasparam));

				char* jj[20];
				int kk=0;
				jj[kk]=strtok(t3[1],"[");
				while(jj[kk]!=NULL)
				{
					kk++;
    				jj[kk]=strtok(NULL,"[");
    			}
    			//printf("%d\n", kk);
    			strct[stct]->arrinstr[strct[stct]->arinstr]->d=kk-1;


    			int u=0;//
    			char num[10];//dim size in string
    			int numb;//dim size store
    			for(int hh=1;hh<kk;hh++)
    			{
    				//printf("%s\n", jj[hh]);
    				u=0;
    				while(jj[hh][u]!=']')
    				{
    					num[u]=jj[hh][u];
    					u++;
    				}
    				num[u]='\0';
    				//printf("%d\n", u);
    				numb=atoi(num);
    				strct[stct]->arrinstr[strct[stct]->arinstr]->dsz[(hh-1)]=numb;
    			}
    			strct[stct]->arrinstr[strct[stct]->arinstr]->pos=cc+1;
    			strct[stct]->arinstr++;
			}
		}
		else
		{
			int q;
	    	for(q=0;q<td;q++)
    		{
    			if(strcmp(t3[0],tdef[q]->assignment)==0)
   		 		{
   		 			strcpy(strct[stct]->itemdatatype[strct[stct]->nitems],tdef[q]->dtype);
   		 			break;
    			}
   			}
			strct[stct]->nitems++;

			if(strchr(t3[1],'[')!=NULL)
			{
				strct[stct]->arrinstr[strct[stct]->arinstr]=(struct arrasparam*)malloc(sizeof(struct arrasparam));

				char* jj[20];
				int kk=0;
				jj[kk]=strtok(t3[1],"[");
				while(jj[kk]!=NULL)
				{
					kk++;
    				jj[kk]=strtok(NULL,"[");
    			}
    			//printf("%d\n", kk);
    			strct[stct]->arrinstr[strct[stct]->arinstr]->d=kk-1;


    			int u=0;//
    			char num[10];//dim size in string
    			int numb;//dim size store
    			for(int hh=1;hh<kk;hh++)
    			{
    				//printf("%s\n", jj[hh]);
    				u=0;
    				while(jj[hh][u]!=']')
    				{
    					num[u]=jj[hh][u];
    					u++;
    				}
    				num[u]='\0';
    				//printf("%d\n", u);
    				numb=atoi(num);
    				strct[stct]->arrinstr[strct[stct]->arinstr]->dsz[(hh-1)]=numb;
    			}
    			strct[stct]->arrinstr[strct[stct]->arinstr]->pos=cc+1;
    			strct[stct]->arinstr++;
			}
		}
	}
	stct++;
}



void tokenizeline()
{
	int j=0;
	//printf("%s",temp);
	char* token[30];
	token[j]=strtok(temp," ");
	while(token[j]!=NULL)
	{
		j++;
    	token[j]=strtok(NULL," ");
    }
    analyze(token,j);
}


int extractline()
{

	char ch=buffer[bf];
	int h=0;
	if(ch=='\0')
		return -1;

	while(ch!=';')
 	{
 		if(ch!='{')
 		{
 			temp[h++]=ch;
 			bf++;
 			ch=buffer[bf];
 		}
 		else if(ch=='{')
 		{
 			bf=prevbf;
 			h=0;
 			break;
 		}
 	}

 	if(ch=='{')
 	{
 		ch=buffer[bf];
 		while(ch!='}')
 		{
 			temp[h++]=ch;
 			bf++;
 			ch=buffer[bf];
 		}
 		temp[h++]='}';
 		temp[h]='\0';
 		bf+=3;
 		prevbf=bf;

 		struct_handler();//take care of lno inside struct_handler()
 		lno++;
 	}
 	else if(ch==';')
 	{
 		temp[h]='\0';
 		//printf("%s\n",temp);
 		bf+=2;//for avoiding ; and \n and checking next for \0
 		lno++;
 		prevbf=bf;
 		tokenizeline();
 	}
 	return 0;
}


int nameEquiMatrix[30][30]={0};
int internalNameEquiMatrix[30][30]={0};

void internal_name_equivalence()
{
	for(int i=0;i<v;i++)
	{
		for(int j=i;j<v;j++)
		{
			if(strcmp(var[i]->defname, var[j]->defname)==0&&var[i]->lineno==var[j]->lineno)
				internalNameEquiMatrix[i][j]=1;
		}
	}

	int visited[v];
	int k=1;
	int c;

	for(int i=0;i<v;i++)
		visited[i]=0;

	for(int i=0;i<v;i++)
	{
		if(visited[i]==0)
		{
			printf("%d. %s", k, var[i]->name);
			visited[i]=1;
			c=0;

			for(int j=i+1;j<v;j++)
			{
				if(visited[j]==0 && internalNameEquiMatrix[i][j]==1)
				{
					printf(",%s", var[j]->name);
					visited[j]=1;
					c=1;
				}
			}
			if(c==1)
				printf(" are internal name equivalent.\n");
			else
				printf(" is not internal name equivalent to any other variable\n");
			k++;
		}
	}
	printf("\n");
}


void name_equivalence()
{
	for(int i=0;i<v;i++)
	{
		for(int j=i;j<v;j++)
		{
			if(strcmp(var[i]->defname, var[j]->defname)==0)
				nameEquiMatrix[i][j]=1;
		}
	}

	int visited[v];
	int k=1;
	int c;

	for(int i=0;i<v;i++)
		visited[i]=0;

	for(int i=0;i<v;i++)
	{
		if(visited[i]==0)
		{
			printf("%d. %s", k, var[i]->name);
			visited[i]=1;
			c=0;

			for(int j=i+1;j<v;j++)
			{
				if(visited[j]==0 && nameEquiMatrix[i][j]==1)
				{
					printf(",%s", var[j]->name);
					visited[j]=1;
					c=1;
				}
			}
			if(c==1)
				printf(" are name equivalent.\n");
			else
				printf(" is not name equivalent to any other variable\n");
			k++;
		}
	}
	printf("\n");
}


char structEquiMatrix[120][120];


char search_structEquiMatrix(char type1[20], char type2[20])
{
    int pos1=-1, pos2=-1;

    for(int x=0;x<120;x++)
    {
        if(x<v)
        {
            if(strcmp(var[x]->datatype, type1)==0)
                pos1=x;

            if(strcmp(var[x]->datatype, type2)==0)
                pos2=x;
        }

        else if(x<v+ar)
        {
            if(strcmp(arr[x-v]->datyp, type1)==0)
                pos1=x;

            if(strcmp(arr[x-v]->datyp, type2)==0)
                pos2=x;
        }

        else if(x>=v+ar+fn && x<v+ar+fn+stct)
        {
            if(strcmp(strct[x-v-ar-fn]->name, type1)==0)
                pos1=x;

            if(strcmp(strct[x-v-ar-fn]->name, type2)==0)
                pos2=x;
        }

    }

    if(structEquiMatrix[pos1][pos2]=='F')
                return 'F';

    return 'T';
}


char var_struct_equivalence(int i, int j)
{
    if(strcmp(var[i]->datatype, var[j]->datatype)==0)
        return 'T';

    return 'F';
}


char array_struct_equivalence(int i, int j)
{
    if(arr[i]->dim==arr[j]->dim)
    {
        char type1[20];
        char type2[20];
        strcpy(type1, arr[i]->datyp);
        strcpy(type2, arr[j]->datyp);
       
        char result = search_structEquiMatrix(type1, type2);

        if(result=='F')
            return 'F';

        int count=0;
        for(int k=0;k<arr[i]->dim;k++)
        {
            if(arr[i]->dimsz[k]==arr[j]->dimsz[k])
                count++;
        }

        if(count==arr[i]->dim)
            return 'T';
    }

    return 'F';
}


char function_struct_equivalence(int i, int j)
{
    if((fun[i]->nargs==fun[j]->nargs)&&(fun[i]->af==fun[j]->af))
    {
        char type1[20];
        char type2[20];
        strcpy(type1, fun[i]->returntype);
        strcpy(type2, fun[j]->returntype);
        char result = search_structEquiMatrix(type1, type2);

        if(result=='F')
            return 'F';

        for(int k=0;k<fun[i]->af;k++)	//checking arrays in argument
        {
            if(fun[i]->arf[k]->pos!=fun[j]->arf[k]->pos || (fun[i]->arf[k]->d!=fun[i]->arf[k]->d))
            {
                char type1[20]; 
                char type2[20]; 
                strcpy(type1, fun[i]->argtype[fun[i]->arf[k]->pos]);
                strcpy(type2, fun[j]->argtype[fun[j]->arf[k]->pos]);

                char result = search_structEquiMatrix(type1, type2);

                if(result=='F')
                    return 'F';
            }
            return 'F';
        }

        for(int k=0;k<fun[i]->nargs;k++)
        {
            char type1[20]; 
            char type2[20]; 
            strcpy(type1, fun[i]->argtype[k]);
            strcpy(type2, fun[j]->argtype[k]);

            char result = search_structEquiMatrix(type1, type2);

            if(result=='F')
                return 'F';
        }

        return 'T';
    }

    return 'F';
}



char structure_struct_equivalence(int i, int j)
{
    if(strct[i]->nitems==strct[j]->nitems && strct[i]->arrinstr==strct[j]->arrinstr)
    {
        for(int k=0;k<strct[i]->nitems;k++)
        {
            char type1[20];
            char type2[20];
            strcpy(type1, strct[i]->itemdatatype[k]);
            strcpy(type2, strct[j]->itemdatatype[k]);

            char result = search_structEquiMatrix(type1, type2);

            if(result=='F')
                return 'F';
        }

        return 'T';
    }

    return 'F';
}


void structural_equivalence_matrix(int t)
{
    int change;

    do
    {
        change=0;

        for(int i=0;i<t;i++)
        {
            for(int j=i+1;j<t;j++)
            {
                if(i<v)
                {
                    if(j<v)
                    {
                    	int x = i;
	                    int y = j;
	                   //printf("%d\n",x);
	                    if(x>=0 && y>=0 && structEquiMatrix[i][j] != var_struct_equivalence(x,y))
	                    {
	                        structEquiMatrix[i][j] = var_struct_equivalence(x,y);
	                        change=1;
	                    }
                    }

                    else if(j>=v)
                    {
	                    structEquiMatrix[i][j] = 'F';
	                    change=1;

	                    if(structEquiMatrix[i][j]=='F')
	                    	change=0;
                    }
                }

                else if(i>=v && i<v+ar)
                {
                    if(j<v)
                    {
                    	structEquiMatrix[i][j] = 'F';
	                    change=1;

	                    if(structEquiMatrix[i][j]=='F')
	                    	change=0;
                    }

                    else if(j>=v && j<v+ar)
                    {
                    	int x=i-v;
	                    int y=j-v;
	                    //printf("%d\n", x);
	                    if(x>=0 && y>=0 && structEquiMatrix[i][j] != array_struct_equivalence(x,y))
	                    {
	                        structEquiMatrix[i][j] = array_struct_equivalence(x,y);
	                        change=1;
	                    }
                    }

                    else if(j>=v+ar)
                    {
                    	structEquiMatrix[i][j] = 'F';
	                    change=1;

	                    if(structEquiMatrix[i][j]=='F')
	                    	change=0;
                    }
                }

                else if(i>=v+ar && i<v+ar+fn)
                {
                    if(j<v+ar)
                    {
                    	structEquiMatrix[i][j] = 'F';
	                    change=1;

	                    if(structEquiMatrix[i][j]=='F')
	                    	change=0;
	                }

                    else if(j>=v+ar && j<v+ar+fn)
                    {
                    	int x = i-v-ar;
	                    int y = j-v-ar;
	                    //printf("%d\n",x);
	                    if(x>=0 && y>=0 && structEquiMatrix[i][j] != function_struct_equivalence(x,y))
	                    {
	                        structEquiMatrix[i][j] = function_struct_equivalence(x,y);
	                        change=1;
	                    }
                    }

                    else if(j>=v+ar+fn)
                    {
                    	structEquiMatrix[i][j] = 'F';
	                    change=1;

	                    if(structEquiMatrix[i][j]=='F')
	                    	change=0;
                    }
                }

                else if(i>=v+ar+fn)
                {
                    if(j<v+ar+fn)
                    {
                    	structEquiMatrix[i][j] = 'F';
	                    change=1;

	                    if(structEquiMatrix[i][j]=='F')
	                    	change=0;
                    }

                    else if(j>=v+ar+fn)
                    {
                    	int x = i-v-ar-fn;
	                    int y = j-v-ar-fn;
	                    //printf("%d\n", x);
	                    if(x>=0 && y>=0 && structEquiMatrix[i][j] != structure_struct_equivalence(x,y))
	                    {
	                        structEquiMatrix[i][j] = structure_struct_equivalence(x,y);
	                        change=1;
	                    }
                    }
                }
            }
        }
    }while(change==1);
}

void print_structEquiMatrix(int t)
{
	for(int i=0;i<t;i++)
	{
		if(i<v)
			printf("%s ", var[i]->name);

		else if(i>=v && i<v+ar)
			printf("%s ", arr[i-v]->name);

		else if(i>=v+ar && i<v+ar+fn)
			printf("%s ", fun[i-v-ar]->name);
	}

	printf("\n");
	for(int i=0;i<t-stct;i++)
	{
		for(int j=0;j<t-stct;j++)
		{
			if(j<i)
				printf("  ");
			else
				printf("%c ", structEquiMatrix[i][j]);
		}
		printf("\n");
	}
}


void teller(char *s)
{
	int j=0;
	char* variable[2];
	variable[j]=strtok(s,",");
	while(variable[j]!=NULL)
	{
		j++;
    	variable[j]=strtok(NULL," ");
    }
	int i=0;
	int pos1,pos2;
	pos1=pos2=-1;
	for(i=0;i<v;i++)
	{
		if(strcmp(var[i]->name,variable[0])==0)
			pos1=i;
		else if(strcmp(var[i]->name,variable[1])==0)
			pos2=i;
	}
	
		for(i=0;i<ar;i++)
		{
			if(strcmp(arr[i]->name,variable[0])==0)
				pos1=v+i;
			else if(strcmp(arr[i]->name,variable[1])==0)
				pos2=v+i;
		}
	
	
		for(i=0;i<fn;i++)
		{
			if(strcmp(fun[i]->name,variable[0])==0)
				pos1=v+ar+i;
			else if(strcmp(fun[i]->name,variable[1])==0)
				pos2=v+ar+i;
		}
	
	
		for(i=0;i<stct;i++)
		{
			if(strcmp(strct[i]->name,variable[0])==0)
				pos1=v+ar+fn+i;
			else if(strcmp(strct[i]->name,variable[1])==0)
				pos2=v+ar+fn+i;
		}
	
	if(pos1==-1||pos2==-1)
		printf("\nVariable not present\n");
	else
	{
		if(structEquiMatrix[pos1][pos2]=='T')
			printf("\nTrue, they are equivalent\n");
		else
			printf("\nFalse, they are not equivalent\n");
	}
}


int main()
{

	char fname[100];
	printf("Enter the file name for test case\n");
	scanf("%s",fname);
    FILE *fptr=fopen(fname,"r");
    if(fptr==NULL)
    {
    	printf("No file exist with given name exist");
    	return 0;
    }

    int i=0;
    char ch=fgetc(fptr);
 	do
 	{
 		buffer[i++]=ch;
 		ch=fgetc(fptr);
 	}while(ch!=EOF);
 	//printf("%s\n",buffer);
 	fclose(fptr);

 	int x;
 	do
 	{
 		x=extractline();
 	}while(x!=-1);

 	for(int i=0;i<120;i++)
 	{
 		for(int j=0;j<120;j++)
 			structEquiMatrix[i][j]='T';
 	}

 	int total=v+ar+fn+stct;
	name_equivalence();
	internal_name_equivalence();
	structural_equivalence_matrix(total);

	
	
	print_structEquiMatrix(total);
	char test[40];
	while(1)
	{
		printf("\nEnter variables to check:\n");
		scanf("%s", test);
		teller(test);
	}
    return 0;
}
