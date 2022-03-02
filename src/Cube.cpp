#include "Cube.h"

Cube::Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context, Cube* newParent) : Model(dxdevice, dxdevice_context)
{
	parent = newParent;

	//Front face
	GiveVertexValues(v101, vec3f(-1.0f, -1.0f, 1.0f), vec3f({ 0, 0, 1 }), vec2f({ 0, 0 }));
	GiveVertexValues(v102, vec3f(1.0f, -1.0f, 1.0f), vec3f({ 0, 0, 1 }), vec2f({ 0, 1 }));
	GiveVertexValues(v103, vec3f(1.0f, 1.0f, 1.0f), vec3f({ 0, 0, 1 }), vec2f({ 1, 1 }));
	GiveVertexValues(v104, vec3f(-1.0f, 1.0f, 1.0f), vec3f({ 0, 0, 1 }), vec2f({ 1, 0 }));

	//Right face
	GiveVertexValues(v201, vec3f(1.0f, 1.0f, 1.0f), vec3f({ 1, 0, 0 }), vec2f({ 0, 0 }));
	GiveVertexValues(v202, vec3f(1.0f, -1.0f, 1.0f), vec3f({ 1, 0, 0 }), vec2f({ 0, 1 }));
	GiveVertexValues(v203, vec3f(1.0f, 1.0f, -1.0f), vec3f({ 1, 0, 0 }), vec2f({ 1, 1 }));
	GiveVertexValues(v204, vec3f(1.0f, -1.0f, -1.0f), vec3f({ 1, 0, 0 }), vec2f({ 1, 0 }));

	//Back face
	GiveVertexValues(v301, vec3f(-1.0f, -1.0f, -1.0f), vec3f({ 0, 0, -1 }), vec2f({ 0, 0 }));
	GiveVertexValues(v302, vec3f(1.0f, -1.0f, -1.0f), vec3f({ 0, 0, -1 }), vec2f({ 0, 1 }));
	GiveVertexValues(v303, vec3f(1.0f, 1.0f, -1.0f), vec3f({ 0, 0, -1 }), vec2f({ 1, 1 }));
	GiveVertexValues(v304, vec3f(-1.0f, 1.0f, -1.0f), vec3f({ 0, 0, -1 }), vec2f({ 1, 0 }));

	//Left face
	GiveVertexValues(v401, vec3f(-1.0f, 1.0f, 1.0f), vec3f({ -1, 0, 0 }), vec2f({ 0, 0 }));
	GiveVertexValues(v402, vec3f(-1.0f, -1.0f, 1.0f), vec3f({ -1, 0, 0 }), vec2f({ 0, 1 }));
	GiveVertexValues(v403, vec3f(-1.0f, 1.0f, -1.0f), vec3f({ -1, 0, 0 }), vec2f({ 1, 1 }));
	GiveVertexValues(v404, vec3f(-1.0f, -1.0f, -1.0f), vec3f({ -1, 0, 0 }), vec2f({ 1, 0 }));

	//Upper face
	GiveVertexValues(v501, vec3f(-1.0f, 1.0f, 1.0f), vec3f({ 0, 1, 0 }), vec2f({ 0, 0 }));
	GiveVertexValues(v502, vec3f(1.0f, 1.0f, 1.0f), vec3f({ 0, 1, 0 }), vec2f({ 0, 1 }));
	GiveVertexValues(v503, vec3f(-1.0f, 1.0f, -1.0f), vec3f({ 0, 1, 0 }), vec2f({ 1, 1 }));
	GiveVertexValues(v504, vec3f(1.0f, 1.0f, -1.0f), vec3f({ 0, 1, 0 }), vec2f({ 1, 0 }));

	//Lower face
	GiveVertexValues(v601, vec3f(-1.0f, -1.0f, 1.0f), vec3f({ 0, -1, 0 }), vec2f({ 0, 0 }));
	GiveVertexValues(v602, vec3f(1.0f, -1.0f, 1.0f), vec3f({ 0, -1, 0 }), vec2f({ 0, 1 }));
	GiveVertexValues(v603, vec3f(-1.0f, -1.0f, -1.0f), vec3f({ 0, -1, 0 }), vec2f({ 1, 1 }));
	GiveVertexValues(v604, vec3f(1.0f, -1.0f, -1.0f), vec3f({ 0, -1, 0 }), vec2f({ 1, 0 }));

	// Populate the index array with two triangles
	// Front Triangle #1
	CreateTriangle(0, 1, 3); 
	//  Front Triangle #2
	CreateTriangle(1, 2, 3);

	// Right Triangle #1
	CreateTriangle(4, 5, 6);
	// Right Triangle #2
	CreateTriangle(5, 7, 6);

	// Back Triangle #1
	CreateTriangle(11, 9, 8);
	// Back Triangle #2
	CreateTriangle(10, 9, 11);

	// Left Triangle #1
	CreateTriangle(14, 13, 12);
	// Left Triangle #2
	CreateTriangle(13, 14, 15);

	// Upper Triangle #1
	CreateTriangle(16, 17, 18);
	// Upper Triangle #2
	CreateTriangle(19, 18, 17);

	// Lower Triangle #1
	CreateTriangle(23, 21, 20);
	// Lower Triangle #2
	CreateTriangle(20, 22, 23);


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

	LoadTexture();
}

void Cube::Render() const
{
	// Bind our vertex buffer
	const UINT32 stride = sizeof(Vertex); //  sizeof(float) * 8;
	const UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Bind our index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
	dxdevice_context->PSSetConstantBuffers(1, 1, &materialAndShininessBuffer);

	//Bind material textures & sampler
	dxdevice_context->PSSetShaderResources(0, 1, &baseMaterial.diffuse_texture.texture_SRV);
	dxdevice_context->PSSetShaderResources(1, 1, &baseMaterial.normal_texture.texture_SRV);
	UpdateMaterialAndShininessBuffer(baseMaterial);

	// Make the drawcall
	dxdevice_context->DrawIndexed(nbr_indices, 0, 0);

}

void Cube::GiveVertexValues(Vertex vertex, vec3f pos, vec3f normal, vec2f texCoord)
{
	vertex.Pos = pos;
	vertex.Normal = normal;
	vertex.TexCoord = texCoord;

	vertices.push_back(vertex);
}

void Cube::CreateTriangle(int index1, int index2, int index3)
{
	indices.push_back(index1);
	indices.push_back(index2);
	indices.push_back(index3);

	Compute_tangentspace(vertices[index1], vertices[index2], vertices[index3]);
}

void Cube::AssignParent(Cube& cube)
{
	parent = &cube;
}

Cube* Cube::GetParent()
{
	return parent;
}

void Cube::SetTransform(mat4f _transform)
{
	transform = _transform;
}

mat4f Cube::GetTransform()
{
	return transform;
}

void Cube::SetRotation(mat4f _rotation)
{
	rotation = _rotation;
}

mat4f Cube::GetRotation()
{
	return rotation;
}

void Cube::SetScale(mat4f _scale)
{
	scale = _scale;
}

mat4f Cube::GetScale()
{
	return scale;
}

mat4f Cube::GetParentTransformAndRotation()
{
	return parent->GetTransform() * parent->GetRotation();
}

void Cube::SetTransformMatrix(mat4f _transform, mat4f _rotation, mat4f _scale) //current
{
	transform = _transform;
	rotation = _rotation;
	scale = _scale;

	transformMatrix = transform * rotation * scale;
}

void Cube::SetTransformMatrix(mat4f _transformMatrix)
{
	transformMatrix = _transformMatrix;
}

mat4f Cube::GetTransformMatrix()
{
	return transformMatrix;
}

mat4f Cube::InheritTransformAndRotation()
{
	parent->GetTransformMatrix().scaling(1);
	return parent->GetTransformMatrix();
}

void Cube::LoadTexture()
{
	HRESULT hr;

	if (baseMaterial.Kd_texture_filename.size())
	{
		hr = LoadTextureFromFile(dxdevice, dxdevice_context, baseMaterial.Kd_texture_filename.c_str(), &baseMaterial.diffuse_texture);
		std::cout << "\t" << baseMaterial.Kd_texture_filename.c_str() << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
	}
	
	if (baseMaterial.normal_texture_filename.size())
	{
		hr = LoadTextureFromFile(dxdevice, dxdevice_context, baseMaterial.normal_texture_filename.c_str(), &baseMaterial.normal_texture);
		std::cout << "\t" << baseMaterial.normal_texture_filename.c_str() << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
	}
}