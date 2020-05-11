1. unzip to a folder "adapter" (this folder should include "build", "inc" and "src" folders).
2. copy "adapter" folder to SDK source code folder "applib"
3. cd to adapter/build folder and build the project:
   make CODE_ARCH=PMC_DEVICE TARGET_PLATFORM=PMC_CUSTOMER_OS
   
   To ¡°clean¡± for a rebuild run the following make command:
   make CODE_ARCH=PMC_DEVICE TARGET_PLATFORM=PMC_CUSTOMER_OS clean

    
