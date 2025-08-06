unsigned short cont_lin = 0;
unsigned short cont_lin_2 = 0;
bool c_bus = false;
bool c_nom = false;

std::string linea_tmp;
std::string linea_tmp_2;
std::string carp_busq;
std::string carp_cmdl_mrea;
std::string carp_txtr;
std::string nomb_arch;
std::string arch_dir;

void f_gen_fbx(std::string& arch_dir, std::string& carp_cmdl_mrea, std::string& carp_txtr, std::string& nomb_arch)
{
    //
}

void eval_lin_b(std::string& dir_exe, bool& c_nom)
{
    //SE ABRE EL ARCHIVO TXT NUEVAMENTE EN OTRA AVARIABLE PARA ITERAR CADA LÍNEA
    std::ifstream arch_txt_2(dir_exe + "/FBXs.txt");

    while (std::getline(arch_txt_2, linea_tmp_2))
    {
        //SI SE ALCANZA LA LÍNEA DEBAJO DE LA ACTUAL
        if ((cont_lin_2 == cont_lin + 1) && c_nom)
        {
            //SI LA LÍNEA POR DEBAJO DE LA ACTUAL INDICA EL COMIENZO DE OTRO MODELO
            if (linea_tmp_2.find("Name: ") != std::string::npos || linea_tmp_2.find("Models folder: ") != std::string::npos)
            {
                //SE LLAMA A LA FUNCIÓN PARA GENERAR FBX
                f_gen_fbx(arch_dir, carp_cmdl_mrea, carp_txtr, nomb_arch);

                std::cout << nomb_arch << ".FBX generated\n" << std::endl;

                break;
            }
        }

        cont_lin_2++;
    }

    cont_lin_2 = 0;
    arch_txt_2.close();
}