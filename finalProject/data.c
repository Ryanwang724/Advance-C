#include "data.h"
#include "ui.h"

void file_st_pos_update_recursive(tDirectory *dirPtr, size_t up_range, size_t up_size)
{
    tFile *filePtr = dirPtr->fileHead;
    while(filePtr != NULL && filePtr->nextFile != NULL) // go to tail 從後面開始移
    {
        filePtr = filePtr->nextFile;
    }

    while (filePtr != NULL)
    {
        if (filePtr->start_position < up_range)
        {
            printf("%s update %zu to %zu\n", filePtr->fileName, filePtr->start_position, filePtr->start_position + up_size);
            filePtr->start_position += up_size;
        }
        filePtr = filePtr->prevFile;
    }

    // 遍歷子目錄
    tDirectory *childPtr = dirPtr->child;
    while (childPtr != NULL)
    {
        file_st_pos_update_recursive(childPtr, up_range, up_size);
        childPtr = childPtr->next;
    }
}

void file_st_pos_update(FileSystem *fs, size_t up_range, size_t up_size)
{
    file_st_pos_update_recursive(fs->root, up_range, up_size);
}

void initializeDirectory(tDirectory *tD)
{
    strcpy(tD->dirName,"/");
    tD -> child = NULL;
    tD -> next = NULL;
    tD -> prev = NULL;
    tD -> parent = NULL;
    tD -> fileHead = NULL;
}

FileSystem* initializeFileSystem(size_t partitionSize)
{
    FileSystem *fs;
    fs = (FileSystem *)malloc(sizeof(FileSystem));

    fs->root = (tDirectory *)malloc(sizeof(tDirectory));
    fs->partition = (char *)malloc(partitionSize);       // 宣告總共記憶體大小
    fs->used_part_loc=partitionSize;
    initializeDirectory(fs->root);
    fs->workDir = fs->root;
    fs->totalSize = partitionSize;
    fs->remainSize = partitionSize;
    strcpy(fs->currentDirectory,"/");

    return fs;
}

void initializeFile(tFile *tF)
{
    strcpy(tF->fileName,"");
    tF->content = NULL;
    tF->size = 0;
    tF->nextFile = NULL;
    tF->prevFile = NULL;
    tF->start_position = 0;
}

int checkName(const char *newDirName)
{
    // 檢查名字是否空的或過長
    if (strcmp(newDirName, "") == 0 || newDirName == NULL || strlen(newDirName) >= MAX_DIRECTORY_NAME)  
    {
        printf("Invalid directory name.\n");
        return 1;
    }
    else
        return 0;
}

void ls(FileSystem *fs)
{
    tDirectory *dirPtr = fs->workDir->child;
    tFile *filePtr = fs->workDir->fileHead;

    // 印出workDir內所有資料夾
    while (dirPtr != NULL)                             
    {
        printf("\033[34m%s\033[0m ", dirPtr->dirName); // 使用藍色顯示資料夾
        dirPtr = dirPtr->next;
    }
    // 印出workDir內所有檔案
    while(filePtr != NULL)                             
    {
        printf("%s ", filePtr -> fileName);
        filePtr = filePtr -> nextFile;
    }
    printf("\n");
}

void cd(FileSystem *fs, const char *path)
{
    // 檢查資料夾名稱是否合法
    if (checkName(path)==1) return;

    // cd .. 的case
    if(strcmp(path, "..") == 0) 
    {
        /*如果parent存在，代表可以執行cd ..*/
        if(fs -> workDir ->parent != NULL) 
        {
            removePath(fs,fs->workDir->dirName);  // 維護路徑名稱
            fs->workDir = fs->workDir->parent;    // 將workDir指向parent
            return;
        }
        else
        {
            printf("Already at the root directory.\n");
            return;
        }
    }
    // 遍歷所有workDir內的資料夾，尋找該資料夾
    tDirectory *dirPtr = fs->workDir->child;
    while (dirPtr != NULL)
    {
        if (strcmp(dirPtr->dirName, path) == 0)
        {
            fs->workDir = dirPtr;                  // 找到後將workDir指向它
            appendPath(fs,path);                   // 維護路徑名稱
            return;
        }
        dirPtr = dirPtr->next;
    }
    printf("Directory '%s' not found.\n", path);
}

void release_part(FileSystem *fs,tFile *target){
    if(fs->used_part_loc!=target->start_position){
        memcpy(fs->partition + (fs->used_part_loc +target->size) ,fs->partition + fs->used_part_loc, target->start_position-fs->used_part_loc);
        fs->used_part_loc+=target->size;
        file_st_pos_update(fs,target->start_position ,target->size);
    }
    else{
        fs->used_part_loc+=target->size;
    }          
}

void rm(FileSystem *fs, tDirectory *workDir, const char *delFileName)
{
    // 檢查資料夾名稱是否合法
    if (checkName(delFileName)==1) return;
    // 檢查資料夾內是否沒有檔案
    if (workDir->fileHead == NULL)
    {
        printf("current directory '%s' not exist file\n", workDir->dirName);
        return;
    }
    // 檢查檔名是否違規
    if(delFileName == NULL || strlen(delFileName) >= MAX_FILE_NAME)
    {
        printf("Invaild Filed Name");
        return;
    }

    tFile *filePtr = workDir->fileHead;
    int flag = 1;

    while(filePtr != NULL)
    {
        // 遍歷資料夾內所有檔案，尋找檔名相同的檔案
        if (strcmp(filePtr->fileName, delFileName) == 0)
        {
            flag = 0;
            break;
        }
        filePtr = filePtr->nextFile;
    }

    if(flag == 1)
    {
        printf("Delete file '%s' not found\n", delFileName);
        return;
    }

    if(workDir->fileHead == filePtr)   // 欲刪除的檔案為第一個時
    {
        if (filePtr->nextFile == NULL)    // 當前資料夾只有一個檔案
        {
            workDir->fileHead = NULL;
        }
        else
        {
            workDir->fileHead = filePtr->nextFile;
            filePtr->nextFile->prevFile = NULL;
        }
    }
    else if(filePtr->nextFile == NULL)    // 欲刪除的檔案為最後一個時
    {
        filePtr->prevFile->nextFile = NULL;
    }
    else                                 // 欲刪除的檔案非第一個與最後一個時
    {
        filePtr->prevFile->nextFile = filePtr->nextFile;
        filePtr->nextFile->prevFile = filePtr->prevFile;
    }
    release_part(fs,filePtr);
}

void rmdirect(FileSystem *fs, tDirectory *target_dir, const char *delPathName)
{
    // 檢查資料夾名稱是否合法
    if (checkName(delPathName)==1) return;
    
    tDirectory *target  = target_dir ->child;
    int flag = 0;
    // 該資料夾內沒有任何資料夾
    if (target_dir->child == NULL)
    {
        printf("current path '%s' not exist folder\n", target_dir->dirName);
        return;
    }
    else
    {
        while(1) 
        {
            // 遍歷資料夾內所有資料夾，尋找名稱相同的資料夾
            // 找到就跳出迴圈，找不到就將flag設1並跳出迴圈
            if(strcmp(target->dirName, delPathName) == 0)
            {
                break;
            }
            target = target->next;
            if (target == NULL)
            {
                printf("folder '%s' not found\n", delPathName);
                flag = 1;
                break;
            }
        }

        if (flag == 1) return;
        // 確認目標資料夾內是否有其他資料夾
        if (target->child == NULL) 
        {
            // 刪除目標資料夾內所有檔案
            while(target->fileHead != NULL)
            {
                rm(fs, target, target->fileHead->fileName);
            }
        }
        else // 目標資料夾內有其他資料夾
        {
            // 刪除目標資料夾內所有檔案
            while(target->fileHead != NULL)
            {
                rm(fs, target, target->fileHead->fileName);
            }
            // 刪除目標資料夾內其他資料夾，遞迴
            while(target->child != NULL)
            {
                rmdirect(fs, target, target->child->dirName);
            }
        }

        if(target_dir->child == target)           // 欲刪除的資料夾為第一個時
        {
            if (target->next == NULL)             // 且只有一個時
            {
                target_dir->child = NULL;
            }
            else
            {
                target_dir->child = target->next; // 欲刪除的資料夾為非第一個，將後面接好
                target->next->prev = NULL;
            }
        }
        else if(target->next == NULL)             // 欲刪除的資料夾為最後一個時
        {
            target->prev->next = NULL;
        }
        else                                      // 欲刪除的檔案非第一個與最後一個時
        {
            target->prev->next = target->next;
            target->next->prev = target->prev;
        }
    }
}

void mk(FileSystem *fs, const char *newDirName)
{
    // 檢查資料夾名稱是否合法
    if (checkName(newDirName)==1) return;

    tDirectory *child_dir = fs->workDir->child;    // 指向該資料夾內，所有資料夾的開頭
    //tDirectory *prev = NULL;

    // 確認當前路徑下有無同名資料夾
    while(child_dir != NULL)
    {
        if(strcmp(child_dir->dirName, newDirName) == 0)
        {
            printf("Directory '%s' already exists.\n", newDirName);
            return;
        }
        child_dir = child_dir->next;
    }

    tDirectory *new_dir = (tDirectory*)malloc(sizeof(tDirectory));
    initializeDirectory(new_dir);
    new_dir->parent = fs->workDir;
    strcpy(new_dir -> dirName, newDirName);

    // workDir內沒有任何資料夾
    if(fs->workDir->child == NULL)
    {
        fs->workDir->child = new_dir;
    }
    else // 插入至資料夾Linked list
    {
        tDirectory *dirPtr = fs->workDir->child;
        while(dirPtr != NULL && dirPtr->next != NULL) // 去到linked list的最後
        {
            dirPtr = dirPtr->next;
        }
        dirPtr->next = new_dir;
        new_dir->prev = dirPtr;
    }
    printf("Directory '%s' created. \n", new_dir);
}

void put(FileSystem *fs, const char *FILE_NAME)
{
    // 檢查名稱是否合法
    if (checkName(FILE_NAME)==1) return;

    tFile *filePtr = fs->workDir->fileHead;
    int flag = 0;
    // 遍歷workDir內的所有檔案，確認是否檔案已存在
    while(filePtr != NULL && filePtr->nextFile != NULL)
    {
        if(strcmp(filePtr->fileName ,FILE_NAME) == 0)
        {
            printf("File '%s' is aready in current path\n",FILE_NAME);
            flag = 1;
            break;
        }
        filePtr = filePtr ->nextFile;
    }
    if (flag == 1) return;

    int fileSize;
    FILE *fp;
    struct stat st;

    fp = fopen(FILE_NAME,"rb");
    if(fp == NULL)
    {
        printf("failed to open file '%s'\n",FILE_NAME);
        return;
    }

    stat(FILE_NAME,&st);
    fileSize = st.st_size; // 計算該檔案大小

    // 確認是否有足夠空間能夠放入檔案
    if(fileSize > fs->used_part_loc)
    {
        printf("Not enough space !\n");
        fclose(fp);
        return;
    }
    // 創立新資料 並給予size 
    tFile *new_file = (tFile*)malloc(sizeof(tFile));
    initializeFile(new_file);
    strcpy(new_file->fileName, FILE_NAME);
    new_file->size = fileSize;

    // 檔案資訊從記憶體最後方開始放
    new_file->start_position = fs->used_part_loc - fileSize;     // 計算放置的位置
    fs->used_part_loc = new_file->start_position;                // 更新fs中以使用的記憶體數量  

    printf("put %s to %d ~ %d\n",new_file->fileName,new_file->start_position,new_file->start_position + new_file->size - 1);
    fread(fs->partition + new_file->start_position, 1, fileSize, fp);

    // 選擇位置連接
    if (fs->workDir->fileHead == NULL)         //該資料夾尚未有檔案
    {
        fs->workDir->fileHead = new_file;
    }
    else                                       // 該資料夾有檔案了
    {
        tFile *filePtr = fs->workDir->fileHead;
        while(filePtr != NULL && filePtr->nextFile != NULL) // 去到linked list的最後
        {
            filePtr = filePtr->nextFile;
        }
        filePtr->nextFile = new_file;
        new_file->prevFile = filePtr;
    }
    fclose(fp);
}

void get(FileSystem *fs, const char *FILE_NAME)
{
    // 檢查名稱是否合法
    if (checkName(FILE_NAME)==1) return;
    FILE *fp;
    tFile *filePtr = fs -> workDir ->fileHead;

    // 遍歷workDir內的所有檔案，尋找目標檔案
    while (filePtr != NULL)
    {
        if(strcmp(filePtr ->fileName , FILE_NAME) == 0)
        {
            char path[MAX_DIRECTORY_NAME+MAX_FILE_NAME] = "./Dump/";
            #ifdef _WIN32
                CreateDirectory("Dump", NULL);
            #else
                if(opendir("Dump")== NULL)
                {
                    mkdir("Dump", 0777);
                }   
            #endif

            strcat(path, FILE_NAME); // 檔名合併至路徑
            fp = fopen(path, "wb");
            printf("get \"%s\", from %d\n",FILE_NAME,filePtr->start_position);
            fwrite(fs->partition + filePtr->start_position , 1, filePtr->size, fp);
            fclose(fp);
            return;
        }
        filePtr = filePtr->nextFile;
    }
    printf("\"%s\" doesn't exist! \n", FILE_NAME);
}

void cat(FileSystem *fs, const char *FILE_NAME)
{
    tFile *filePtr = fs->workDir->fileHead;

    // Find the file in the current directory
    while (filePtr != NULL)
    {
        if (strcmp(filePtr->fileName, FILE_NAME) == 0)
        {
            filePtr->content = (char *)malloc(filePtr->size);
            memcpy(filePtr->content,fs->partition +filePtr->start_position,  filePtr->size); 
            // Display the content of the file
            printf("File Content:\n%s\n", (void *)filePtr->content);
            return;
        }
        filePtr = filePtr->nextFile;
    }
    // If the file is not found
    printf("File not found: %s\n", FILE_NAME);
}

void status(FileSystem *fs)
{
    printf("partition size: %d\n",fs->totalSize);
    printf("free space:     %d\n",fs->used_part_loc);
}

void help()
{
    printf("--------------commands-------------\n");
    print_command_menu();
    printf("-----------------------------------\n");
}

// 遞歸函數遍歷檔案系統並創建字串
void save_directory_recursive(FILE *fp, tDirectory *dirPtr, const char *currentPath) {
    fprintf(fp, "folder:%s\n", currentPath);

    tFile *filePtr = dirPtr->fileHead;
    while (filePtr != NULL) {
        fprintf(fp, "file:%s%s size:%zu pointer:%zu\n", currentPath, filePtr->fileName, filePtr->size, filePtr->start_position);
        filePtr = filePtr->nextFile;
        fprintf(fp, "interval:\n");
    }

    tDirectory *childPtr = dirPtr->child;
    while (childPtr != NULL) {
        char nextPath[MAX_DIRECTORY_NAME + MAX_DIRECTORY_NAME];
        snprintf(nextPath, sizeof(nextPath), "%s%s/", currentPath, childPtr->dirName);
        fprintf(fp, "interval:\n");
        save_directory_recursive(fp, childPtr, nextPath);
        childPtr = childPtr->next;
    }
}

void save_file_system_state(const char *filename, FileSystem *fs) {
    FILE *fp = fopen(filename, "w");
    FILE *dump = fopen("dump.dump", "wb");
    if (fp != NULL) {
        // 遞歸函數遍歷檔案系統並創建字串
        save_directory_recursive(fp, fs->root, "/");

        // 保存 partition 內容
        fprintf(fp, "partitionsize:%zu\n", fs->totalSize);
        printf("Partition total size: %zu\n",fs->totalSize);
        fwrite(fs->partition, 1, fs->totalSize, dump);
        fclose(fp);
        fclose(dump);
        printf("saved: %s\n", filename);
    } else {
        printf("fail: %s\n", filename);
    }
}

tDirectory *createDirectory(const char *dirName) {
    tDirectory *newDir = (tDirectory *)malloc(sizeof(tDirectory));
    initializeDirectory(newDir);
    strcpy(newDir->dirName, dirName);
    return newDir;
}

void addChildDirectory(tDirectory *parent, tDirectory *child) {
    if(parent->child == NULL)
    {
        child->parent = parent;
        parent->child = child;
    }
    else
    {
        tDirectory *dirPtr = parent->child;
        while(dirPtr != NULL && dirPtr->next != NULL)
        {
            dirPtr = dirPtr->next;
        }
        dirPtr->next = child;
        child->prev = dirPtr;
    }
}

tDirectory *findChildDirectory(tDirectory *parent, const char *dirName) {
    tDirectory *current = parent->child;
    while (current != NULL) {
        if (strcmp(current->dirName, dirName) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

tDirectory *find_dir(FileSystem *fs, const char *path) {
    char *token = strtok((char *)path, "/");
    tDirectory *currentDir = fs->root;
    while (token != NULL) {
        // Check if the directory already exists
        tDirectory *childDir = findChildDirectory(currentDir, token);
        currentDir = childDir;
        token = strtok(NULL, "/");
    }
    return currentDir;
}

void mkSubdirectories(FileSystem *fs, const char *path) {
    char tempPath[256];
    strncpy(tempPath, path, sizeof(tempPath));

    char *token = strtok(tempPath, "/");
    tDirectory *currentDir = fs->root;

    while (token != NULL) {
        // Check if the directory already exists
        tDirectory *childDir = findChildDirectory(currentDir, token);
        
        if (childDir == NULL) {
            // If not, create a new directory
            tDirectory *newDir = createDirectory(token);
            addChildDirectory(currentDir, newDir);
            currentDir = newDir;
        } else {
            // If it exists, move to the next level
            currentDir = currentDir->child;
        }

        token = strtok(NULL, "/");
    }
}

void trim_newline(char *str) {
    size_t length = strlen(str);
    
    // 移除行末換行符號
    while (length > 0 && (str[length - 1] == '\n' || str[length - 1] == '\r')) {
        str[--length] = '\0';
    }
    
    // 移除行首換行符號
    size_t start = 0;
    while (str[start] == '\n' || str[start] == '\r') {
        ++start;
    }
    
    if (start > 0) {
        memmove(str, str + start, length - start + 1);  // 包括 null 結尾符號
    }
}

void addFileToDirectory(char *inputString, tDirectory *rootDirectory ,FileSystem *fs,int cnt) {
    char fileName[50];
    size_t fileSize, start_position;
    // 找到 "file:" 開始的位置
    // 使用 sscanf 進行資訊解析
    sscanf(inputString, "%s size:%d pointer:%d", fileName, &fileSize, &start_position);

    // 檔案相關資訊解析成功
    // 在目前目錄中新增檔案
    tFile *newFile = (tFile *)malloc(sizeof(tFile));
    initializeFile(newFile);
    strncpy(newFile->fileName, fileName, sizeof(newFile->fileName) - 1);
    newFile->size = fileSize;

    newFile->start_position = start_position;
    newFile->content = (char *)malloc(fileSize);
    memcpy(newFile->content,fs->partition +newFile->start_position,  fileSize);

    newFile->nextFile = rootDirectory->fileHead;
    if (newFile->nextFile != NULL) 
    {
        newFile->nextFile->prevFile = newFile;
    }
    rootDirectory->fileHead = newFile;

    newFile->start_position = fs->used_part_loc - newFile->size;     // 計算放置的位置
    fs->used_part_loc = newFile->start_position;    
    printf("free space: %d\n",fs->used_part_loc);            // 更新fs中以使用的記憶體數量  
    // 印出解析的資訊（可以移除）
    printf("File Name: %s\n", fileName);
    printf("File Size: %zu\n", fileSize);
    printf("Start Position: %zu\n", start_position);
    printf("\n");
}

void load_file_system_state(FILE *fp, FileSystem *fs) {
    FILE *filePtr = fopen("dump.dump", "rb");
    fread(fs->partition, 1, fs->totalSize, filePtr);
    fclose(filePtr);
    char line[256];
    tDirectory *dir_ptr;
    char buffer[1024];
    int cnt = 0;
    size_t partitionsize = 0;  // Initialize partition size

    rewind(fp);
    while (fgets(line, sizeof(line), fp) != NULL) 
    {
        trim_newline(line);

        if (strncmp(line, "folder:", 7) == 0) 
        {
            // Handle folder
            char *path = line + 7;
            printf("create%s\n", path);

            // Check if the directory already exists, if not, create it
            mkSubdirectories(fs, path);
        } 
        else if (strncmp(line, "file:", 5) == 0) 
        {
            // Handle file
            char *path = line + 5;
            trim_newline(path);

            // Save the directory part in path
            char *last_slash = strrchr(path, '/');
            if (last_slash != NULL) 
            {
                *last_slash = '\0';  // Remove file part
            }

            dir_ptr = find_dir(fs, path);

            // Initialize buffer with the current line (only file information)
            strcpy(buffer, last_slash + 1);  // Copy the file part

            // Read subsequent lines and append to the buffer until a new folder, file, or partition is encountered
            while (fgets(line, sizeof(line), fp) != NULL &&
                strncmp(line, "folder:", 7) != 0 &&
                strncmp(line, "file:", 5) != 0 &&
                strncmp(line, "interval:", 9) != 0 &&
                strncmp(line, "partitionsize:", 14) != 0) 
            {
                trim_newline(line);
                strcat(buffer, line);
            }
            // Print the buffer for debugging
            printf("%s\n", buffer);
            // Add the file to the directory
            cnt++;  //計算還原幾個檔案了
            addFileToDirectory(buffer, dir_ptr,fs,cnt);
        }
    }
}

size_t readRemainingPartitionSize(FILE *fp) {
    char line[256];
    size_t remainingSize = 0;

    // 找到 "partition:" 開始的位置
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "partition:", 10) == 0) {
            // 從 "partition:" 行的下一行開始讀取實際分區數據
            if (fgets(line, sizeof(line), fp) != NULL) {
                // 使用 atoi 或 sscanf 將分區數據轉換為整數
                remainingSize = atoi(line); // 或使用 sscanf(line, "%zu", &remainingSize);
            }
            break;
        }
    }
    return remainingSize;
}

void reverseLinkedListFile(tDirectory *targetDir) 
{
    tFile *current = targetDir->fileHead;
    tFile *temp = NULL;

    // 遍歷整個鏈表
    while (current != NULL ) {
        // 交換 next 和 prev 指針
        temp = current->prevFile;
        current->prevFile = current->nextFile;
        current->nextFile = temp;

        // 移動到下一個節點
        current = current->prevFile;
    }

    // 更新 head 指向新的頭部
    if (temp != NULL) {
        targetDir->fileHead = temp->prevFile;
    } else {
        targetDir->fileHead = current; // 如果 temp 為空，將 current 設為新的 fileHead
    }

    // 對子目錄遞迴調用
    tDirectory *currentChild = targetDir->child;
    while (currentChild != NULL) {
        if (currentChild->fileHead != NULL) {
            reverseLinkedListFile(currentChild);
        }
        currentChild = currentChild->next;
    }
}