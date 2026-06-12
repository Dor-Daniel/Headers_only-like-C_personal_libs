# C build system

## IMPORTANT 
  For now this header only support windows. (maybe later we add support for linux)

## Usage
  - Download the header file and place it in your working directory.
  - Create a '.c' file with the following code:
    ```code
      
      // #define COMPILER_PATH               L"\"C:\\Path\\To\\Your\\Compiler\\gcc.exe\""
      // #define LINKER_FLAGS                L"-llinkerflag1 -llinkerflag2 ..."
      // #define COMPILER_FLAGS              L"-Wwarning1 -Wwarning2 ..."
      // #define LIBS_FLAGS_PATH             L"-L\"C:\\Path\\To\\Your\\Library\\Folder\" ..."
      // #define LIBS_INCLUDE_PATH           L"-I\"C:\\Path\\To\\Your\\Includes\\Folder\" ..."
      // #define DEBUG_DEFINITIONS           L"-D_DEBUG -Ddefinition1 -Ddefinition2 ..."
      // #define RELEASE_DEFINITIONS         L"-Ddefinition1 -Ddefinition2 ..."
      // #define SOURCE_FILES_DIRECTORY      L"C:\\Path\\To\\Your\\Source\\Directory"
      // #define BUILD_DIRECTORY             L"C:\\Path\\To\\Your\\Build\\Directory" 
      // 
      // NOTE: You dont need to create the build folder we create this at runtime if it does not exist. 
      
      #define BUILD_IMPLEMENTATION
      #include "build.h"
      
      int main(int argc, char ** argv)
      {
          build(argc, argv);
          return 0;
      }
    ```
## Features
  - We do not allocate nothing in runtime (For performance) so you might want to change the amount of memory chuncks or their sizes.
  - The reaseon for working with wchar_t is to support naming with other langueges... but i didnt tested it.
