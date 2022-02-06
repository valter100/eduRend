//
//  Model.cpp
//
//  CJ Gribel 2016, cjgribel@gmail.com
//

#include "Model.h"

Model::Model(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : dxdevice(dxdevice), dxdevice_context(dxdevice_context)
{ 
	InitMaterialAndShininessBuffer();
	CreateSamplerState();
	shininess = 5.0f;
}

void Model::CreateSamplerState()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sd =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd, &samplerState));

	samplerStates.push_back(samplerState);

	D3D11_SAMPLER_DESC sd2 =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd2, &samplerState2));

	samplerStates.push_back(samplerState2);

	D3D11_SAMPLER_DESC sd3 =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd3, &samplerState3));

	samplerStates.push_back(samplerState3);

	D3D11_SAMPLER_DESC sd4 =
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd4, &samplerState4));

	samplerStates.push_back(samplerState4);

	D3D11_SAMPLER_DESC sd5 =
	{
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd5, &samplerState5));

	samplerStates.push_back(samplerState5);

	D3D11_SAMPLER_DESC sd6 =
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		16,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f },
		-FLT_MAX,
		FLT_MAX,
	};
	ASSERT(hr = dxdevice->CreateSamplerState(&sd6, &samplerState6));

	samplerStates.push_back(samplerState6);
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
	matAndShineBuffer->Specular = vec4f(material.Ks, shininess);
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
	vec3f tangent, binormal;
	// TODO: compute tangent and binormal vectors
	// using Lengyel’s method, as given in lecture

	vec3f D = v1.Pos - v0.Pos; //Q1
	vec3f E = v2.Pos - v0.Pos; //Q2

	vec2f F = v1.TexCoord - v0.TexCoord; //S1, T1
	vec2f G = v2.TexCoord - v0.TexCoord; //S2, T2

	D = tangent*F.x + binormal*F.y; //Q1
	E = tangent*G.x + binormal*G.y; //Q2

	mat2f FG = { (F.x, F.y), (G.x, G.y) };

	mat3f DE = { (D.x, D.y, D.z), (E.x, E.y, E.z), (1,1,1) };
	mat2f GF = FG.invert();

	//vec3f DEGF[2][3] = {
	//	vec3f(GF.m11 * DE.m11 + GF.m12 * DE.m21,
	//	 GF.m11 * DE.m12 + GF.m11 * DE.m22,
	//	 GF.m11 * DE.m13 + GF.m12 * DE.m23),
	//	vec3f(GF.m21 * DE.m11 + GF.m21 * DE.m21,
	//	 GF.m21 * DE.m11 + GF.m22 * DE.m21,
	//	 GF.m21 * DE.m13 + GF.m22 * DE.m23) };

	//mat3f DEGF = { 
	//	(GF.m11 * DE.m11 + GF.m12 * DE.m21, 
	//	 GF.m11 * DE.m12 + GF.m11 * DE.m22, 
	//	 GF.m11 * DE.m13 + GF.m12 * DE.m23), 
	//	(GF.m21 * DE.m11 + GF.m21 * DE.m21,
	//	 GF.m21 * DE.m11 + GF.m22 * DE.m21,
	//	 GF.m21 * DE.m13 + GF.m22 * DE.m23),
	//	 (1,1,1)
	//};

	float inverseFG = (1 / ((F.x * G.y) - (F.y * G.x)));

	//Matris till vänster tar man rad * column 

	float tx = inverseFG * (GF.m11 * DE.m11 + GF.m12 * DE.m21);
	float ty = inverseFG * (GF.m11 * DE.m12 + GF.m12 * DE.m22);
	float tz = inverseFG * (GF.m11 * DE.m13 + GF.m12 * DE.m23);

	float bx = inverseFG * (GF.m21 * DE.m11 + GF.m22 * DE.m21);
	float by = inverseFG * (GF.m21 * DE.m12 + GF.m22 * DE.m22);
	float bz = inverseFG * (GF.m11 * DE.m13 + GF.m22 * DE.m23);

	tangent = vec3f(tx, ty, tz);
	binormal = vec3f(bx, by, bz);

	//tangent.normalize();
	//binormal.normalize();

	v0.Tangent = v1.Tangent = v2.Tangent = tangent;
	v0.Binormal = v1.Binormal = v2.Binormal = binormal;
}

void Model::ChangeSamplerState(const float change)
{
	samplerStateIndex += change;

	if (samplerStateIndex >= samplerStates.size())
	{
		samplerStateIndex = 0;
	}
	else if (samplerStateIndex < 0)
	{
		samplerStateIndex = samplerStates.size() - 1;
	}

	std::cout << samplerStateIndex << std::endl;
}

ID3D11SamplerState* Model::GetCurrentSamplerState()
{
	return samplerStates[samplerStateIndex];
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
	vbufferDesc.ByteWidth = (UINT)(vertices.size()*sizeof(Vertex));
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
	ibufferDesc.ByteWidth = (UINT)(indices.size()*sizeof(unsigned));
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
	ID3D11DeviceContext* dxdevice_context)
	: Model(dxdevice, dxdevice_context)
{
	// Load the OBJ
	OBJLoader* mesh = new OBJLoader();
	mesh->Load(objfile);

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

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0 };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = (UINT)(mesh->vertices.size()*sizeof(Vertex));
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
	ibufferDesc.ByteWidth = (UINT)(indices.size()*sizeof(unsigned));
	// Data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);
	SETNAME(index_buffer, "IndexBuffer");
    
	for (int i = 0; i < indices.size(); i += 3) // For all triangles
		Model::Compute_tangentspace(
			mesh->vertices[indices[i + 0]],
			mesh->vertices[indices[i + 1]],
			mesh->vertices[indices[i + 2]]
		);

	// Copy materials from mesh
	append_materials(mesh->materials);

	std::string objectMapPath;
	size_t pos = objfile.rfind("/");

	if (pos != std::string::npos)
		objectMapPath = objfile.substr(0, pos + 1);

	baseMaterial.Kd_texture_filename = objectMapPath + "diffuseColor.jpg";
	//baseMaterial.normal_texture_filename = objectMapPath + "normalMap.png";
	baseMaterial.normal_texture_filename = "assets/objects/sphere/normalMap.png";
	
	std::cout << "LOOKING FOR DIFFUSE TEX IN: " << baseMaterial.Kd_texture_filename << std::endl;


	// Go through materials and load textures (if any) to device
	std::cout << "Loading textures..." << std::endl;
	for (auto& mtl : materials)
	{
		HRESULT hr;

		// Load Diffuse texture
		//
		if (mtl.Kd_texture_filename.size()) 
		{
			hr = LoadTextureFromFile(dxdevice, mtl.Kd_texture_filename.c_str(), &mtl.diffuse_texture);
			std::cout << "\t" << mtl.Kd_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
		else
		{
			hr = LoadTextureFromFile(dxdevice, baseMaterial.Kd_texture_filename.c_str(), &mtl.diffuse_texture);
		}

		// Load Normal texture
		if (mtl.normal_texture_filename.size())
		{
			hr = LoadTextureFromFile(dxdevice, mtl.normal_texture_filename.c_str(), &mtl.normal_texture);
			std::cout << "\t" << mtl.normal_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
		else 
		{
			hr = LoadTextureFromFile(dxdevice, baseMaterial.normal_texture_filename.c_str(), &mtl.normal_texture);
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
		const Material& mtl = materials[irange.mtl_index];

		if (materials.size() == 0)
		{
			const Material& mtl = baseMaterial;
		}

		// Bind diffuse texture to slot t0 of the PS
		dxdevice_context->PSSetShaderResources(0, 1, &mtl.diffuse_texture.texture_SRV);
		dxdevice_context->PSSetShaderResources(1, 1, &mtl.normal_texture.texture_SRV);
		dxdevice_context->PSSetSamplers(0, 1, &samplerStates[samplerStateIndex]);

		UpdateMaterialAndShininessBuffer(mtl);

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
	SAFE_RELEASE(samplerState);
}