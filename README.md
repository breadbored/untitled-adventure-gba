# Butano Template for Window VS Code
>Template to kickstart the development of a Gameboy Advance game using the Butano engine in Visual Studio Code on Windows

- Check [Butano](https://github.com/GValiente/butano) for detailed information about the engine!

### Getting started
1. Please ensure you follow Butano's [Getting Started](https://gvaliente.github.io/butano/getting_started.html) guide before using this template
   - Install devkitARM to the default directory(`C:\\devkitPro`)
   - If you have installed devkitARM to a different directory, please update the following files: `Makefile` and `.vscode/c_cpp_properties.json` 
2. Create a new repository and clone it using this template repository
3. Clone the Butano engine by executing the following command from the root of the cloned repository
    ```
    git clone https://github.com/GValiente/butano.git engine
    ``` 

### Build test
1. Build the project by executing the following command from the root of the project
    ```
    make -j#cpu-cores# 
    ```
    For example, you can use `make -j8` or `make -j16` depending on the number of CPU cores you have
2. Run `butano-window-vscode-template.gba` from the root
   
   You should see the text `'BUTANO-WINDOW-VSCODE-TEMPLATE'` displayed on the screen 
4. (Optional) Cleanup
    If needed, you can delete the build and gba files by running the command
    ```
    make clean
    ```

