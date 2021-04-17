#pragma once

#include <iostream>
#include <vector>
#include <fstream>




template<typename T>
static void vector_write(std::ofstream& out_file, const std::vector<T>& data)
{
	const std::size_t count = data.size();
	out_file.write(reinterpret_cast<const char*>(&data[0]), data.size() * sizeof(T));
}

template<typename T>
static void vector_write(const std::string& out_filename, const std::vector<T>& data)
{
	std::ofstream out_file;
	out_file.open(out_filename, std::ios::out | std::ios::binary);
	vector_write(out_file, data);
	out_file.close();
}

template<typename T>
static void vector_write(const std::string& out_filename, const T* data, std::size_t count)
{
	std::ofstream out_file;
	out_file.open(out_filename, std::ios::out | std::ios::binary);
	out_file.write(reinterpret_cast<const char*>(data), count * sizeof(T));
	out_file.close();
}



template<typename T>
static auto vector_read(std::ifstream& file, std::shared_ptr<T>& data)
{
	auto fsize = file.tellg();
	file.seekg(0, std::ios::beg);
	const auto count = fsize / sizeof(T);
	data->reset(count);
	file.read(reinterpret_cast<char*>(data->get()), fsize);
	return count;
}

template<typename T>
static auto vector_read(std::ifstream& file, std::vector<T>& data)
{
	auto fsize = file.tellg();
	file.seekg(0, std::ios::beg);
	const auto count = fsize / sizeof(T);
	data.resize(count);
	file.read(reinterpret_cast<char*>(&data[0]), fsize);
	return count;
}

template<typename T>
static auto vector_read(const std::string& in_filename, std::vector<T>& data)
{
	std::ifstream file(in_filename, std::ios::in | std::ios::binary | std::ios::ate);
	return vector_read(file, data);
}


template<typename T>
static auto read_data_from_file(const std::string& in_filename, T& data)
{
	std::ifstream file(in_filename, std::ios::in | std::ios::binary | std::ios::ate);
	auto fsize = file.tellg();
	file.seekg(0, std::ios::beg);
	const auto count = fsize / sizeof(T);
	file.read(reinterpret_cast<char*>(&data), fsize);
	file.close();
	return count;
}


#if 0
int main(int argc, char **argv)
{
	std::vector<double> v1 = { 1.1, 2.2, 3.3 };
	std::vector<double> v2;

	std::ofstream out_file;
	out_file.open("test_io_double.bin", std::ios::out | std::ios::binary);
	vector_write(out_file, v1);
	out_file.close();

	std::ifstream in_file;
	in_file.open("test_io_double.bin", std::ios::in | std::ios::binary);
	vector_read(in_file, v2);
	in_file.close();

	std::cout << "Saved Data  : ";
	for (const auto v : v1)
		std::cout << v << ' ';
	std::cout << std::endl;

	std::cout << "Loaded Data : ";
	for (const auto v : v2)
		std::cout << v << ' ';
	std::cout << std::endl;

	return EXIT_SUCCESS;
}
#endif