#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>  

char **tokenize(char *line);

char *mygets(char *buf, size_t size) {
    if (buf != NULL && size > 0) {
        if (fgets(buf, size, stdin)) {
            buf[strcspn(buf, "\n")] = '\0';
            return buf;
        }
        *buf = '\0';  /* clear buffer at end of file */
    }
    return NULL;
}

void checkcpupercentage(char** words)
{
	int rc1 = fork();
	if (rc1<0)
	{
		// fork failed 
		fprintf(stderr, "fork failed\n"); 
		exit(1);
	}
	else if (rc1==0)
	{
		// child (new process)
		char pid[1024], s1[1024], s2[1024], s3[1024];
		strcpy(pid,words[1]);
		strcpy(s1, "/proc/");
		strcpy(s2, "/stat");
		strcat(strcat(s1, pid), s2);
		int fd1 = open(s1, O_RDONLY);
		if (fd1 < 0) 
		{ 
			printf("Error while opening the file\n");
		}
		char buffer1[1024];
		int a = read(fd1, buffer1, 1024);
		if (a < 0)
		{
			printf("Error while reading the file\n");
		}
		close(fd1);
		char **values1 = tokenize(buffer1);
		int utime_before, stime_before, total_time_before = 0;
		utime_before = atoi(values1[13]);
		stime_before = atoi(values1[14]);
		strcpy(s3, "/proc");
		strcat(s3, s2);
		fd1 = open(s3, O_RDONLY);
		if (fd1 < 0) 
		{ 
			printf("Error while opening the file\n");
		}
		char buffer2[1024];
		int b = read(fd1, buffer2, 1024);
		if (b < 0)
		{
			printf("Error while reading the file\n");
		}
		close(fd1);
		char **values2 = tokenize(buffer2);
		for (int i=1; i<11;i++)
		{
			total_time_before += atoi(values2[i]);
		}
		sleep(1);
		fd1 = open(s1, O_RDONLY);
		if (fd1 < 0) 
		{ 
			printf("Error while opening the file\n");
		}
		char buffer3[1024];
		int c = read(fd1, buffer3, 1024);
		if (c < 0)
		{
			printf("Error while reading the file\n");
		}
		close(fd1);
		char **values3 = tokenize(buffer3);
		int utime_after, stime_after, total_time_after = 0;
		utime_after = atoi(values3[13]);
		stime_after = atoi(values3[14]);
		fd1 = open(s3, O_RDONLY);
		if (fd1 < 0) 
		{ 
			printf("Error while opening the file\n");
		}
		char buffer4[1024];
		int d = read(fd1, buffer4, 1024);
		if (d < 0)
		{
			printf("Error while reading the file\n");
		}
		close(fd1);
		char **values4 = tokenize(buffer4);
		for (int i=1; i<11;i++)
		{
			total_time_after += atoi(values4[i]);
		}
		int ut, st;
		ut = (100*(utime_after - utime_before))/(total_time_after - total_time_before);
		st = (100*(stime_after - stime_before))/(total_time_after - total_time_before);
		printf("User Mode CPU Percentage: %d%%\n", ut);
		printf("System Mode CPU Percentage: %d%%\n", st);
		exit(0);
	}
	else
	{
		wait(NULL);
	}
}

void checkresidentmemory(char** words)
{
	int rc2 = fork();
	if (rc2<0)
	{
		// fork failed 
		fprintf(stderr, "fork failed\n"); 
		exit(1);
	}
	else if (rc2==0)
	{
		char *args1[] = {"ps","-o", "rss", "-p", words[1], "--no-headers", NULL};
		execv("/bin/ps",args1);
	}
	else
	{
		wait(NULL);
	}
}

void listFiles()
{
	int rc3 = fork();
	if (rc3<0)
	{
		// fork failed 
		fprintf(stderr, "fork failed\n"); 
		exit(1);
	}
	else if (rc3==0)
	{
		// child (new process)
		int copy_stdoutfd = dup2(1, 10);
		close(1);
		int fd2 = open("files.txt", O_WRONLY | O_CREAT);
		if (fd2 < 0)
		{
			printf("Error while creating and opening the file\n");
		}
		char *args2[] = {"/bin/ls", NULL};
		execv(args2[0],args2);
		fflush(stdout);
		close(fd2);
		dup(copy_stdoutfd);
	}
	else
	{
		wait(NULL);
	}
}

void sortFile(char** words)
{
	int rc4 = fork();
	if (rc4<0)
	{
		// fork failed 
		fprintf(stderr, "fork failed\n"); 
		exit(1);
	}
	else if (rc4==0)
	{
		// child (new process)
		int copy_stdinfd = dup2(0, 10);
		close(0);
		int fd3 = open(words[1], O_WRONLY | O_CREAT);
		if (fd3 < 0)
		{
			printf("Error while creating and opening the file\n");
		}
		char *args3[] = {"/usr/bin/sort", words[1], NULL};
		execv(args3[0],args3);
		fflush(stdin);
		close(fd3);
		dup(copy_stdinfd);
	}
	else
	{
		wait(NULL);
	}
}

void commands(char** words, int size)
{
	if (strcmp(words[0],"exit")==0 && size == 1)
	{
	   	exit(0);
	}
	else if (strcmp(words[0],"checkcpupercentage")==0 && size == 2)
	{
		checkcpupercentage(words);
	}
	else if (strcmp(words[0],"checkresidentmemory")==0 && size == 2)
	{
		checkresidentmemory(words);
	}
	else if (strcmp(words[0],"listFiles")==0 && size == 1)
	{
		listFiles();
	}
	else if (strcmp(words[0],"sortFile")==0 && size == 2)
	{
		sortFile(words);
	}
	else 
	{
		printf("Illegal command or arguments\n");
	}
}

int main()
{
    char input[1024];
    while (1)
    {
        printf("myshell> ");
        memset(input, 0, sizeof(input));   
        mygets(input, sizeof input);
        char **words = tokenize(input);
        /** WRITE YOUR CODE HERE **/
        if (strcmp(input,"")==0)
        {
        	printf("Illegal command or arguments\n");	
        }
        else
        {
        	int size=0, command_no = 1, j = 0, size1 = 0, size2 = 0;
        	char *command1[1024], *command2[1024];
        	while(1)
        	{
        		if(words[size]!=NULL)
        		{
        			size++;
        		}
        		else
        		{
        			break;
        		}
        	}
        	for (int i=0;i<size;i++)
        	{
        		if (strcmp(words[i],";")==0)
        		{
        			j=0;
        			command_no = 2;
        		}
        		else if (command_no == 1)
        		{
        			command1[j] = words[i];
        			size1++;
        			j++;
        		}
        		else
        		{
        			command2[j] = words[i];
        			size2++;
        			j++;
        		}
        	}
        	if (command_no == 1)
        	{
        		commands(words, size);
        	}
        	else
        	{
        		if ((strcmp(command1[0],"exit")==0 && size1 == 1) || (strcmp(command2[0],"exit")==0 && size2 == 1))
				{
				   	exit(0);
				}
        		int rc5 = fork();
        		if (rc5 < 0)
				{
					// fork failed 
					fprintf(stderr, "fork failed\n"); 
					exit(1);
				}
				else if (rc5 == 0)
				{
					// child1 (new process)
					commands(command1, size1);
					exit(0);
				}
				else
				{
					// parent
					int rc6 = fork();
					if (rc6 < 0)
					{
						// fork failed 
						fprintf(stderr, "fork failed\n"); 
						exit(1);
					}
					else if (rc6 == 0)
					{
						// child2 (new process)
						commands(command2, size2);
						exit(0);
					}
					else
					{
						wait(NULL);
					}
					wait(NULL);
				}
        	}
        }
    }
    return 0;
}


char **tokenize(char *input)
{
    int count = 0;
    char **tokens = (char **) malloc(32 * sizeof(char *));
    char *sch;
    sch = strtok(input, " \t\n"); 

    while (sch != NULL)
    {
        tokens[count] = sch;
        count++;
        sch = strtok(NULL, " \t\n"); 
    }

    free(sch);
    tokens[count] = NULL ;
    return tokens;
}
