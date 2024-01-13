#include "ui.h"
#include "data.h"

int main()
{
    int partitionSize;
    int choice;
    char inputBuf[100];
    FileSystem *myFilesystem;
    char *partition = NULL;
    char dump_name[50];
    char commnad_string[50];
    char name[50];
    char command[50];
    char line[256];
    FILE *filePtr;    
    FILE *filePtr1;
    size_t partitionsize;
    do
    {
        print_option_menu();
        scanf("%d",&choice);
        getchar();
        switch (choice) 
        {
            case 1:
                strcpy(dump_name, "file_system_state.txt");
                filePtr = fopen(dump_name, "r");
                if (filePtr == NULL) {
                    printf("Error opening file_system_state.txt\n");
                    return 1;
                }
                filePtr1 = fopen("dump.dump", "rb");
                fseek(filePtr1, 0, SEEK_END);
                rewind(filePtr1);
                fclose(filePtr1);
                
                filePtr1 = fopen(dump_name, "r");

                while (fgets(line, sizeof(line), filePtr1) != NULL) {
                    if (strncmp(line, "partitionsize:", 14) == 0) {
                        // Read the partition size information
                        sscanf(line + 14, "%zu", &partitionsize);
                        printf("Partition Size: %zu\n", partitionsize);
                    }
                }
                myFilesystem = initializeFileSystem(partitionsize);
                load_file_system_state(filePtr1, myFilesystem);
                choice = 3;
            case 3:
                print_command_menu();
                printf("\n");
                reverseLinkedListFile(myFilesystem->root);
                while(1)
                {
                    print_current_path(myFilesystem);
                    fgets(commnad_string, sizeof(commnad_string), stdin);
                    
                    //使用sscanf 進行分割
                    sscanf(commnad_string,"%s%s", command, name);
                    if(strcmp(command,"ls") == 0)
                    {
                        ls(myFilesystem);
                    }
                    else if(strcmp(command,"put") == 0)
                    {
                        put(myFilesystem, name);
                    }
                    else if(strcmp(command,"cat")==0)
                    {
                        cat(myFilesystem, name);
                    }
                    else if(strcmp(command,"mkdir") == 0)
                    {
                        mk(myFilesystem, name);
                    }
                    else if(strcmp(command,"cd") == 0)
                    {
                        cd(myFilesystem, name);
                    }
                    else if(strcmp(command,"get") == 0)
                    {
                        get(myFilesystem, name);
                    }
                    else if(strcmp(command,"help") == 0)
                    {
                        help();
                    }
                    else if (strcmp(command, "exit") == 0) {
                        // Save partition to a file before exiting
                        save_file_system_state("file_system_state.txt", myFilesystem);
                        break;
                    }
                    else if(strcmp(command,"status") == 0)
                    {
                        status(myFilesystem);
                    }
                    else if(strcmp(command,"rm") == 0)
                    {
                        rm(myFilesystem, myFilesystem->workDir, name);
                    }
                    else if(strcmp(command, "rmdir") == 0)
                    {
                        rmdirect(myFilesystem, myFilesystem->workDir, name);
                    }
                    else
                        printf("Invaild false!\n");
                }
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
            case 2:
                printf("\nInput size of a new partition (example 1024000): \n");
                scanf("%d",&partitionSize);
                printf("partition size = %d\n\n",partitionSize);
                getchar();
                partition = (char*)malloc(sizeof(char)*partitionSize);
                if(partition != NULL)
                {
                    
                    myFilesystem = initializeFileSystem(partitionSize);
                    printf("Make new partition successful!\n");
                }
                print_command_menu();
                printf("\n");
                
                while(1)
                {
                    print_current_path(myFilesystem);
                    char commnad_string[50]="", command[50]="", name[50]="";
                    fgets(commnad_string, sizeof(commnad_string), stdin);

                    //使用sscanf 進行分割
                    sscanf(commnad_string,"%s%s", command, name);
                    if(strcmp(command,"ls") == 0)
                    {
                        ls(myFilesystem);
                    }
                    else if(strcmp(command,"put") == 0)
                    {
                        put(myFilesystem, name);
                    }
                    else if(strcmp(command,"cat")==0)
                    {
                        cat(myFilesystem, name);
                    }
                    else if(strcmp(command,"mkdir") == 0)
                    {
                        mk(myFilesystem, name);
                    }
                    else if(strcmp(command,"cd") == 0)
                    {
                        cd(myFilesystem, name);
                    }
                    else if(strcmp(command,"get") == 0)
                    {
                        get(myFilesystem, name);
                    }
                    else if(strcmp(command,"help") == 0)
                    {
                        help();
                    }
                    else if (strcmp(command, "exit") == 0) {
                        // Save partition to a file before exiting
                        save_file_system_state("file_system_state.txt", myFilesystem);
                        break;
                    }
                    else if(strcmp(command,"status") == 0)
                    {
                        status(myFilesystem);
                    }
                    else if(strcmp(command,"rm") == 0)
                    {
                        rm(myFilesystem, myFilesystem->workDir, name);
                    }
                    else if(strcmp(command, "rmdir") == 0)
                    {
                        rmdirect(myFilesystem, myFilesystem->workDir, name);
                    }
                    else
                        printf("Invaild false!\n");
                }
                break;
        }
    }while(choice != 1 && choice != 2 && choice != 3);
    return 0;   
}