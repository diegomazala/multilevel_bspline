#pragma once

#include <vector>
#include <string>
#include "yaml.hpp"

class FaceBlendingProject
{

private:

    Yaml::Node root;
    
public:

	struct Thumbnail 
	{ 
		Thumbnail(const std::string& _filename, float _x, float _y) :
			filename(_filename), x(_x), y(_y) {	}
		Thumbnail(){}
		std::string filename;
		float x, y; 
	};

    bool load(const char* filepath)
	{
		try
		{
			Parse(root, filepath);
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
			return false;
		}
		return true;
	}

	std::vector<std::string> getTexturesFilename()
	{
		std::vector<std::string> filenames;
		try
		{
			Yaml::Node& meshes = root["textures"];
			if (meshes.IsSequence() == false)
			{
				throw Yaml::InternalException("textures is not of type sequence.");
			}

			for (auto itS = meshes.Begin(); itS != meshes.End(); itS++)
			{
				Yaml::Node& entry = (*itS).second;
				for (auto itSS = entry.Begin(); itSS != entry.End(); itSS++)
				{
					filenames.push_back((*itSS).second.As<std::string>());
				}
			}
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return filenames;
	}

	std::vector<Thumbnail> getThumbnails()
	{
		std::vector<Thumbnail> thumbnails;
		try
		{
			Yaml::Node& thumbs = root["thumbnails"];
			if (thumbs.IsSequence() == false)
			{
				throw Yaml::InternalException("thumbnails is not of type sequence.");
			}

			for (auto itS = thumbs.Begin(); itS != thumbs.End(); itS++)
			{
				Yaml::Node& entry = (*itS).second;
				thumbnails.push_back({ entry["thumbnail"].As<std::string>(), entry["pos_x"].As<float>(0), entry["pos_y"].As<float>(0) });
			}
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return thumbnails;
	}

	std::vector<float> getThumbnailsPos()
	{
		std::vector<float> thumbnailsPos;
		try
		{
			Yaml::Node& thumbs = root["thumbnails"];
			if (thumbs.IsSequence() == false)
			{
				throw Yaml::InternalException("thumbnails is not of type sequence.");
			}

			for (auto itS = thumbs.Begin(); itS != thumbs.End(); itS++)
			{
				Yaml::Node& entry = (*itS).second;
				thumbnailsPos.push_back(entry["pos_x"].As<float>(0));
				thumbnailsPos.push_back(entry["pos_y"].As<float>(0));
			}
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return thumbnailsPos;
	}

	std::vector<std::string> getOriginalMeshesFilename()
	{
		std::vector<std::string> filenames;
		try
		{
			Yaml::Node& meshes = root["original_meshes"];
			if (meshes.IsSequence() == false)
			{
				throw Yaml::InternalException("original_meshes is not of type sequence.");
			}

			for (auto itS = meshes.Begin(); itS != meshes.End(); itS++)
			{
				Yaml::Node& entry = (*itS).second;
				for (auto itSS = entry.Begin(); itSS != entry.End(); itSS++)
				{
					filenames.push_back((*itSS).second.As<std::string>());
				}
			}
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return filenames;
	}

	std::vector<std::string> getSmoothedMeshesFilename()
	{
		std::vector<std::string> filenames;
		try
		{
			Yaml::Node& meshes = root["smoothed_meshes"];
			if (meshes.IsSequence() == false)
			{
				throw Yaml::InternalException("smoothed_meshes is not of type sequence.");
			}

			for (auto itS = meshes.Begin(); itS != meshes.End(); itS++)
			{
				Yaml::Node& entry = (*itS).second;
				for (auto itSS = entry.Begin(); itSS != entry.End(); itSS++)
				{
					filenames.push_back((*itSS).second.As<std::string>());
				}
			}
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return filenames;
	}

	std::string getNormalMeshFilename()
	{
		try
		{
			Yaml::Node& parameters = root["parameters"];

			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}
			return parameters["normal_mesh"].As<std::string>();
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Example exception " << e.Type() << ": " << e.what() << std::endl;
			return std::string();
		}
	}

	std::string getUVMeshFilename()
	{
		try
		{
			Yaml::Node& parameters = root["parameters"];

			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}
			return parameters["uv_mesh"].As<std::string>();
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Example exception " << e.Type() << ": " << e.what() << std::endl;
			return std::string();
		}
	}

	std::string getRGBMeshFilename()
	{
		try
		{
			Yaml::Node& parameters = root["parameters"];

			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}
			return parameters["rgb_mesh"].As<std::string>();
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Example exception " << e.Type() << ": " << e.what() << std::endl;
			return std::string();
		}
	}

	std::string getTriMeshFilename()
	{
		try
		{
			Yaml::Node& parameters = root["parameters"];

			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}
			return parameters["tri_mesh"].As<std::string>();
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Example exception " << e.Type() << ": " << e.what() << std::endl;
			return std::string();
		}
	}

	std::string getMaskTexFilename()
	{
		try
		{
			Yaml::Node& parameters = root["parameters"];

			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}
			return parameters["mask_texture"].As<std::string>();
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Example exception " << e.Type() << ": " << e.what() << std::endl;
			return std::string();
		}
	}

	std::vector<std::string> getPolyfitFilename()
	{
		std::vector<std::string> filenames;
		try
		{
			Yaml::Node& polyfit = root["polyfit"];
			if (polyfit.IsSequence() == false)
			{
				throw Yaml::InternalException("polyfit is not of type sequence.");
			}

			for (auto itS = polyfit.Begin(); itS != polyfit.End(); itS++)
			{
				Yaml::Node& entry = (*itS).second;
				for (auto itSS = entry.Begin(); itSS != entry.End(); itSS++)
				{
					filenames.push_back((*itSS).second.As<std::string>());
				}
			}
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return filenames;
	}

	std::vector<float> getMaskWeights()
	{
		std::vector<float> weight;
		try
		{
			Yaml::Node& parameters = root["interface"];

			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}

			weight.push_back(parameters["mask_weight_x"].As<float>());
			weight.push_back(parameters["mask_weight_y"].As<float>());
			weight.push_back(parameters["mask_weight_z"].As<float>());

			return weight;
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return weight;
	}

	std::vector<float> getMousePositions()
	{
		std::vector<float> pos;
		try
		{
			Yaml::Node& parameters = root["interface"];

			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}

			pos.push_back(parameters["mouse_pos_0_x"].As<float>());
			pos.push_back(parameters["mouse_pos_0_y"].As<float>());
			pos.push_back(parameters["mouse_pos_1_x"].As<float>());
			pos.push_back(parameters["mouse_pos_1_y"].As<float>());
			pos.push_back(parameters["mouse_pos_2_x"].As<float>());
			pos.push_back(parameters["mouse_pos_2_y"].As<float>());
		
			return pos;
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
		return pos;
	}

	void setMousePosition(const std::string& param_name, float value)
	{
		try
		{
			Yaml::Node& parameters = root["interface"];
			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}
			parameters[param_name] = std::to_string(value);
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
	}

	template<typename T>
	void setParameter(const std::string& param_group, const std::string& param_name, T value)
	{
		try
		{
			Yaml::Node& parameters = root[param_group];
			if (parameters.IsMap() == false)
			{
				throw Yaml::InternalException("Parameters is not of type Map.");
			}
			parameters[param_name] = std::to_string(value);
		}
		catch (const Yaml::Exception e)
		{
			std::cout << "Exception " << e.Type() << ": " << e.what() << std::endl;
		}
	}
};
