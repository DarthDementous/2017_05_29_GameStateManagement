#pragma once
#include <vector>
#include <memory>
#include <Texture.h>
#include <Font.h>
#include <Audio.h>



class ResourceBase
{
protected:
	ResourceBase(const std::string& filename) : m_filename(filename) {};
public:
	~ResourceBase() {};

	std::string getFilename() { return m_filename; }
	virtual void* getData() = 0;

	template<typename T>
	T* as() { return (T*)getData(); }

protected:
	std::string m_filename;
};

// Rather than define a new class for each type of resource, use templates. Sub-classes will be 
// created automatically for us
template<class T>
class Resource : public ResourceBase
{
public:
	// Each type of resource may have a different constructor argument list (e.g., the arguments needed
	// for a font are different to those of a texture).
	// Use a varadic argument list to account for these differences
	template<typename ...Args>
	Resource(const std::string& filename, Args... args) : ResourceBase(filename) {
		m_data = std::unique_ptr<T>(new T(filename.c_str(), args...));
	};
	~Resource() {};

	void* getData() {
		return m_data.get();
	}
private:
	std::unique_ptr<T> m_data;
};

class ResourceManager
{
public:
	// define an enum to allow us to specify what type of resource we want to get from the Resource Manager
	enum ResourceType {
		TEXTURE = 0,
		FONT,
		AUDIO
	};
	
	std::vector< std::shared_ptr<ResourceBase> > m_resources;
	
	// keep the constructor, copy constructor and assignment allocator private
	ResourceManager() {};
	ResourceManager(const ResourceManager&) {};
	ResourceManager& operator=(const ResourceManager&) {};

public:
	static ResourceManager& getInstance();
	~ResourceManager() {};
		
	std::shared_ptr<ResourceBase> get(const std::string filename, ResourceType type);
	void collectGarbage();
	int getCount();
};

