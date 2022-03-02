//
//  Model.cpp
//
//  CJ Gribel 2016, cjgribel@gmail.com
//

#include "Model.h"

Model::Model(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : dxdevice(dxdevice), dxdevice_context(dxdevice_context)
{
	InitMaterialAndShininessBuffer();

	baseMaterial.Kd_texture_filename = "assets/textures/diffuseColor.jpg";
	baseMaterial.normal_texture_filename = "assets/textures/normalMap.png";
}



void Model::InitMaterialAndShininessBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
	MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBuffer_desc.ByteWidth = sizeof(MaterialAndShininessBuffer);
	MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBuffer_desc.MiscFlags = 0;
	MatrixBuffer_desc.StructureByteStride = 0;
	ASSERT(hr = dxdevice->CreateBuffer(&MatrixBuffer_desc, nullptr, &materialAndShininessBuffer));
}

void Model::UpdateMaterialAndShininessBuffer(Material material) const
{
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(materialAndShininessBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MaterialAndShininessBuffer* matAndShineBuffer = (MaterialAndShininessBuffer*)resource.pData;
	matAndShineBuffer->Ambient = vec4f(material.Ka, 0);
	matAndShineBuffer->Diffuse = vec4f(material.Kd, 0);
	matAndShineBuffer->Specular = vec4f(material.Ks, material.Shininess);
	dxdevice_context->Unmap(materialAndShininessBuffer, 0);
}

void Model::SetBasicMaterialValues(vec3f ka, vec3f kd, vec3f ks, float shininess)
{
	baseMaterial.Ka = ka;
	baseMaterial.Kd = kd;
	baseMaterial.Ks = ks;
	baseMaterial.Shininess = shininess;
}

void Model::Compute_tangentspace(Vertex& v0, Vertex& v1, Vertex& v2)
{
	float tx, ty, tz;
	float bx, by, bz;

	vec3f D = v1.Pos - v0.Pos;
	vec3f E = v2.Pos - v0.Pos;

	vec2f F = v1.TexCoord - v0.TexCoord;
	vec2f G = v2.TexCoord - v0.TexCoord;

	float inverseFG = (1 / (F.x * G.y - F.y * G.x));

	tx = inverseFG * (G.y * D.x - F.y * E.x);
	ty = inverseFG * (G.y * D.y - F.y * E.y);
	tz = inverseFG * (G.y * D.z - F.y * E.z);

	bx = inverseFG * (-G.x * D.x + F.x * E.x);
	by = inverseFG * (-G.x * D.y + F.x * E.y);
	bz = inverseFG * (-G.x * D.z + F.x * E.z);

	vec3f tangent = { tx, ty, tz };
	vec3f binormal = { bx, by, bz };

	v0.Tangent = v1.Tangent = v2.Tangent = tangent;
	v0.Binormal = v1.Binormal = v2.Binormal = binormal;
}

Material* Model::GetDefaultMaterial()
{
	return &baseMaterial;
}

void Model::ChangeDiffuseTexture(std::string filepath) //Remove
{
	LoadTextureFromFile(dxdevice, dxdevice_context, filepath.c_str(), &baseMaterial.diffuse_texture);
}

QuadModel::QuadModel(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context)
	: Model(dxdevice, dxdevice_context)
{
	// Vertex and index arrays
	// Once their data is loaded to GPU buffers, they are not needed anymore
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	// Populate the vertex array with 4 vertices
	Vertex v0, v1, v2, v3;
	v0.Pos = { -0.5, -0.5f, 0.0f };
	v0.Normal = { 0, 0, 1 };
	v0.TexCoord = { 0, 0 };
	v1.Pos = { 0.5, -0.5f, 0.0f };
	v1.Normal = { 0, 0, 1 };
	v1.TexCoord = { 0, 1 };
	v2.Pos = { 0.5, 0.5f, 0.0f };
	v2.Normal = { 0, 0, 1 };
	v2.TexCoord = { 1, 1 };
	v3.Pos = { -0.5, 0.5f, 0.0f };
	v3.Normal = { 0, 0, 1 };
	v3.TexCoord = { 1, 0 };
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	// Populate the index array with two triangles
	// Triangle #1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// Triangle #2
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0 };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = (UINT)(vertices.size() * sizeof(Vertex));
	// Data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &vertices[0];
	// Create vertex buffer on device using descriptor & data
	const HRESULT vhr = dxdevice->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);
	SETNAME(vertex_buffer, "VertexBuffer");

	//  Index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0 };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = (UINT)(indices.size() * sizeof(unsigned));
	// Data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	const HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);
	SETNAME(index_buffer, "IndexBuffer");

	nbr_indices = (unsigned int)indices.size();
}


void QuadModel::Render() const
{
	// Bind our vertex buffer
	const UINT32 stride = sizeof(Vertex); //  sizeof(float) * 8;
	const UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Bind our index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Make the drawcall
	dxdevice_context->DrawIndexed(nbr_indices, 0, 0);
}


OBJModel::OBJModel(
	const std::string& objfile,
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context, bool isSkybox)
	: Model(dxdevice, dxdevice_context)
{
	// Load the OBJ
	OBJLoader* mesh = new OBJLoader();
	mesh->Load(objfile);
	this->isSkybox = isSkybox;
	const char* filePaths[6] =
	{
		"assets/cubemaps/skybox/skybox_posx.png",
		"assets/cubemaps/skybox/skybox_negx.png",
		"assets/cubemaps/skybox/skybox_posy.png",
		"assets/cubemaps/skybox/skybox_negy.png",
		"assets/cubemaps/skybox/skybox_posz.png",
		"assets/cubemaps/skybox/skybox_negz.png" //wrong
	};

	//if (isSkybox)
	//{
	//	filePaths[0] = "assets/cubemaps/skybox/skybox_posx.png";
	//	filePaths[1] = "assets/cubemaps/skybox/skybox_negx.png";
	//	filePaths[2] = "assets/cubemaps/skybox/skybox_posy.png";
	//	filePaths[3] = "assets/cubemaps/skybox/skybox_negy.png";
	//	filePaths[4] = "assets/cubemaps/skybox/skybox_posz.png";
	//	filePaths[5] = "assets/cubemaps/skybox/skybox_negz.png";
	//}

	// Load and organize indices in ranges per drawcall (material)

	std::vector<unsigned> indices;
	unsigned int i_ofs = 0;

	for (auto& dc : mesh->drawcalls)
	{
		// Append the drawcall indices
		for (auto& tri : dc.tris)
		{
			indices.insert(indices.end(), tri.vi, tri.vi + 3);
		}

		// Create a range
		unsigned int i_size = (unsigned int)dc.tris.size() * 3;
		int mtl_index = dc.mtl_index > -1 ? dc.mtl_index : -1;
		index_ranges.push_back({ i_ofs, i_size, 0, mtl_index });

		i_ofs = (unsigned int)indices.size();

	}

	for (int i = 0; i < indices.size(); i += 3) // For all triangles
		Compute_tangentspace(
			mesh->vertices[indices[i + 0]],
			mesh->vertices[indices[i + 1]],
			mesh->vertices[indices[i + 2]]
		);

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0 };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = (UINT)(mesh->vertices.size() * sizeof(Vertex));
	// Data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &(mesh->vertices)[0];
	// Create vertex buffer on device using descriptor & data
	HRESULT vhr = dxdevice->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);
	SETNAME(vertex_buffer, "VertexBuffer");

	// Index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0 };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = (UINT)(indices.size() * sizeof(unsigned));
	// Data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);
	SETNAME(index_buffer, "IndexBuffer");

	// Copy materials from mesh
	append_materials(mesh->materials);

	std::string objectMapPath;
	size_t pos = objfile.rfind("/");

	if (pos != std::string::npos)
		objectMapPath = objfile.substr(0, pos + 1);

	// Go through materials and load textures (if any) to device
	std::cout << "Loading textures..." << std::endl;
	for (auto& mtl : materials)
	{
		HRESULT hr;

		// Load Diffuse texture
		//
		if (mtl.Kd_texture_filename.size())
		{
			hr = LoadTextureFromFile(dxdevice, dxdevice_context, mtl.Kd_texture_filename.c_str(), &mtl.diffuse_texture);
			std::cout << "\t" << mtl.Kd_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
		else if(isSkybox)
		{
			hr = LoadCubeTextureFromFile(dxdevice, filePaths, &baseMaterial.diffuse_texture);
			std::cout << "loaded extra skybox texture: " << filePaths << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
		else
		{
			hr = LoadTextureFromFile(dxdevice, dxdevice_context, baseMaterial.Kd_texture_filename.c_str(), &mtl.diffuse_texture);
			std::cout << "loaded extra texture: " << baseMaterial.Kd_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}

		// Load Normal texture
		if (mtl.normal_texture_filename.size())
		{
			hr = LoadTextureFromFile(dxdevice, dxdevice_context, mtl.normal_texture_filename.c_str(), &mtl.normal_texture);
			std::cout << "\t" << mtl.normal_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
		else //This gives all models without their own normal maps a "default" normal map
		{
			hr = LoadTextureFromFile(dxdevice, dxdevice_context, baseMaterial.normal_texture_filename.c_str(), &mtl.normal_texture);
			std::cout << "loaded extra texture: " << baseMaterial.normal_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}

		if (mtl.specular_texture_filename.size())
		{
			hr = LoadTextureFromFile(dxdevice, dxdevice_context, mtl.specular_texture_filename.c_str(), &mtl.specular_texture);
			std::cout << "\t" << mtl.specular_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}

	}

	std::cout << "Done." << std::endl;

	SAFE_DELETE(mesh);
}


void OBJModel::Render() const
{
	// Bind vertex buffer
	const UINT32 stride = sizeof(Vertex);
	const UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Bind index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
	dxdevice_context->PSSetConstantBuffers(1, 1, &materialAndShininessBuffer);

	// Iterate drawcalls
	for (auto& irange : index_ranges)
	{
		// Fetch material
		Material mtl = materials[irange.mtl_index];
		//Bind Textures
		dxdevice_context->PSSetShaderResources(0, 1, &mtl.diffuse_texture.texture_SRV);
		dxdevice_context->PSSetShaderResources(1, 1, &mtl.normal_texture.texture_SRV);
		//Bind Sampler
		

		UpdateMaterialAndShininessBuffer(mtl);
		UpdateMaterialAndShininessBuffer(baseMaterial); //Adds material and shine to all models using the base Material

		// Make the drawcall
		dxdevice_context->DrawIndexed(irange.size, irange.start, 0);
	}
}

OBJModel::~OBJModel()
{
	for (auto& material : materials)
	{
		SAFE_RELEASE(material.diffuse_texture.texture_SRV);
		SAFE_RELEASE(material.normal_texture.texture_SRV);
		// Release other used textures ...
	}
}