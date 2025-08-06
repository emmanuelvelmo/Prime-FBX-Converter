#include<iostream>
#include<filesystem>
#include<fstream>
#include<string>
#include<vector>
#include<functional>
#include<optional>
#include<bitset>
#include<cmath>
#include<cstdlib>
#include<Windows.h>
#include"Prime FBX Converter.h"

int main()
{
    //SE CAPTURA DIRECTORIO DEL EJECUTABLE
    wchar_t w_dir[MAX_PATH];
    GetModuleFileName(NULL, w_dir, MAX_PATH);

    std::wstring pre_dir(w_dir);

    std::string dir_exe = std::string(pre_dir.begin(), pre_dir.end());
    dir_exe = dir_exe.substr(0, dir_exe.find_last_of('\\'));

    std::cout << "Place FBXs.txt in the same folder as the executable and press any key to continue\n";
    system("pause");
    std::cout << std::endl;

    //SE COMIENZA A PROCESAR TXT PARA CREAR DIRECTORIOS, BUSCAR Y COPIAR ARCHIVOS
    if (std::filesystem::exists(dir_exe + "/FBXs.txt"))
    {
        std::ifstream arch_txt(dir_exe + "/FBXs.txt");

        //SE ITERA CADA LÍNEA DEL ARCHIVO TXT
        while (std::getline(arch_txt, linea_tmp))
        {
            //SE CAPTURA DIRECTORIO PARA REALIZAR BÚSQUEDA
            if (linea_tmp.find("Search folder: ") != std::string::npos)
            {
                carp_busq = linea_tmp.substr(15);
                //SE CORROBORA QUE HAY UNA CARPETA PARA REALIZAR BÚSQUEDA
                c_bus = true;

                cont_lin++;
            }
            //SE CAPTURA DIRECTORIO PARA ALMACENAR MODELOS
            else if (linea_tmp.find("Models folder: ") != std::string::npos && c_bus)
            {
                carp_cmdl_mrea = linea_tmp.substr(15);
                //SE INDICA QUE SE HA TERMINADO DE GENERAR FBX
                c_nom = false;

                cont_lin++;
            }
            //SE CAPTURA DIRECTORIO PARA ALMACENAR TEXTURAS
            else if (linea_tmp.find("Textures folder: ") != std::string::npos && c_bus)
            {
                carp_txtr = linea_tmp.substr(17);
                c_nom = false;

                cont_lin++;
            }
            //SE CAPTURA NOMBRE DEL MODELO / SE CREAN DIRECTORIOS DE MODELO Y TEXTURAS CON EL MISMO NOMBRE
            else if (linea_tmp.find("Name: ") != std::string::npos && c_bus)
            {
                nomb_arch = linea_tmp.substr(6);

                if (!std::filesystem::is_directory(carp_cmdl_mrea + "/" + nomb_arch))
                {
                    std::filesystem::create_directories(carp_cmdl_mrea + "/" + nomb_arch);
                }
                if (!std::filesystem::is_directory(carp_txtr + "/" + nomb_arch))
                {
                    std::filesystem::create_directories(carp_txtr + "/" + nomb_arch);
                }

                //SE INDICA QUE DEBE GENERARSE FBX
                c_nom = true;

                cont_lin++;
            }
            //SI LA LÍNEA ESTÁ VACÍA NO SE HACE NADA
            else if (linea_tmp.empty())
            {
                //EVALUAR LA LÍNEA POR DEBAJO DE LA ACTUAL / GENERAR FBX SI SE HAN GUARDADO TODOS LOS ARCHIVOS EN CARPETA
                eval_lin_b(dir_exe, c_nom);

                cont_lin++;
            }
            //SE CAPTURA NOMBRE DEL ARCHIVO CON EXTENSIÓN / BUSCAR Y COPIAR ARCHIVOS HACIA DIRECTORIO DE MODELO
            else
            {
                bool arch_veri = false;

                //SE REALIZA BÚSQUEDA RECURSIVA EN CARPETA DE BÚSQUEDA
                for (std::filesystem::directory_entry iter_dir : std::filesystem::recursive_directory_iterator(carp_busq))
                {
                    //SI SE ENCUENTRA EL ARCHIVO EN LA CARPETA DE BÚSQUEDA
                    if (iter_dir.is_regular_file() && iter_dir.path().filename() == linea_tmp)
                    {
                        //SE COPIA DESDE CARPETA DE BÚSQUEDA A CARPETA DE MODELO
                        std::filesystem::copy(iter_dir, carp_cmdl_mrea + "/" + nomb_arch, std::filesystem::copy_options::overwrite_existing);

                        //SE INDICA EN CONSOLA EL ARCHIVO COPIADO
                        std::cout << linea_tmp << " OK" << std::endl;

                        arch_dir = iter_dir.path().string();
                        arch_veri = true;

                        break;
                    }
                }

                if (!arch_veri)
                {
                    //SE INDICA QUE EL ARCHIVO NO SE ENCONTRÓ
                    std::cout << linea_tmp << " not found" << std::endl;
                }

                eval_lin_b(dir_exe, c_nom);

                cont_lin++;
            }
        }

        arch_txt.close();
    }
    else
    {
        std::cout << "FBXs.txt was not found" << std::endl;
    }

    system("pause");
    return 0;
}