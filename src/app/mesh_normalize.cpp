#include "mesh_utils.h"


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout 
			<< "Usage: app <mesh_input_file> <mesh_input_file>\n"
			<< "Usage: app ../data/face.obj ../data/face_norm.obj \n";
		return EXIT_FAILURE;
	}
	
	const std::string filename_in = argv[1];
	const std::string filename_out = (argc > 2) ? argv[2] : filename_append_before_extension(filename_in, "norm");

	Mesh_t mesh;
	if (!load_mesh(mesh, filename_in))
	{
		std::cout << "Could not read " << filename_in << std::endl;
		return EXIT_FAILURE;
	}

    normalize_mesh(mesh);

	if (!save_mesh(mesh, filename_out))
	{
		std::cout << "Could not save " << filename_out << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}